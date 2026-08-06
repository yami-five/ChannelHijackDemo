from __future__ import annotations

import math
import re
import sys
from enum import Enum

try:
    from .blender_transform_converter import (
        ENGINE_ROTATION_ANGLE_SCALE,
        _format_c_float,
        blender_location_to_engine,
        blender_quaternion_to_engine_axis_angle,
        blender_scale_to_engine,
    )
except ImportError:
    from blender_transform_converter import (
        ENGINE_ROTATION_ANGLE_SCALE,
        _format_c_float,
        blender_location_to_engine,
        blender_quaternion_to_engine_axis_angle,
        blender_scale_to_engine,
    )


MAX_KEYFRAMES = 256
MAX_OUTPUT_FRAMES = 65535

Vector = tuple[float, ...]
Quaternion = tuple[float, float, float, float]
ModelAnimationValue = tuple[float, float, float, float]

_NUMBER_PATTERN = re.compile(
    r"[-+]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][-+]?\d+)?"
)
_C_IDENTIFIER_PATTERN = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


class TransformType(Enum):
    TRANSLATION = "translation"
    ROTATION = "rotation"
    SCALE = "scale"

    @property
    def component_count(self) -> int:
        return 4 if self is TransformType.ROTATION else 3

    @property
    def input_format(self) -> str:
        return "W, X, Y, Z" if self is TransformType.ROTATION else "X, Y, Z"


def parse_keyframes(text: str, transform_type: TransformType) -> list[Vector]:
    """Parse one Blender-space keyframe per non-empty input line."""
    keyframes: list[Vector] = []
    for line_number, raw_line in enumerate(text.splitlines(), start=1):
        line = raw_line.split("#", 1)[0].strip()
        if not line:
            continue

        values = tuple(float(value) for value in _NUMBER_PATTERN.findall(line))
        if len(values) != transform_type.component_count:
            raise ValueError(
                f"Line {line_number}: expected {transform_type.input_format}, "
                f"found {len(values)} values."
            )
        if not all(math.isfinite(value) for value in values):
            raise ValueError(f"Line {line_number}: values must be finite numbers.")

        keyframes.append(values)
        if len(keyframes) > MAX_KEYFRAMES:
            raise ValueError(
                f"The keyframe list cannot contain more than {MAX_KEYFRAMES} items."
            )

    if not keyframes:
        raise ValueError("Enter at least one keyframe.")
    return keyframes


def _normalize_quaternion(value: Vector) -> Quaternion:
    length = math.sqrt(sum(component * component for component in value))
    if length < 1e-12:
        raise ValueError("Rotation quaternion cannot have zero length.")
    return tuple(component / length for component in value)  # type: ignore[return-value]


def _lerp(left: Vector, right: Vector, factor: float) -> Vector:
    return tuple(
        left_value + (right_value - left_value) * factor
        for left_value, right_value in zip(left, right)
    )


def _slerp(left: Vector, right: Vector, factor: float) -> Quaternion:
    start = _normalize_quaternion(left)
    end = _normalize_quaternion(right)
    dot = sum(a * b for a, b in zip(start, end))

    if dot < 0.0:
        end = tuple(-component for component in end)  # type: ignore[assignment]
        dot = -dot

    dot = max(-1.0, min(1.0, dot))
    if dot > 0.9995:
        return _normalize_quaternion(_lerp(start, end, factor))

    angle = math.acos(dot)
    sin_angle = math.sin(angle)
    start_weight = math.sin((1.0 - factor) * angle) / sin_angle
    end_weight = math.sin(factor * angle) / sin_angle
    return tuple(
        start_weight * start_value + end_weight * end_value
        for start_value, end_value in zip(start, end)
    )  # type: ignore[return-value]


