#include "model_animation.h"

const ModelAnimationValue *
model_animation_get_value(const ModelAnimation *animation,
                          uint32_t value_index) {
  if (animation == NULL || animation->values == NULL ||
      value_index >= animation->values_count) {
    return NULL;
  }

  return &animation->values[value_index];
}

bool model_animation_apply(e3d_EngineContext *engine_ctx, e3d_Mesh *mesh,
                           uint32_t transformation_index,
                           const ModelAnimation *animation,
                           uint32_t value_index) {
  if (engine_ctx == NULL || mesh == NULL) {
    return false;
  }

  const ModelAnimationValue *value =
      model_animation_get_value(animation, value_index);
  if (value == NULL) {
    return false;
  }

  e3d_Mesh_ModifyTransformation(engine_ctx, mesh, value->w, value->x,
                                value->y, value->z, transformation_index);
  return true;
}
