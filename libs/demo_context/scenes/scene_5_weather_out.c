#include "../demo_context.h"
#include "../demo_scene.h"
#include "scene_5_weather_out.h"
#include "animation.h"
#include "fpa.h"
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
static const e3d_Sprite *channel;
static const e3d_Sprite *curtain[CURTAIN_LENGTH];
static const e3d_Sprite *left_hands[2];
static const e3d_Sprite *right_hands[2];
static const SpriteWaveAnimation curtain_animation = {
    -55, 0, 0, 12, 6, 0, 500, 700, 0, 1};

static void load_assets(e3d_EngineContext *ctx) {
  if (channel != NULL) return;
  channel = ctx->storage->get_sprite(35u);
  for (uint32_t i = 0; i < CURTAIN_LENGTH; i++)
    curtain[i] = ctx->storage->get_sprite(37u);
  left_hands[0] = ctx->storage->get_sprite(41u);
  left_hands[1] = ctx->storage->get_sprite(42u);
  right_hands[0] = ctx->storage->get_sprite(48u);
  right_hands[1] = ctx->storage->get_sprite(49u);
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
  demo_begin_frame();
  e3d_Painter_DrawImage(ctx, GFX_TEXTURE_TV);
  e3d_Painter_DrawSprite(ctx, channel, 91, 76, 0, 1);
  draw_sprite_wave(ctx, curtain, CURTAIN_LENGTH, f, &curtain_animation);
  int16_t rx = (int16_t)(165 + wave_offset(f, 350u, -TABLE_SIZE / 2u, 2));
  if (f < 10u)
    e3d_Painter_DrawSprite(ctx, right_hands[0], rx, (int16_t)(90u - f * 9u), 0, 1);
  else if (f < 30u)
    e3d_Painter_DrawSprite(ctx, right_hands[1], (int16_t)(rx + 1),
                           wave_offset(f, 350u, 0u, 2), 0, 1);
  else if (f < 40u)
    e3d_Painter_DrawSprite(ctx, right_hands[0], rx, (int16_t)((f - 30u) * 9u), 0, 1);
  else
    e3d_Painter_DrawSprite(ctx, right_hands[0], rx,
                           (int16_t)(90 + wave_offset(f, 350u, 0u, 2)), 0, 1);
  uint32_t li = (f > 45u && f <= 55u) ? 1u : 0u;
  e3d_Painter_DrawSprite(ctx, left_hands[li],
      (int16_t)(-22 + wave_offset(f, 350u, TABLE_SIZE / 2u, 2)),
      (int16_t)(147 + wave_offset(f, 350u, TABLE_SIZE / 2u, 2)), 0, 1);
  finish(); demo_end_frame();
  }
}
DemoSceneContext *scene_5_weather_out_get_context(void) { return &scene_ctx; }
