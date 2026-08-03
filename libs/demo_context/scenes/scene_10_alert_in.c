#include "../demo_context.h"
#include "../demo_scene.h"
#include "scene_10_alert_in.h"
#include "scene_tv.h"
#include "storage/gfx_indices.h"

static void run_scene(DemoContext *demo_ctx);
static DemoSceneContext scene_ctx = {
    .scene_frame = 0u,
    .scene_elapsed_ms = 0u,
    .scene_start_time_ms = 0u,
    .scene_duration = 100u,
    .run_scene = run_scene,
};
static const e3d_Sprite *background, *parts[4], *big_dark, *big;
static const e3d_Sprite *alert, *alert_alternate;
static uint16_t noise_colors[4] = {0xffff, 0xad75, 0x5acb, 0x0000};
static e3d_Rectangle plasma_rect = {28, 44, 182, 242};
static SceneTvFrame tv_frame;
static void load_assets(e3d_EngineContext *ctx) {
  if (background != NULL) return;
  background = ctx->storage->get_sprite(56u);
  for (uint32_t i = 0u; i < 4u; i++) parts[i] = ctx->storage->get_sprite((uint8_t)(57u + i));
  big_dark = ctx->storage->get_sprite(61u); big = ctx->storage->get_sprite(62u);
  alert = ctx->storage->get_sprite(65u); alert_alternate = ctx->storage->get_sprite(66u);
}
static void finish(void) {
  scene_ctx.scene_frame++;
  scene_ctx.scene_elapsed_ms = demo_platform_time_ms() - scene_ctx.scene_start_time_ms;
}
static void run_scene(DemoContext *demo_ctx) {
  e3d_EngineContext *ctx = demo_ctx->engine_ctx;
  load_assets(ctx);
  scene_tv_frame_load(&tv_frame, ctx);
  while (demo_is_running() &&
         (scene_ctx.scene_duration == 0u ||
          scene_ctx.scene_frame < scene_ctx.scene_duration)) {
  uint32_t f = scene_ctx.scene_frame;
  if (f == 0u) scene_ctx.scene_start_time_ms = demo_platform_time_ms();
  demo_begin_frame(); e3d_Painter_DrawImage(ctx, GFX_TEXTURE_TV_BIG);
  e3d_Painter_DrawSprite(ctx, background, 44, 1, 0, 1);
  e3d_Painter_DrawSprite(ctx, parts[0], 44, 31, 0, 1);
  e3d_Painter_DrawSprite(ctx, parts[3], 152, 105, 0, 1);
  e3d_Painter_DrawSprite(ctx, parts[2], 44, 121, 0, 1);
  e3d_Painter_DrawSprite(ctx, parts[1], 164, 30, 0, 1);
  e3d_Painter_DrawSprite(ctx, f >= 30u ? big : big_dark, 112, f >= 30u ? 53 : 62, 0, 1);
  if ((f >= 45u && f < 50u) || (f >= 80u && f < 85u))
    e3d_Painter_DrawPlasma(ctx, noise_colors, 4u, f, 2, 3, 4, 5, 3, &plasma_rect);
  else if (f >= 50u && f < 80u)
    e3d_Painter_DrawSprite(ctx, alert, 44, 1, 0, 1);
  else if (f >= 85u)
    e3d_Painter_DrawSprite(ctx, alert_alternate, 44, 1, 0, 1);
  scene_tv_frame_draw(&tv_frame, ctx);
  finish(); demo_end_frame();
  }
}
DemoSceneContext *scene_10_alert_in_get_context(void) { return &scene_ctx; }
