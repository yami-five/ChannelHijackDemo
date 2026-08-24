#include "scene_0_begin.h"
#include "../demo_context.h"
#include "../demo_scene.h"
#include "animation.h"
#include "fpa.h"
#include "storage/gfx_indices.h"
#include "storage/sprites._indices.h"

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

  assets.tv_off = engine_ctx->storage->get_sprite(SPRITE_OFF);

  for (uint32_t i = 0u; i < CURTAIN_LENGTH; i++) {
    assets.curtain[i] = engine_ctx->storage->get_sprite(SPRITE_FIRANKA);
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

    e3d_Painter_DrawImage(engine_ctx, GFX_TEXTURE_TV);
    e3d_Painter_DrawSprite(engine_ctx, assets.tv_off, 91, 76, 0, 1);
    draw_sprite_wave(engine_ctx, assets.curtain, CURTAIN_LENGTH, frame,
                     &curtain_animation);

    end_scene_frame();
    demo_end_frame();
  }
}

DemoSceneContext *scene_0_begin_get_context(void) { return &scene_ctx; }