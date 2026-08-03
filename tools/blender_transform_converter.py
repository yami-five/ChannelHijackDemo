from __future__ import annotations

import argparse
import math
import sys
from collections.abc import Sequence


Vector3 = tuple[float, float, float]
Quaternion = tuple[float, float, float, float]


def blender_location_to_engine(location: Vector3) -> Vector3:
    """Convert Blender's (X, Y, Z-up) position to engine (X, Y-up, Z)."""
    x, y, z = location
    return x, z, -y


def _quaternion_multiply(left: Quaternion, right: Quaternion) -> Quaternion:
    lw, lx, ly, lz = left
    rw, rx, ry, rz = right
    return (
        lw * rw - lx * rx - ly * ry - lz * rz,
        lw * rx + lx * rw + ly * rz - lz * ry,
        lw * ry - lx * rz + ly * rw + lz * rx,
        lw * rz + lx * ry - ly * rx + lz * rw,
    )


def _blender_xyz_euler_quaternion(rotation_degrees: Vector3) -> Quaternion:
    """Return Blender XYZ Euler rotation as a (w, x, y, z) quaternion."""
    x, y, z = (math.radians(value) * 0.5 for value in rotation_degrees)
    qx = (math.cos(x), math.sin(x), 0.0, 0.0)
    qy = (math.cos(y), 0.0, math.sin(y), 0.0)
    qz = (math.cos(z), 0.0, 0.0, math.sin(z))
    return _quaternion_multiply(_quaternion_multiply(qz, qy), qx)


def blender_xyz_euler_to_engine_axis_angle(
    rotation_degrees: Vector3,
) -> tuple[float, Vector3]:
    """Convert Blender XYZ Euler degrees to engine turns and rotation axis."""
    w, x, y, z = _blender_xyz_euler_quaternion(rotation_degrees)

    # q and -q describe the same rotation. Keeping w positive produces the
    # shortest axis-angle representation and an angle no greater than 180 deg.
    if w < 0.0:
        w, x, y, z = -w, -x, -y, -z

    # Apply the same basis change as positions: (X, Y, Z) -> (X, Z, -Y).
    engine_x, engine_y, engine_z = x, z, -y
    vector_length = math.sqrt(
        engine_x * engine_x + engine_y * engine_y + engine_z * engine_z
    )
    if vector_length < 1e-12:
        return 0.0, (1.0, 0.0, 0.0)

    clamped_w = max(-1.0, min(1.0, w))
    angle_turns = (2.0 * math.acos(clamped_w)) / math.tau
    axis = (
        engine_x / vector_length,
        engine_y / vector_length,
        engine_z / vector_length,
    )
    return angle_turns, axis


def _finite_float(value: str) -> float:
    result = float(value)
    if not math.isfinite(result):
        raise argparse.ArgumentTypeError("values must be finite numbers")
    return result


def _format_c_float(value: float) -> str:
    if abs(value) < 0.00000005:
        value = 0.0
    text = f"{value:.7f}".rstrip("0").rstrip(".")
    if "." not in text:
        text += ".0"
    return f"{text}f"


def generate_c_snippet(mesh: str, location: Vector3, rotation: Vector3) -> str:
    mesh = mesh.strip()
    if not mesh:
        raise ValueError("Mesh expression cannot be empty.")

    translation = blender_location_to_engine(location)
    angle_turns, axis = blender_xyz_euler_to_engine_axis_angle(rotation)
    angle, axis_x, axis_y, axis_z = map(
        _format_c_float, (angle_turns, *axis)
    )
    translate_x, translate_y, translate_z = map(_format_c_float, translation)
    return "\n".join(
        (
            f"add_mesh_transformation(engine_ctx, {mesh}, {angle},",
            f"                        {axis_x}, {axis_y}, {axis_z},",
            "                        MODEL_TRANSFORM_ROTATE);",
            f"add_mesh_transformation(engine_ctx, {mesh}, 0.0f,",
            f"                        {translate_x}, {translate_y}, {translate_z},",
            "                        MODEL_TRANSFORM_TRANSLATE);",
        )
    )


