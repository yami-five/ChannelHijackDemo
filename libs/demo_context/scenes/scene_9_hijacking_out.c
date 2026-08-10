#include "../demo_context.h"
#include "../demo_scene.h"
#include "scene_9_hijacking_out.h"
#include "animation.h"
#include "fpa.h"
#include "storage/gfx_indices.h"
#include "storage/sprites._indices.h"

#define CURTAIN_LENGTH 20u
static void run_scene(DemoContext *demo_ctx);
static DemoSceneContext scene_ctx = {
    .scene_frame = 0u,
    .scene_elapsed_ms = 0u,
    .scene_start_time_ms = 0u,
    .scene_duration = 80u,
    .run_scene = run_scene,
};
static const e3d_Sprite *hijacking;
static const e3d_Sprite *curtain[CURTAIN_LENGTH];
static const e3d_Sprite *right_hands[2];
static const SpriteWaveAnimation curtain_animation = {
    -55, 0, 0, 12, 6, 0, 500, 700, 0, 1};
static void load_assets(e3d_EngineContext *engine_ctx) {
  if (hijacking != NULL) return;
  hijacking = engine_ctx->storage->get_sprite(SPRITE_HIJACKING);
  for (uint32_t i = 0u; i < CURTAIN_LENGTH; i++)
    curtain[i] = engine_ctx->storage->get_sprite(SPRITE_FIRANKA);
  right_hands[0] = engine_ctx->storage->get_sprite(SPRITE_RIGHT4);
  right_hands[1] = engine_ctx->storage->get_sprite(SPRITE_RIGHT5);
}
static void finish(void) {
  scene_ctx.scene_frame++;
  scene_ctx.scene_elapsed_ms = demo_platform_time_ms() - scene_ctx.scene_start_time_ms;
}
static void run_scene(DemoContext *demo_ctx) {
  e3d_EngineContext *engine_ctx = demo_ctx->engine_ctx;
  load_assets(engine_ctx);
  while (demo_is_running() &&
         (scene_ctx.scene_duration == 0u ||
          scene_ctx.scene_frame < scene_ctx.scene_duration)) {
  uint32_t f = scene_ctx.scene_frame;
  if (f == 0u) scene_ctx.scene_start_time_ms = demo_platform_time_ms();
  demo_begin_frame(); e3d_Painter_DrawImage(engine_ctx, GFX_TEXTURE_TV);
  e3d_Painter_DrawSprite(engine_ctx, hijacking, 91, 76, 0, 1);
  draw_sprite_wave(engine_ctx, curtain, CURTAIN_LENGTH, f, &curtain_animation);
  int16_t x = (int16_t)(165 + wave_offset(f, 350u, -TABLE_SIZE / 2u, 2));
  if (f >= 10u && f < 20u)
    e3d_Painter_DrawSprite(engine_ctx, right_hands[0], x, (int16_t)(90u - (f - 10u) * 9u), 0, 1);
  else if (f >= 20u && f < 70u)
    e3d_Painter_DrawSprite(engine_ctx, right_hands[1], (int16_t)(x + 10), wave_offset(f, 350u, 0u, 2), 0, 1);
  else if (f >= 70u && f < 80u)
    e3d_Painter_DrawSprite(engine_ctx, right_hands[0], x, (int16_t)((f - 70u) * 9u), 0, 1);
  else
    e3d_Painter_DrawSprite(engine_ctx, right_hands[0], x, (int16_t)(90 + wave_offset(f, 350u, 0u, 2)), 0, 1);
  finish(); demo_end_frame();
  }
}
DemoSceneContext *scene_9_hijacking_out_get_context(void) { return &scene_ctx; }
