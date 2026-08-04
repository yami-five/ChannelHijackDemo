from __future__ import annotations

import argparse
import math
import re
import sys
from collections.abc import Sequence


bl_info = {
    "name": "Euzebia3D Transform Copier",
    "author": "Euzebia3D",
    "version": (1, 3, 0),
    "blender": (3, 6, 0),
    "location": "3D View > Sidebar > Euzebia3D",
    "description": "Copy the selected object's transform as Euzebia3D C code",
    "category": "3D View",
}

Vector3 = tuple[float, float, float]
Quaternion = tuple[float, float, float, float]

# Euzebia3D v0.11.4-v0.11.5 pass Q12 radians directly to a 36000-entry trig
# table instead of converting them with radian_to_index(). Compensate for that
# scale mismatch until the pinned engine version is upgraded with a renderer fix.
ENGINE_ROTATION_ANGLE_SCALE = 5729.0 / 4096.0


def blender_location_to_engine(location: Vector3) -> Vector3:
    """Convert Blender's (X, Y, Z-up) position to engine (X, Y-up, Z)."""
    x, y, z = location
    return x, z, -y


def blender_scale_to_engine(
    scale: Vector3, geometry_in_engine_axes: bool = False
) -> Vector3:
    """Return scale for raw Blender-axis or already converted geometry."""
    x, y, z = scale
    return (x, z, y) if geometry_in_engine_axes else (x, y, z)


def _quaternion_multiply(left: Quaternion, right: Quaternion) -> Quaternion:
    lw, lx, ly, lz = left
    rw, rx, ry, rz = right
    return (
        lw * rw - lx * rx - ly * ry - lz * rz,
        lw * rx + lx * rw + ly * rz - lz * ry,
        lw * ry - lx * rz + ly * rw + lz * rx,
        lw * rz + lx * ry - ly * rx + lz * rw,
    )


