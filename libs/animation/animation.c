#include "animation.h"

#include "fpa.h"

int16_t wave_offset(uint32_t frame, uint16_t speed, uint32_t phase,
                    int16_t amplitude) {
  uint32_t wave_phase =
      (((frame % TABLE_SIZE) * (uint32_t)speed) + phase) % TABLE_SIZE;
  return (int16_t)(((int32_t)fast_sin((int32_t)wave_phase) * amplitude) >> 10);
}

void draw_sprite_wave(e3d_EngineContext *engine_ctx,
                      const e3d_Sprite *const sprites[], uint16_t sprite_count,
                      uint32_t frame, const SpriteWaveAnimation *animation) {
  if (engine_ctx == NULL || sprites == NULL || sprite_count == 0u ||
      animation == NULL) {
    return;
  }

  for (uint16_t i = 0u; i < sprite_count; i++) {
    if (sprites[i] == NULL) {
      continue;
    }

    int16_t amplitude_x =
        sprite_count > 1u ? (int16_t)(((int32_t)i * animation->amplitude_x) /
                                      (sprite_count - 1u))
                          : 0;
    int16_t amplitude_y =
        sprite_count > 1u ? (int16_t)(((int32_t)i * animation->amplitude_y) /
                                      (sprite_count - 1u))
                          : 0;
    uint32_t phase = (uint32_t)i * animation->phase_step;
    int16_t x =
        (int16_t)(animation->origin_x + (int32_t)i * animation->spacing_x +
                  wave_offset(frame, animation->speed, phase, amplitude_x));
    int16_t y =
        (int16_t)(animation->origin_y + (int32_t)i * animation->spacing_y +
                  wave_offset(frame, animation->speed, phase, amplitude_y));

    e3d_Painter_DrawSprite(engine_ctx, sprites[i], x, y, animation->angle,
                           animation->scale);
  }
}
