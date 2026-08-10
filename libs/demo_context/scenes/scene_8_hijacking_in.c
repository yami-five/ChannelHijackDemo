#include "../demo_context.h"
#include "../demo_scene.h"
#include "scene_8_hijacking_in.h"
#include "scene_tv.h"
#include "storage/gfx_indices.h"
#include "storage/sprites._indices.h"

static void run_scene(DemoContext *demo_ctx);
static DemoSceneContext scene_ctx = {
    .scene_frame = 0u,
    .scene_elapsed_ms = 0u,
    .scene_start_time_ms = 0u,
    .scene_duration = 115u,
    .run_scene = run_scene,
};
static const e3d_Sprite *digits[10];
static const e3d_Sprite *tv_bug;
static const e3d_Sprite *logo;
static const e3d_Sprite *background;
static const e3d_Sprite *parts[4];
static const e3d_Sprite *big_dark;
static const e3d_Sprite *big;
static const e3d_Sprite *break_image;
static uint16_t noise_colors[4] = {0xffff, 0xad75, 0x5acb, 0x0000};
static e3d_Rectangle plasma_rect = {28, 44, 182, 242};
static SceneTvFrame tv_frame;

static void load_assets(e3d_EngineContext *engine_ctx) {
  if (background != NULL) return;
  for (uint32_t i = 0u; i < 10u; i++)
    digits[i] = engine_ctx->storage->get_sprite((uint8_t)(SPRITE_SEG_0 + i));
  tv_bug = engine_ctx->storage->get_sprite(SPRITE_TV_BUG);
  logo = engine_ctx->storage->get_sprite(SPRITE_LOGO_CHANNEL4);
  background = engine_ctx->storage->get_sprite(SPRITE_HIJACKING_BACKGROUND);
  for (uint32_t i = 0u; i < 4u; i++)
    parts[i] =
        engine_ctx->storage->get_sprite((uint8_t)(SPRITE_HIJACKING_PART1 + i));
  big_dark = engine_ctx->storage->get_sprite(SPRITE_BIG_BUG_DARK);
  big = engine_ctx->storage->get_sprite(SPRITE_BIG_BUG);
  break_image = engine_ctx->storage->get_sprite(SPRITE_PRZERWA);
}
static void finish(void) {
  scene_ctx.scene_frame++;
  scene_ctx.scene_elapsed_ms = demo_platform_time_ms() - scene_ctx.scene_start_time_ms;
}
static void draw_digits(e3d_EngineContext *engine_ctx, uint8_t left, uint8_t right) {
  e3d_Painter_DrawSprite(engine_ctx, digits[left], 56, 36, 0, 2);
  e3d_Painter_DrawSprite(engine_ctx, digits[right], 76, 36, 0, 2);
}
static void run_scene(DemoContext *demo_ctx) {
  e3d_EngineContext *engine_ctx = demo_ctx->engine_ctx;
  load_assets(engine_ctx);
  scene_tv_frame_load(&tv_frame, engine_ctx);
  while (demo_is_running() &&
         (scene_ctx.scene_duration == 0u ||
          scene_ctx.scene_frame < scene_ctx.scene_duration)) {
  uint32_t f = scene_ctx.scene_frame;
  if (f == 0u) scene_ctx.scene_start_time_ms = demo_platform_time_ms();
  demo_begin_frame(); e3d_Painter_DrawImage(engine_ctx, GFX_TEXTURE_TV_BIG);
  if (f <= 15u) draw_digits(engine_ctx, 0u, 4u);
  if (f < 15u) {
    e3d_Painter_DrawSprite(engine_ctx, tv_bug, 44, 1, 0, 1);
    e3d_Painter_DrawSprite(engine_ctx, logo, 254, 38, 0, 1);
  } else if ((f >= 15u && f < 20u) || (f >= 45u && f < 50u) ||
             (f >= 70u && f < 75u) || (f >= 90u && f < 95u)) {
    e3d_Painter_DrawPlasma(engine_ctx, noise_colors, 4u, f, 2, 3, 4, 5, 3, &plasma_rect);
  } else if (f >= 75u && f < 90u) {
    e3d_Painter_DrawSprite(engine_ctx, break_image, 44, 1, 0, 1);
  } else {
    e3d_Painter_DrawSprite(engine_ctx, background, 44, 1, 0, 1);
    if (f >= 35u) e3d_Painter_DrawSprite(engine_ctx, parts[0], 44, 31, 0, 1);
    if (f >= 65u) e3d_Painter_DrawSprite(engine_ctx, parts[3], 152, 105, 0, 1);
    if (f >= 95u) e3d_Painter_DrawSprite(engine_ctx, parts[2], 44, 121, 0, 1);
    if (f >= 125u) e3d_Painter_DrawSprite(engine_ctx, parts[1], 164, 30, 0, 1);
    e3d_Painter_DrawSprite(engine_ctx, f >= 170u ? big : big_dark, 112,
                           f >= 170u ? 53 : 62, 0, 1);
  }
  if (f >= 100u && f < 115u) draw_digits(engine_ctx, 1u, 0u);
  else if (f >= 90u && f < 100u) draw_digits(engine_ctx, 0u, 9u);
  else if (f >= 75u && f < 90u) draw_digits(engine_ctx, 0u, 8u);
  else if (f >= 60u && f < 75u) draw_digits(engine_ctx, 0u, 7u);
  else if (f >= 45u && f < 60u) draw_digits(engine_ctx, 0u, 6u);
  scene_tv_frame_draw(&tv_frame, engine_ctx);
  finish(); demo_end_frame();
  }
}
DemoSceneContext *scene_8_hijacking_in_get_context(void) { return &scene_ctx; }
