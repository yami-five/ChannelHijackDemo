#include "model_animation.h"

#include <stddef.h>

const ModelAnimationValue *
model_animation_get_value(const ModelAnimation *animation,
                          uint32_t value_index) {
  if (animation == NULL || animation->values == NULL ||
      value_index >= animation->values_count) {
    return NULL;
  }

  return &animation->values[value_index];
}
