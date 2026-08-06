from collections.abc import Iterable


bl_info = {
    "name": "Euzebia3D Animation Baker",
    "author": "Euzebia3D",
    "version": (1, 1, 0),
    "blender": (3, 6, 0),
    "location": "3D View > Sidebar > Euzebia3D",
    "description": (
        "Bake evaluated object motion and copy path animation samples for "
        "Euzebia3D"
    ),
    "category": "Animation",
}

Vector3 = tuple[float, float, float]
Quaternion = tuple[float, float, float, float]
TransformSample = tuple[int, Vector3, Quaternion, Vector3]


def frame_numbers(frame_start: int, frame_end: int, frame_step: int) -> range:
    """Return an inclusive animation frame range."""
    if frame_step < 1:
        raise ValueError("Frame step must be at least 1.")
    if frame_end < frame_start:
        raise ValueError("End frame cannot be smaller than start frame.")
    return range(frame_start, frame_end + 1, frame_step)


def _format_float(value: float) -> str:
    if abs(value) < 0.0000000005:
        value = 0.0
    return f"{value:.9g}"


def format_channel_samples(
    samples: Iterable[TransformSample], channel: str
) -> str:
    """Format samples for direct pasting into model_animation_generator.py."""
    channel_index = {"LOCATION": 1, "ROTATION": 2}.get(channel)
    if channel_index is None:
        raise ValueError(f"Unsupported transform channel: {channel}")

    return "\n".join(
        ", ".join(_format_float(value) for value in sample[channel_index])
        for sample in samples
    )


def format_location_rotation_samples(
    samples: Iterable[TransformSample],
) -> str:
    """Format frame, location and quaternion rotation as CSV."""
    lines = [
        "frame,location_x,location_y,location_z,"
        "rotation_w,rotation_x,rotation_y,rotation_z"
    ]
    for frame, location, rotation, _scale in samples:
        values = (*location, *rotation)
        lines.append(
            f"{frame}," + ",".join(_format_float(value) for value in values)
        )
    return "\n".join(lines)


try:
    import bpy
except ImportError:
    bpy = None


