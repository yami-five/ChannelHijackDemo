#ifndef CHANNEL_HIJACK_DEMO_MODEL_ANIMATION_H
#define CHANNEL_HIJACK_DEMO_MODEL_ANIMATION_H

#include <stdint.h>

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

#endif
