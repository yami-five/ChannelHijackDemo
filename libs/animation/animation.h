#ifndef CHANNEL_HIJACK_DEMO_ANIMATION_H
#define CHANNEL_HIJACK_DEMO_ANIMATION_H

#include <stdint.h>

#include "engineApi.h"

typedef struct {
  int16_t origin_x;
  int16_t origin_y;
  int16_t spacing_x;
  int16_t spacing_y;
  int16_t amplitude_x;
  int16_t amplitude_y;
  uint16_t speed;
  uint16_t phase_step;
  int32_t angle;
  uint8_t scale;
} SpriteWaveAnimation;

void draw_sprite_wave(e3d_EngineContext *engine_ctx,
                      const e3d_Sprite *const sprites[], uint16_t sprite_count,
                      uint32_t frame, const SpriteWaveAnimation *animation);
int16_t wave_offset(uint32_t frame, uint16_t speed, uint32_t phase,
                    int16_t amplitude);

#endif