if bpy is not None:

    def _animation_fcurves(animation_data):
        """Return F-curves from legacy and Blender 5 layered Actions."""
        action = animation_data.action if animation_data is not None else None
        if action is None:
            return []

        legacy_fcurves = getattr(action, "fcurves", None)
        if legacy_fcurves is not None:
            return list(legacy_fcurves)

        action_slot = getattr(animation_data, "action_slot", None)
        if action_slot is None:
            return []

        fcurves = []
        for layer in action.layers:
            for strip in layer.strips:
                channelbag_getter = getattr(strip, "channelbag", None)
                if not callable(channelbag_getter):
                    continue
                channelbag = channelbag_getter(action_slot)
                if channelbag is not None:
                    fcurves.extend(channelbag.fcurves)
        return fcurves


    def _rotation_keyframe_conflict(obj) -> str | None:
        """Describe keyed frames hidden by the object's rotation mode."""
        active_path = {
            "QUATERNION": "rotation_quaternion",
            "AXIS_ANGLE": "rotation_axis_angle",
        }.get(obj.rotation_mode, "rotation_euler")
        rotation_paths = {
            "rotation_euler",
            "rotation_quaternion",
            "rotation_axis_angle",
        }
        frames_by_path = {path: set() for path in rotation_paths}

        for fcurve in _animation_fcurves(obj.animation_data):
            if fcurve.data_path not in rotation_paths:
                continue
            frames_by_path[fcurve.data_path].update(
                round(keyframe.co.x, 6) for keyframe in fcurve.keyframe_points
            )

        active_frames = frames_by_path[active_path]
        for inactive_path in sorted(rotation_paths - {active_path}):
            hidden_frames = sorted(frames_by_path[inactive_path] - active_frames)
            if not hidden_frames:
                continue
            formatted_frames = ", ".join(f"{frame:g}" for frame in hidden_frames)
            return (
                f"{obj.name}: {inactive_path} has keyed frame(s) "
                f"{formatted_frames}, but Rotation Mode uses {active_path}. "
                "Switch Rotation Mode or move/delete the inactive rotation keys."
            )

        return None

    def _sample_evaluated_world_transforms(
        context,
        obj,
        frame_start: int,
        frame_end: int,
        frame_step: int,
    ) -> list[TransformSample]:
        frames = frame_numbers(frame_start, frame_end, frame_step)
        scene = context.scene
        original_frame = scene.frame_current
        samples: list[TransformSample] = []
        previous_rotation = None

        try:
            for frame in frames:
                scene.frame_set(frame)
                depsgraph = context.evaluated_depsgraph_get()
                evaluated_obj = obj.evaluated_get(depsgraph)
                location, rotation, scale = evaluated_obj.matrix_world.decompose()

                rotation.normalize()
                if (
                    previous_rotation is not None
                    and previous_rotation.dot(rotation) < 0.0
                ):
                    rotation.negate()
                previous_rotation = rotation.copy()

                samples.append(
                    (
                        frame,
                        tuple(location),
                        (rotation.w, rotation.x, rotation.y, rotation.z),
                        tuple(scale),
                    )
                )
        finally:
            scene.frame_set(original_frame)

        return samples


    def _settings_and_range(context):
        scene = context.scene
        settings = scene.euzebia3d_animation_baker
        return settings, scene.frame_start, scene.frame_end


    class EUZEBIA3D_AnimationBakerSettings(bpy.types.PropertyGroup):
        frame_step: bpy.props.IntProperty(
            name="Frame Step",
            description="Sample every Nth frame; the start frame is always included",
            default=1,
            min=1,
            soft_max=10,
        )
        baked_suffix: bpy.props.StringProperty(
            name="Baked Suffix",
            description="Suffix appended to the duplicated baked object",
            default="_Baked",
        )


    class EUZEBIA3D_OT_copy_baked_channel(bpy.types.Operator):
        bl_idname = "euzebia3d.copy_baked_channel"
        bl_label = "Copy Baked Channel"
        bl_description = (
            "Sample the evaluated world transform, including Follow Path and "
            "other constraints, and copy it for Model Animation Generator"
        )
        bl_options = {"REGISTER"}

        channel: bpy.props.EnumProperty(
            name="Channel",
            items=(
                (
                    "LOCATION_ROTATION",
                    "Location + Rotation",
                    "Copy frame, world-space location and quaternion rotation as CSV",
                ),
                ("LOCATION", "Translation", "Copy world-space X, Y, Z"),
                ("ROTATION", "Rotation", "Copy world-space W, X, Y, Z quaternion"),
            ),
            default="LOCATION_ROTATION",
        )

        @classmethod
        def poll(cls, context):
            return context.active_object is not None

        def execute(self, context):
            obj = context.active_object
            settings, frame_start, frame_end = _settings_and_range(context)
            rotation_conflict = _rotation_keyframe_conflict(obj)
            if rotation_conflict is not None:
                self.report({"ERROR"}, rotation_conflict)
                return {"CANCELLED"}
            try:
                samples = _sample_evaluated_world_transforms(
                    context,
                    obj,
                    frame_start,
                    frame_end,
                    settings.frame_step,
                )
                if self.channel == "LOCATION_ROTATION":
                    output = format_location_rotation_samples(samples)
                else:
                    output = format_channel_samples(samples, self.channel)
            except (RuntimeError, ValueError) as exc:
                self.report({"ERROR"}, str(exc))
                return {"CANCELLED"}

            context.window_manager.clipboard = output
            channel_label = {
                "LOCATION_ROTATION": "location and rotation",
                "LOCATION": "translation",
                "ROTATION": "rotation",
            }[self.channel]
            self.report(
                {"INFO"},
                f"Copied {len(samples)} {channel_label} samples",
            )
            return {"FINISHED"}


    class EUZEBIA3D_OT_bake_animation_duplicate(bpy.types.Operator):
        bl_idname = "euzebia3d.bake_animation_duplicate"
        bl_label = "Bake to Duplicate"
        bl_description = (
            "Create an unconstrained duplicate with world-space location, "
            "and quaternion rotation keyframed from the evaluated motion"
        )
        bl_options = {"REGISTER", "UNDO"}

        @classmethod
        def poll(cls, context):
            return context.active_object is not None

        def execute(self, context):
            source = context.active_object
            settings, frame_start, frame_end = _settings_and_range(context)
            rotation_conflict = _rotation_keyframe_conflict(source)
            if rotation_conflict is not None:
                self.report({"ERROR"}, rotation_conflict)
                return {"CANCELLED"}
            try:
                samples = _sample_evaluated_world_transforms(
                    context,
                    source,
                    frame_start,
                    frame_end,
                    settings.frame_step,
                )
            except (RuntimeError, ValueError) as exc:
                self.report({"ERROR"}, str(exc))
                return {"CANCELLED"}

            baked = source.copy()
            baked.name = source.name + settings.baked_suffix
            baked.animation_data_clear()
            baked.constraints.clear()
            baked.parent = None
            baked.rotation_mode = "QUATERNION"
            baked.scale = samples[0][3]
            context.collection.objects.link(baked)

            for frame, location, rotation, _scale in samples:
                baked.location = location
                baked.rotation_quaternion = rotation
                baked.keyframe_insert(
                    data_path="location", frame=frame, group="Transform"
                )
                baked.keyframe_insert(
                    data_path="rotation_quaternion", frame=frame, group="Transform"
                )

            if baked.animation_data and baked.animation_data.action:
                action = baked.animation_data.action
                action.name = baked.name + "Action"
                for fcurve in _animation_fcurves(baked.animation_data):
                    for keyframe in fcurve.keyframe_points:
                        keyframe.interpolation = "LINEAR"

            for selected in context.selected_objects:
                selected.select_set(False)
            baked.select_set(True)
            context.view_layer.objects.active = baked
            context.scene.frame_set(context.scene.frame_current)

            self.report(
                {"INFO"},
                f"Baked {len(samples)} frames to {baked.name}",
            )
            return {"FINISHED"}


    class EUZEBIA3D_PT_animation_baker(bpy.types.Panel):
        bl_label = "Animation Baker"
        bl_idname = "EUZEBIA3D_PT_animation_baker"
        bl_space_type = "VIEW_3D"
        bl_region_type = "UI"
        bl_category = "Euzebia3D"

        def draw(self, context):
            layout = self.layout
            scene = context.scene
            settings = scene.euzebia3d_animation_baker
            obj = context.active_object

            if obj is None:
                layout.label(text="Select an animated object.", icon="INFO")
                return

            layout.label(text=f"Object: {obj.name}")
            range_box = layout.box()
            range_box.label(text="Sampling Range")
            range_box.prop(scene, "frame_start", text="Start")
            range_box.prop(scene, "frame_end", text="End")
            range_box.prop(settings, "frame_step")

            bake_box = layout.box()
            bake_box.label(text="Non-destructive Bake")
            bake_box.prop(settings, "baked_suffix", text="Name suffix")
            bake_box.operator(
                EUZEBIA3D_OT_bake_animation_duplicate.bl_idname,
                icon="ACTION",
            )

            export_box = layout.box()
            export_box.label(text="Export Evaluated Motion")
            operator = export_box.operator(
                EUZEBIA3D_OT_copy_baked_channel.bl_idname,
                text="Copy Location + Rotation (CSV)",
                icon="COPYDOWN",
            )
            operator.channel = "LOCATION_ROTATION"
            export_box.separator()
            export_box.label(text="For Model Animation Generator:")
            operator = export_box.operator(
                EUZEBIA3D_OT_copy_baked_channel.bl_idname,
                text="Copy Translation (X, Y, Z)",
                icon="COPYDOWN",
            )
            operator.channel = "LOCATION"
            operator = export_box.operator(
                EUZEBIA3D_OT_copy_baked_channel.bl_idname,
                text="Copy Rotation (W, X, Y, Z)",
                icon="COPYDOWN",
            )
            operator.channel = "ROTATION"

            sample_count = len(
                frame_numbers(scene.frame_start, scene.frame_end, settings.frame_step)
            )
            layout.label(text=f"Output: {sample_count} samples (world space)")


    BLENDER_CLASSES = (
        EUZEBIA3D_AnimationBakerSettings,
        EUZEBIA3D_OT_copy_baked_channel,
        EUZEBIA3D_OT_bake_animation_duplicate,
        EUZEBIA3D_PT_animation_baker,
    )
else:
    BLENDER_CLASSES = ()


def register() -> None:
    if bpy is None:
        raise RuntimeError("This add-on must be registered from Blender.")

    for blender_class in BLENDER_CLASSES:
        bpy.utils.register_class(blender_class)
    bpy.types.Scene.euzebia3d_animation_baker = bpy.props.PointerProperty(
        type=EUZEBIA3D_AnimationBakerSettings
    )


def unregister() -> None:
    if bpy is None:
        return

    del bpy.types.Scene.euzebia3d_animation_baker
    for blender_class in reversed(BLENDER_CLASSES):
        bpy.utils.unregister_class(blender_class)


if __name__ == "__main__":
    register()
