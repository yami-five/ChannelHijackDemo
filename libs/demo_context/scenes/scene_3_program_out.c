#include "scene_3_program_out.h"
#include "../demo_context.h"
#include "../demo_scene.h"
#include "animation.h"
#include "fpa.h"
#include "storage/gfx_indices.h"
#include "storage/sprites._indices.h"

#define CURTAIN_LENGTH 20u

typedef struct {
  const e3d_Sprite *channel;
  const e3d_Sprite *curtain[CURTAIN_LENGTH];
  const e3d_Sprite *left_hands[2];
  const e3d_Sprite *right_hands[2];
} SceneAssets;

static void run_scene(DemoContext *demo_ctx);

static DemoSceneContext scene_ctx = {
    .scene_frame = 0u,
    .scene_elapsed_ms = 0u,
    .scene_start_time_ms = 0u,
    .run_scene = run_scene,
    .scene_duration = 60u,
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
  if (assets.channel != NULL) {
    return;
  }

  assets.channel = engine_ctx->storage->get_sprite(SPRITE_CHANNEL1);
  for (uint32_t i = 0u; i < CURTAIN_LENGTH; i++) {
    assets.curtain[i] = engine_ctx->storage->get_sprite(SPRITE_FIRANKA);
  }
  assets.left_hands[0] = engine_ctx->storage->get_sprite(SPRITE_LEFT4);
  assets.left_hands[1] = engine_ctx->storage->get_sprite(SPRITE_LEFT5);
  assets.right_hands[0] = engine_ctx->storage->get_sprite(SPRITE_RIGHT4);
  assets.right_hands[1] = engine_ctx->storage->get_sprite(SPRITE_RIGHT5);
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

  e3d_Painter_DrawImage(engine_ctx, GFX_TEXTURE_TV);
  e3d_Painter_DrawSprite(engine_ctx, assets.channel, 91, 76, 0, 1);
  draw_sprite_wave(engine_ctx, assets.curtain, CURTAIN_LENGTH, frame,
                   &curtain_animation);

  if (frame < 5u) {
    e3d_Painter_DrawSprite(
        engine_ctx, assets.right_hands[0],
        (int16_t)(165 + wave_offset(frame, 350u, -TABLE_SIZE / 2u, 2)),
        (int16_t)(90u - frame * 9u), 0, 1);
  } else if (frame < 25u) {
    e3d_Painter_DrawSprite(
        engine_ctx, assets.right_hands[1],
        (int16_t)(166 + wave_offset(frame, 350u, -TABLE_SIZE / 2u, 2)),
        (int16_t)wave_offset(frame, 350u, 0u, 2), 0, 1);
  } else if (frame < 35u) {
    e3d_Painter_DrawSprite(
        engine_ctx, assets.right_hands[0],
        (int16_t)(165 + wave_offset(frame, 350u, -TABLE_SIZE / 2u, 2)),
        (int16_t)(((int32_t)frame - 30) * 9), 0, 1);
  } else {
    e3d_Painter_DrawSprite(
        engine_ctx, assets.right_hands[0],
        (int16_t)(165 + wave_offset(frame, 350u, -TABLE_SIZE / 2u, 2)),
        (int16_t)(90 + wave_offset(frame, 350u, 0u, 2)), 0, 1);
  }

  const uint32_t left_hand_index = (frame > 45u && frame <= 55u) ? 1u : 0u;
  e3d_Painter_DrawSprite(
      engine_ctx, assets.left_hands[left_hand_index],
      (int16_t)(-22 + wave_offset(frame, 350u, TABLE_SIZE / 2u, 2)),
      (int16_t)(147 + wave_offset(frame, 350u, TABLE_SIZE / 2u, 2)), 0, 1);

  end_scene_frame();
  demo_end_frame();
  }
}

DemoSceneContext *scene_3_program_out_get_context(void) { return &scene_ctx; }