def _keyframe_positions(keyframe_count: int, frame_count: int) -> list[int]:
    if keyframe_count == 1:
        return [0]

    intervals = keyframe_count - 1
    return [
        (index * (frame_count - 1) + intervals // 2) // intervals
        for index in range(keyframe_count)
    ]


def interpolate_keyframes(
    keyframes: list[Vector], frame_count: int, transform_type: TransformType
) -> list[Vector]:
    """Distribute keyframes evenly and generate one value per output frame."""
    if frame_count < 1 or frame_count > MAX_OUTPUT_FRAMES:
        raise ValueError(
            f"Frame count must be between 1 and {MAX_OUTPUT_FRAMES}."
        )
    if frame_count < len(keyframes):
        raise ValueError("Frame count cannot be smaller than keyframe count.")

    if len(keyframes) == 1:
        value = (
            _normalize_quaternion(keyframes[0])
            if transform_type is TransformType.ROTATION
            else keyframes[0]
        )
        return [value] * frame_count

    positions = _keyframe_positions(len(keyframes), frame_count)
    frames: list[Vector] = []
    segment = 0
    for frame_index in range(frame_count):
        while (
            segment + 1 < len(positions) - 1
            and frame_index > positions[segment + 1]
        ):
            segment += 1

        segment_start = positions[segment]
        segment_end = positions[segment + 1]
        factor = (frame_index - segment_start) / (segment_end - segment_start)
        if transform_type is TransformType.ROTATION:
            frames.append(
                _slerp(keyframes[segment], keyframes[segment + 1], factor)
            )
        else:
            frames.append(
                _lerp(keyframes[segment], keyframes[segment + 1], factor)
            )
    return frames


def convert_frame(
    frame: Vector,
    transform_type: TransformType,
    geometry_in_engine_axes: bool = False,
) -> ModelAnimationValue:
    """Convert one interpolated Blender value to engine w/x/y/z values."""
    if transform_type is TransformType.TRANSLATION:
        x, y, z = blender_location_to_engine(frame)  # type: ignore[arg-type]
        return 0.0, x, y, z

    if transform_type is TransformType.SCALE:
        x, y, z = blender_scale_to_engine(  # type: ignore[arg-type]
            frame, geometry_in_engine_axes
        )
        return 0.0, x, y, z

    angle_turns, axis = blender_quaternion_to_engine_axis_angle(
        frame,  # type: ignore[arg-type]
        geometry_in_engine_axes=geometry_in_engine_axes,
    )
    return angle_turns * ENGINE_ROTATION_ANGLE_SCALE, *axis


def _preserve_rotation_axis(
    values: list[ModelAnimationValue],
) -> list[ModelAnimationValue]:
    nonzero_frames = [
        index for index, value in enumerate(values) if abs(value[0]) >= 0.00000005
    ]
    if not nonzero_frames:
        return [(value[0], 0.0, 0.0, 1.0) for value in values]

    result = values.copy()
    for index, value in enumerate(values):
        if abs(value[0]) >= 0.00000005:
            continue
        nearest = min(nonzero_frames, key=lambda frame: abs(frame - index))
        result[index] = (value[0], *values[nearest][1:])
    return result


def generate_inc_code(
    name: str,
    transform_type: TransformType,
    keyframes: list[Vector],
    frame_count: int,
    looped: bool = False,
    delay: int = 0,
    geometry_in_engine_axes: bool = False,
) -> str:
    """Generate ModelAnimation declarations ready to paste into an .inc file."""
    name = name.strip()
    if not _C_IDENTIFIER_PATTERN.fullmatch(name):
        raise ValueError("Name must be a valid C identifier.")
    if delay < 0:
        raise ValueError("Delay cannot be negative.")

    frames = interpolate_keyframes(keyframes, frame_count, transform_type)
    if looped:
        frames.extend(frames[-2:0:-1])
    if len(frames) + delay > MAX_OUTPUT_FRAMES:
        raise ValueError(
            f"The animation cannot contain more than {MAX_OUTPUT_FRAMES} frames "
            "including delay."
        )
    frames = [frames[0]] * delay + frames
    values = [
        convert_frame(frame, transform_type, geometry_in_engine_axes)
        for frame in frames
    ]
    if transform_type is TransformType.ROTATION:
        values = _preserve_rotation_axis(values)
    values_name = f"{name}_values"

    lines = [f"static const ModelAnimationValue {values_name}[] = {{"]
    for w, x, y, z in values:
        formatted = ", ".join(map(_format_c_float, (w, x, y, z)))
        lines.append(f"    {{{formatted}}},")
    lines.extend(
        (
            "};",
            "",
            f"static const ModelAnimation {name} = {{",
            f"    .values = {values_name},",
            f"    .values_count = {len(values)}u,",
            "};",
        )
    )
    return "\n".join(lines)


def run_gui() -> int:
    try:
        from PySide6.QtWidgets import (
            QApplication,
            QCheckBox,
            QComboBox,
            QFormLayout,
            QHBoxLayout,
            QLabel,
            QLineEdit,
            QMainWindow,
            QMessageBox,
            QPlainTextEdit,
            QPushButton,
            QSpinBox,
            QVBoxLayout,
            QWidget,
        )
    except ImportError:
        print(
            "PySide6 is not installed. Install it with: pip install PySide6",
            file=sys.stderr,
        )
        return 1

    class ModelAnimationGeneratorWindow(QMainWindow):
        def __init__(self) -> None:
            super().__init__()
            self.setWindowTitle("Euzebia3D Model Animation Generator")
            self.resize(900, 720)

            central_widget = QWidget()
            self.setCentralWidget(central_widget)
            layout = QVBoxLayout(central_widget)

            form = QFormLayout()
            layout.addLayout(form)

            self.transform_type_input = QComboBox()
            self.transform_type_input.addItem(
                "Translation", TransformType.TRANSLATION.value
            )
            self.transform_type_input.addItem(
                "Rotation (quaternion)", TransformType.ROTATION.value
            )
            self.transform_type_input.addItem(
                "Scale", TransformType.SCALE.value
            )
            form.addRow("Transform type:", self.transform_type_input)

            self.geometry_axes_input = QComboBox()
            self.geometry_axes_input.addItem("Blender (Z up)", False)
            self.geometry_axes_input.addItem("Engine (Y up)", True)
            self.geometry_axes_input.setToolTip(
                "Coordinate system used by vertices in the exported OBJ. "
                "Use the same value as OBJ axes in Euzebia3D Transform Copier."
            )
            form.addRow("OBJ axes:", self.geometry_axes_input)

            self.name_input = QLineEdit("model_animation")
            self.name_input.setPlaceholderText("Valid C identifier")
            form.addRow("Animation name:", self.name_input)

            self.frame_count_input = QSpinBox()
            self.frame_count_input.setRange(1, MAX_OUTPUT_FRAMES)
            self.frame_count_input.setValue(30)
            form.addRow("Frame count:", self.frame_count_input)

            self.delay_input = QSpinBox()
            self.delay_input.setRange(0, MAX_OUTPUT_FRAMES - 1)
            self.delay_input.setValue(0)
            self.delay_input.setToolTip(
                "Add this many copies of the first calculated frame at the "
                "beginning of the animation."
            )
            form.addRow("Delay (frames):", self.delay_input)

            self.looped_input = QCheckBox()
            self.looped_input.setToolTip(
                "Append the animation in reverse without duplicating its endpoints."
            )
            form.addRow("Looped:", self.looped_input)

            self.format_label = QLabel()
            layout.addWidget(self.format_label)

            self.keyframes_input = QPlainTextEdit()
            self.keyframes_input.setPlaceholderText("0.0, 0.0, 0.0\n1.0, 2.0, 3.0")
            layout.addWidget(self.keyframes_input, 1)

            hint = QLabel(
                "Enter one keyframe per line, up to 256. Keyframes are spaced "
                "evenly and the values between them are interpolated."
            )
            hint.setWordWrap(True)
            layout.addWidget(hint)

            actions = QHBoxLayout()
            self.generate_button = QPushButton("Generate .inc code")
            self.copy_button = QPushButton("Copy code")
            actions.addWidget(self.generate_button)
            actions.addWidget(self.copy_button)
            layout.addLayout(actions)

            self.status_label = QLabel()
            layout.addWidget(self.status_label)

            self.output = QPlainTextEdit()
            self.output.setReadOnly(True)
            layout.addWidget(self.output, 2)

            self.transform_type_input.currentIndexChanged.connect(
                self.update_input_format
            )
            self.generate_button.clicked.connect(self.generate)
            self.copy_button.clicked.connect(self.copy_output)
            self.update_input_format()

        def selected_transform_type(self) -> TransformType:
            return TransformType(self.transform_type_input.currentData())

        def update_input_format(self) -> None:
            transform_type = self.selected_transform_type()
            self.format_label.setText(
                "Blender keyframes — format: " + transform_type.input_format
            )
            if transform_type is TransformType.ROTATION:
                self.keyframes_input.setPlaceholderText(
                    "1.0, 0.0, 0.0, 0.0\n0.9238795, 0.3826834, 0.0, 0.0"
                )
            else:
                self.keyframes_input.setPlaceholderText(
                    "0.0, 0.0, 0.0\n1.0, 2.0, 3.0"
                )

        def generate(self) -> None:
            transform_type = self.selected_transform_type()
            try:
                keyframes = parse_keyframes(
                    self.keyframes_input.toPlainText(), transform_type
                )
                output = generate_inc_code(
                    self.name_input.text(),
                    transform_type,
                    keyframes,
                    self.frame_count_input.value(),
                    looped=self.looped_input.isChecked(),
                    delay=self.delay_input.value(),
                    geometry_in_engine_axes=bool(
                        self.geometry_axes_input.currentData()
                    ),
                )
            except ValueError as exc:
                QMessageBox.warning(self, "Invalid input", str(exc))
                return

            self.output.setPlainText(output)
            output_frame_count = self.frame_count_input.value()
            if self.looped_input.isChecked():
                output_frame_count = max(1, output_frame_count * 2 - 2)
            output_frame_count += self.delay_input.value()
            self.status_label.setText(
                f"Generated {output_frame_count} frames."
            )

        def copy_output(self) -> None:
            if not self.output.toPlainText():
                self.generate()
            if self.output.toPlainText():
                QApplication.clipboard().setText(self.output.toPlainText())
                self.status_label.setText("Code copied to the clipboard.")

    app = QApplication(sys.argv)
    window = ModelAnimationGeneratorWindow()
    window.show()
    return app.exec()


def main() -> int:
    return run_gui()


if __name__ == "__main__":
    raise SystemExit(main())