def run_gui(
    initial_location: Vector3 = (0.0, 0.0, 0.0),
    initial_rotation: Vector3 = (0.0, 0.0, 0.0),
    initial_mesh: str = "mesh",
) -> int:
    try:
        from PySide6.QtWidgets import (
            QApplication,
            QDoubleSpinBox,
            QFormLayout,
            QHBoxLayout,
            QLabel,
            QLineEdit,
            QMainWindow,
            QMessageBox,
            QPlainTextEdit,
            QPushButton,
            QVBoxLayout,
            QWidget,
        )
    except ImportError:
        print(
            "PySide6 is not installed. Install it with: pip install PySide6",
            file=sys.stderr,
        )
        return 1

    class BlenderTransformConverterWindow(QMainWindow):
        def __init__(self) -> None:
            super().__init__()
            self.setWindowTitle("Blender Transform Converter")
            self.resize(850, 520)

            central_widget = QWidget()
            self.setCentralWidget(central_widget)
            layout = QVBoxLayout(central_widget)
            form = QFormLayout()
            layout.addLayout(form)

            self.location_inputs, location_layout = self.create_vector_inputs(
                initial_location, " m"
            )
            form.addRow("Blender Location:", location_layout)

            self.rotation_inputs, rotation_layout = self.create_vector_inputs(
                initial_rotation, "°"
            )
            form.addRow("Blender Rotation (XYZ Euler):", rotation_layout)

            self.mesh_input = QLineEdit(initial_mesh)
            self.mesh_input.setPlaceholderText("For example: assets.maggot1")
            form.addRow("C mesh expression:", self.mesh_input)

            mapping_label = QLabel(
                "Axis mapping: Blender (X, Y, Z) → Euzebia3D (X, Z, -Y)"
            )
            layout.addWidget(mapping_label)

            actions_layout = QHBoxLayout()
            self.convert_button = QPushButton("Convert")
            self.copy_button = QPushButton("Copy generated code")
            actions_layout.addWidget(self.convert_button)
            actions_layout.addWidget(self.copy_button)
            layout.addLayout(actions_layout)

            self.status_label = QLabel()
            layout.addWidget(self.status_label)

            self.output = QPlainTextEdit()
            self.output.setReadOnly(True)
            layout.addWidget(self.output)

            self.convert_button.clicked.connect(self.convert)
            self.copy_button.clicked.connect(self.copy_output)
            self.convert()

        @staticmethod
        def create_vector_inputs(
            initial_values: Vector3, suffix: str
        ) -> tuple[tuple[QDoubleSpinBox, ...], QHBoxLayout]:
            row = QHBoxLayout()
            inputs = []
            for axis, value in zip("XYZ", initial_values):
                row.addWidget(QLabel(f"{axis}:"))
                input_widget = QDoubleSpinBox()
                input_widget.setRange(-1_000_000.0, 1_000_000.0)
                input_widget.setDecimals(6)
                input_widget.setSingleStep(0.1)
                input_widget.setSuffix(suffix)
                input_widget.setValue(value)
                row.addWidget(input_widget)
                inputs.append(input_widget)
            return tuple(inputs), row

        def convert(self) -> None:
            location = tuple(input_widget.value() for input_widget in self.location_inputs)
            rotation = tuple(input_widget.value() for input_widget in self.rotation_inputs)
            try:
                output = generate_c_snippet(
                    mesh=self.mesh_input.text(),
                    location=location,
                    rotation=rotation,
                )
            except ValueError as exc:
                QMessageBox.warning(self, "Invalid input", str(exc))
                return

            self.output.setPlainText(output)
            self.status_label.setText("Transform converted to Euzebia3D C code.")

        def copy_output(self) -> None:
            QApplication.clipboard().setText(self.output.toPlainText())
            self.status_label.setText("Generated code copied to clipboard.")

    app = QApplication(sys.argv)
    window = BlenderTransformConverterWindow()
    window.show()
    return app.exec()


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Convert a Blender XYZ Euler transform to Euzebia3D axis-angle "
            "rotation and translation calls."
        ),
        epilog=(
            "Example: python tools/blender_transform_converter.py "
            "--location -3.5142 4.173 0.12 "
            "--rotation 88.756 9.9085 -24.182 --mesh assets.maggot1"
        ),
    )
    parser.add_argument(
        "--location",
        nargs=3,
        type=_finite_float,
        metavar=("X", "Y", "Z"),
        help="Blender Location values in meters. Omit together with --rotation to open the UI.",
    )
    parser.add_argument(
        "--rotation",
        nargs=3,
        type=_finite_float,
        metavar=("X", "Y", "Z"),
        help="Blender XYZ Euler Rotation values in degrees. Omit together with --location to open the UI.",
    )
    parser.add_argument(
        "--mesh",
        default="mesh",
        help="C mesh expression used in generated calls (default: mesh).",
    )
    parser.add_argument(
        "--no-ui",
        action="store_true",
        help="Run in command-line mode only.",
    )
    return parser.parse_args(argv)


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv)
    if args.location is None and args.rotation is None and not args.no_ui:
        return run_gui(initial_mesh=args.mesh)

    if args.location is None or args.rotation is None:
        print(
            "Both --location and --rotation are required in command-line mode.",
            file=sys.stderr,
        )
        return 2

    try:
        print(
            generate_c_snippet(
                mesh=args.mesh,
                location=tuple(args.location),
                rotation=tuple(args.rotation),
            )
        )
    except ValueError as exc:
        print(str(exc), file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