def blender_quaternion_to_engine_axis_angle(
    rotation: Quaternion,
    geometry_in_engine_axes: bool = False,
) -> tuple[float, Vector3]:
    """Convert Blender quaternion to engine angle-in-turns and rotation axis."""
    w, x, y, z = rotation
    length = math.sqrt(w * w + x * x + y * y + z * z)
    if length < 1e-12:
        raise ValueError("Rotation quaternion cannot have zero length.")

    blender_rotation = (w / length, x / length, y / length, z / length)

    half_sqrt_two = math.sqrt(0.5)
    basis_rotation = (half_sqrt_two, -half_sqrt_two, 0.0, 0.0)
    engine_rotation = _quaternion_multiply(basis_rotation, blender_rotation)
    if geometry_in_engine_axes:
        basis_rotation_inverse = (half_sqrt_two, half_sqrt_two, 0.0, 0.0)
        engine_rotation = _quaternion_multiply(
            engine_rotation, basis_rotation_inverse
        )

    # Euzebia3D's reversed quaternion product applies the inverse rotation.
    # Conjugating here makes the result match Blender's active rotation.
    w, x, y, z = (
        engine_rotation[0],
        -engine_rotation[1],
        -engine_rotation[2],
        -engine_rotation[3],
    )

    # q and -q represent the same rotation. A positive w selects the equivalent
    # representation with an angle no greater than half a turn.
    if w < 0.0:
        w, x, y, z = -w, -x, -y, -z

    axis_length = math.sqrt(x * x + y * y + z * z)
    if axis_length < 1e-12:
        return 0.0, (1.0, 0.0, 0.0)

    angle_turns = (2.0 * math.acos(max(-1.0, min(1.0, w)))) / math.tau
    axis = (
        x / axis_length,
        y / axis_length,
        z / axis_length,
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


def generate_c_snippet(
    mesh: str,
    location: Vector3,
    rotation: Quaternion,
    scale: Vector3 | None = None,
    geometry_in_engine_axes: bool = False,
) -> str:
    mesh = mesh.strip()
    if not mesh:
        raise ValueError("Mesh expression cannot be empty.")

    translation = blender_location_to_engine(location)
    angle_turns, axis = blender_quaternion_to_engine_axis_angle(
        rotation,
        geometry_in_engine_axes=geometry_in_engine_axes,
    )
    engine_angle = angle_turns * ENGINE_ROTATION_ANGLE_SCALE
    angle, axis_x, axis_y, axis_z = map(_format_c_float, (engine_angle, *axis))
    translate_x, translate_y, translate_z = map(_format_c_float, translation)
    lines = []
    if scale is not None:
        scale_x, scale_y, scale_z = map(
            _format_c_float,
            blender_scale_to_engine(scale, geometry_in_engine_axes),
        )
        lines.extend(
            (
                f"e3d_Mesh_AddTransformation(engine_ctx, {mesh}, 0.0f,",
                f"                        {scale_x}, {scale_y}, {scale_z},",
                "                        MODEL_TRANSFORM_SCALE);",
            )
        )

    lines.extend(
        (
            f"e3d_Mesh_AddTransformation(engine_ctx, {mesh}, {angle},",
            f"                        {axis_x}, {axis_y}, {axis_z},",
            "                        MODEL_TRANSFORM_ROTATE);",
            f"e3d_Mesh_AddTransformation(engine_ctx, {mesh}, 0.0f,",
            f"                        {translate_x}, {translate_y}, {translate_z},",
            "                        MODEL_TRANSFORM_TRANSLATE);",
        )
    )
    return "\n".join(lines)


def run_gui(
    initial_location: Vector3 = (0.0, 0.0, 0.0),
    initial_rotation: Quaternion = (1.0, 0.0, 0.0, 0.0),
    initial_mesh: str = "mesh",
) -> int:
    try:
        from PySide6.QtWidgets import (
            QApplication,
            QCheckBox,
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
                initial_rotation, "", "WXYZ"
            )
            form.addRow("Blender Rotation (Quaternion):", rotation_layout)

            self.mesh_input = QLineEdit(initial_mesh)
            self.mesh_input.setPlaceholderText("For example: assets.maggot1")
            form.addRow("C mesh expression:", self.mesh_input)

            self.geometry_axes_checkbox = QCheckBox(
                "OBJ vertices are already converted to engine axes"
            )
            self.geometry_axes_checkbox.setChecked(False)
            layout.addWidget(self.geometry_axes_checkbox)

            mapping_label = QLabel(
                "Mapping: position (X, Y, Z) → (X, Z, -Y); "
                "Blender quaternion → corrected v0.11.x angle + axis"
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
            initial_values: Sequence[float], suffix: str, labels: str = "XYZ"
        ) -> tuple[tuple[QDoubleSpinBox, ...], QHBoxLayout]:
            row = QHBoxLayout()
            inputs = []
            for axis, value in zip(labels, initial_values):
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
                    geometry_in_engine_axes=self.geometry_axes_checkbox.isChecked(),
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


def _object_name_to_c_identifier(name: str) -> str:
    identifier = re.sub(r"[^0-9A-Za-z_]", "_", name.strip()) or "mesh"
    if identifier[0].isdigit():
        identifier = f"_{identifier}"
    return identifier


try:
    import bpy
except ImportError:
    bpy = None


if bpy is not None:

    class EUZEBIA3D_OT_copy_transform(bpy.types.Operator):
        bl_idname = "euzebia3d.copy_transform"
        bl_label = "Copy Transform as C Code"
        bl_description = (
            "Convert the active object's local transform and copy Euzebia3D "
            "C calls to the system clipboard"
        )
        bl_options = {"REGISTER"}

        @classmethod
        def poll(cls, context):
            return context.active_object is not None

        def execute(self, context):
            obj = context.active_object
            location, rotation, scale = obj.matrix_basis.decompose()
            mesh_expression = obj.euzebia3d_mesh_expression.strip()
            if not mesh_expression:
                mesh_expression = f"assets.{_object_name_to_c_identifier(obj.name)}"

            try:
                snippet = generate_c_snippet(
                    mesh=mesh_expression,
                    location=tuple(location),
                    rotation=(rotation.w, rotation.x, rotation.y, rotation.z),
                    scale=tuple(scale) if obj.euzebia3d_include_scale else None,
                    geometry_in_engine_axes=(
                        obj.euzebia3d_geometry_axes == "ENGINE"
                    ),
                )
            except ValueError as exc:
                self.report({"ERROR"}, str(exc))
                return {"CANCELLED"}

            context.window_manager.clipboard = snippet
            self.report({"INFO"}, f"Copied transform for {obj.name}")
            return {"FINISHED"}


    class EUZEBIA3D_PT_transform_copier(bpy.types.Panel):
        bl_label = "C Transform Copier"
        bl_idname = "EUZEBIA3D_PT_transform_copier"
        bl_space_type = "VIEW_3D"
        bl_region_type = "UI"
        bl_category = "Euzebia3D"

        def draw(self, context):
            layout = self.layout
            obj = context.active_object
            if obj is None:
                layout.label(text="Select an object first.", icon="INFO")
                return

            layout.label(text=f"Object: {obj.name}")
            layout.prop(obj, "euzebia3d_mesh_expression", text="C mesh")
            if not obj.euzebia3d_mesh_expression.strip():
                default_mesh = f"assets.{_object_name_to_c_identifier(obj.name)}"
                layout.label(text=f"Default: {default_mesh}")
            layout.prop(obj, "euzebia3d_include_scale", text="Include scale")
            layout.prop(obj, "euzebia3d_geometry_axes", text="OBJ axes")
            layout.operator(
                EUZEBIA3D_OT_copy_transform.bl_idname,
                icon="COPYDOWN",
            )


    BLENDER_CLASSES = (
        EUZEBIA3D_OT_copy_transform,
        EUZEBIA3D_PT_transform_copier,
    )
else:
    BLENDER_CLASSES = ()


def register() -> None:
    if bpy is None:
        raise RuntimeError("This add-on must be registered from Blender.")

    bpy.types.Object.euzebia3d_mesh_expression = bpy.props.StringProperty(
        name="C mesh expression",
        description="Expression passed as the mesh argument; empty uses assets.<object name>",
        default="",
    )
    bpy.types.Object.euzebia3d_include_scale = bpy.props.BoolProperty(
        name="Include scale",
        description="Add scale if it was not already baked into the OBJ vertices",
        default=False,
    )
    bpy.types.Object.euzebia3d_geometry_axes = bpy.props.EnumProperty(
        name="OBJ axes",
        description="Coordinate system used by vertices in the exported OBJ",
        items=(
            (
                "BLENDER",
                "Blender (Z up)",
                "OBJ retains Blender axes; matches this project's maggot.obj",
            ),
            (
                "ENGINE",
                "Engine (Y up)",
                "OBJ vertices were already converted to engine axes",
            ),
        ),
        default="BLENDER",
    )
    for blender_class in BLENDER_CLASSES:
        bpy.utils.register_class(blender_class)


def unregister() -> None:
    if bpy is None:
        return

    for blender_class in reversed(BLENDER_CLASSES):
        bpy.utils.unregister_class(blender_class)
    del bpy.types.Object.euzebia3d_geometry_axes
    del bpy.types.Object.euzebia3d_include_scale
    del bpy.types.Object.euzebia3d_mesh_expression


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Convert a Blender transform with a WXYZ quaternion to Euzebia3D "
            "angle-axis rotation and translation calls."
        ),
        epilog=(
            "Example: python tools/blender_transform_converter.py "
            "--location -3.5142 4.173 0.12 "
            "--rotation 0.92388 0.382683 0 0 --mesh assets.maggot1"
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
        nargs=4,
        type=_finite_float,
        metavar=("W", "X", "Y", "Z"),
        help=(
            "Blender Rotation quaternion in W X Y Z order. "
            "Omit together with --location to open the UI."
        ),
    )
    parser.add_argument(
        "--mesh",
        default="mesh",
        help="C mesh expression used in generated calls (default: mesh).",
    )
    parser.add_argument(
        "--geometry-engine-axes",
        action="store_true",
        help=(
            "Use when OBJ vertices were already converted from Blender Z-up "
            "to engine Y-up axes. Default assumes raw Blender axes."
        ),
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
                geometry_in_engine_axes=args.geometry_engine_axes,
            )
        )
    except ValueError as exc:
        print(str(exc), file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
