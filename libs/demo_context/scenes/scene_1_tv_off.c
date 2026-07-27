#include "scene_1_tv_off.h"
#include "../demo_context.h"
#include "../demo_scene.h"
#include "animation.h"
#include "fpa.h"

#define CURTAIN_LENGTH 20u

typedef struct {
  const e3d_Sprite *tv_off;
  const e3d_Sprite *curtain[CURTAIN_LENGTH];
  const e3d_Sprite *left_hands[7];
  const e3d_Sprite *right_hands[5];
} SceneAssets;

static void run_scene(DemoContext *demo_ctx);

static DemoSceneContext scene_ctx = {
    .scene_elapsed_ms = 0,
    .scene_start_time_ms = 0,
    .scene_frame = 0,
    .run_scene = run_scene,
    .scene_duration = 120u,
};

static SceneAssets assets;

static const SpriteWaveAnimation curtain_animation = {
    .origin_x = -55,
    .origin_y = 0,
    .spacing_x = 0,
    .spacing_y = 12,
    .amplitude_x = 6,
    .amplitude_y = 0,
    .speed = 500,
    .phase_step = 700,
    .angle = 0,
    .scale = 1,
};

static void load_assets(e3d_EngineContext *engine_ctx) {
  if (assets.tv_off != NULL) {
    return;
  }

  assets.tv_off = engine_ctx->storage->get_sprite(54u);

  for (uint32_t i = 0u; i < CURTAIN_LENGTH; i++) {
    assets.curtain[i] = engine_ctx->storage->get_sprite(37u);
  }
  for (uint32_t i = 0u; i < 7u; i++) {
    assets.left_hands[i] = engine_ctx->storage->get_sprite((uint8_t)(38u + i));
  }
  for (uint32_t i = 0u; i < 5u; i++) {
    assets.right_hands[i] = engine_ctx->storage->get_sprite((uint8_t)(45u + i));
  }
}

static void end_scene_frame(void) {
  scene_ctx.scene_frame++;
  scene_ctx.scene_elapsed_ms =
      demo_platform_time_ms() - scene_ctx.scene_start_time_ms;
}

static void run_scene(DemoContext *demo_ctx) {
  e3d_EngineContext *engine_ctx = demo_ctx->engine_ctx;
  load_assets(engine_ctx);
  while (demo_is_running() &&
         (scene_ctx.scene_duration == 0u ||
          scene_ctx.scene_frame < scene_ctx.scene_duration)) {
    const uint32_t frame = scene_ctx.scene_frame;

  if (frame == 0u) {
    scene_ctx.scene_start_time_ms = demo_platform_time_ms();
  }

  demo_begin_frame();

  e3d_Painter_DrawImage(engine_ctx, 1u);
  e3d_Painter_DrawSprite(engine_ctx, assets.tv_off, 91, 76, 0, 1);
  draw_sprite_wave(engine_ctx, assets.curtain, CURTAIN_LENGTH, frame,
                   &curtain_animation);

  if (frame < 20u) {
    e3d_Painter_DrawSprite(engine_ctx, assets.left_hands[0], 0,
                           (int16_t)(244u - frame * 5u), 0, 1);
    e3d_Painter_DrawSprite(engine_ctx, assets.right_hands[0], 168,
                           (int16_t)(204u - frame * 7u), 0, 1);
  } else if (frame < 30u) {
    e3d_Painter_DrawSprite(engine_ctx, assets.left_hands[1], -1, 78, 0, 1);
    e3d_Painter_DrawSprite(engine_ctx, assets.right_hands[1], 160, 112, 0, 1);
  } else if (frame < 40u) {
    e3d_Painter_DrawSprite(engine_ctx, assets.left_hands[2], 0, 78, 0, 1);
    e3d_Painter_DrawSprite(engine_ctx, assets.right_hands[2], 167, 89, 0, 1);
  } else if (frame < 50u) {
    e3d_Painter_DrawSprite(engine_ctx, assets.left_hands[0], 0,
                           (int16_t)(135u + (frame - 40u) * 10u), 0, 1);
    e3d_Painter_DrawSprite(
        engine_ctx, assets.right_hands[3],
        (int16_t)(165 + wave_offset(frame, 350u, -TABLE_SIZE / 2u, 2)),
        (int16_t)(90 + wave_offset(frame, 350u, 0u, 2)), 0, 1);
  } else if (frame < 65u) {
    e3d_Painter_DrawSprite(engine_ctx, assets.left_hands[5], -17,
                           (int16_t)(242u - (frame - 50u) * 7u), 0, 1);
    e3d_Painter_DrawSprite(
        engine_ctx, assets.right_hands[3],
        (int16_t)(165 + wave_offset(frame, 350u, -TABLE_SIZE / 2u, 2)),
        (int16_t)(90 + wave_offset(frame, 350u, 0u, 2)), 0, 1);
  } else {
    const uint32_t left_hand_index = (frame > 105u && frame <= 115u) ? 6u : 5u;
    e3d_Painter_DrawSprite(
        engine_ctx, assets.left_hands[left_hand_index],
        (int16_t)(-22 + wave_offset(frame, 350u, TABLE_SIZE / 2u, 2)),
        (int16_t)(147 + wave_offset(frame, 350u, TABLE_SIZE / 2u, 2)), 0, 1);

    if (frame >= 80u && frame < 90u) {
      e3d_Painter_DrawSprite(
          engine_ctx, assets.right_hands[3],
          (int16_t)(165 + wave_offset(frame, 350u, -TABLE_SIZE / 2u, 2)),
          (int16_t)(90u - (frame - 80u) * 9u), 0, 1);
    } else if (frame >= 90u && frame < 110u) {
      e3d_Painter_DrawSprite(
          engine_ctx, assets.right_hands[4],
          (int16_t)(175 + wave_offset(frame, 350u, -TABLE_SIZE / 2u, 2)),
          (int16_t)wave_offset(frame, 350u, 0u, 2), 0, 1);
    } else if (frame >= 110u) {
      e3d_Painter_DrawSprite(
          engine_ctx, assets.right_hands[3],
          (int16_t)(165 + wave_offset(frame, 350u, -TABLE_SIZE / 2u, 2)),
          (int16_t)((frame - 110u) * 9u), 0, 1);
    } else {
      e3d_Painter_DrawSprite(
          engine_ctx, assets.right_hands[3],
          (int16_t)(165 + wave_offset(frame, 350u, -TABLE_SIZE / 2u, 2)),
          (int16_t)(90 + wave_offset(frame, 350u, 0u, 2)), 0, 1);
    }
  }

  end_scene_frame();
  demo_end_frame();
  }
}

DemoSceneContext *scene_1_tv_off_get_context(void) { return &scene_ctx; }
