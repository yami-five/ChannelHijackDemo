#include "../demo_context.h"
#include "../demo_scene.h"
#include "scene_11_alert_out.h"
#include "animation.h"
#include "storage/gfx_indices.h"

#define CURTAIN_LENGTH 20u
static void run_scene(DemoContext *demo_ctx);
static DemoSceneContext scene_ctx = {
    .scene_frame = 0u,
    .scene_elapsed_ms = 0u,
    .scene_start_time_ms = 0u,
    .scene_duration = 60u,
    .run_scene = run_scene,
};
static const e3d_Sprite *alert_small;
static const e3d_Sprite *curtain[CURTAIN_LENGTH];
static const SpriteWaveAnimation curtain_animation = {
    -55, 0, 0, 12, 6, 0, 500, 700, 0, 1};
static void load_assets(e3d_EngineContext *ctx) {
  if (alert_small != NULL) return;
  alert_small = ctx->storage->get_sprite(67u);
  for (uint32_t i = 0u; i < CURTAIN_LENGTH; i++)
    curtain[i] = ctx->storage->get_sprite(37u);
}
static void finish(void) {
  scene_ctx.scene_frame++;
  scene_ctx.scene_elapsed_ms = demo_platform_time_ms() - scene_ctx.scene_start_time_ms;
}
static void run_scene(DemoContext *demo_ctx) {
  e3d_EngineContext *ctx = demo_ctx->engine_ctx;
  load_assets(ctx);
  while (demo_is_running() &&
         (scene_ctx.scene_duration == 0u ||
          scene_ctx.scene_frame < scene_ctx.scene_duration)) {
  uint32_t f = scene_ctx.scene_frame;
  if (f == 0u) scene_ctx.scene_start_time_ms = demo_platform_time_ms();
  demo_begin_frame(); e3d_Painter_DrawImage(ctx, GFX_TEXTURE_TV);
  e3d_Painter_DrawSprite(ctx, alert_small, 91, 76, 0, 1);
  draw_sprite_wave(ctx, curtain, CURTAIN_LENGTH, f, &curtain_animation);
  finish(); demo_end_frame();
  }
}
DemoSceneContext *scene_11_alert_out_get_context(void) { return &scene_ctx; }
