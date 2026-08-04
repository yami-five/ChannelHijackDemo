#ifndef CHANNEL_HIJACK_DEMO_MODEL_ANIMATION_H
#define CHANNEL_HIJACK_DEMO_MODEL_ANIMATION_H

#include <stdbool.h>
#include <stdint.h>

#include "engineApi.h"

typedef struct {
  float w;
  float x;
  float y;
  float z;
} ModelAnimationValue;

typedef struct {
  const ModelAnimationValue *values;
  uint32_t values_count;
} ModelAnimation;

const ModelAnimationValue *
model_animation_get_value(const ModelAnimation *animation,
                          uint32_t value_index);

bool model_animation_apply(e3d_EngineContext *engine_ctx, e3d_Mesh *mesh,
                           uint32_t transformation_index,
                           const ModelAnimation *animation,
                           uint32_t value_index);

#endif
