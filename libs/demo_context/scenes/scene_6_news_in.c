#include "../demo_context.h"
#include "../demo_scene.h"
#include "scene_6_news_in.h"
#include "scene_tv.h"
#include "storage/gfx_indices.h"
#include "storage/sprites._indices.h"

typedef struct {
  const e3d_Sprite *agro_main;
  const e3d_Sprite *burak;
  const e3d_Sprite *mouth[4];
  const e3d_Sprite *segment_zero;
  const e3d_Sprite *segment_three;
  const e3d_Sprite *logo;
} SceneAssets;

static void run_scene(DemoContext *demo_ctx);
static DemoSceneContext scene_ctx = {
    .scene_frame = 0u,
    .scene_elapsed_ms = 0u,
    .scene_start_time_ms = 0u,
    .scene_duration = 60u,
    .run_scene = run_scene,
};
static SceneAssets assets;
static SceneTvFrame tv_frame;
static uint16_t plasma_colors[16] = {
    0x1be6, 0x2427, 0x3447, 0x4488, 0x54c8, 0x5d09, 0x6d49, 0x7d8a,
    0x7d8a, 0x6d49, 0x5d09, 0x54c8, 0x4488, 0x3447, 0x2427, 0x1be6};
static uint16_t noise_colors[4] = {0xffff, 0xad75, 0x5acb, 0x0000};
static e3d_Rectangle plasma_rect = {28, 44, 182, 242};
static e3d_Rectangle news_bar = {15, 38, 20, 280};
static e3d_Rectangle news_bar_top = {15, 28, 10, 280};
static e3d_Rectangle program_rect = {44, 28, 182, 242};
static const char news_text[] =
    "PILNE: Ministerstwo Grabi i Widel uspokaja, ze tegoroczny wysyp "
    "kabaczkuf nie zagraza bespieczenstwu panstwa... Rolnicy z gminy Dolne "
    "Pole donoszo, ze kombajn pana Zdzislawa sam odmuwil pracy i zazondal "
    "urlopu pod gruszom... Ceny marchwi so stabilne, ale eksperty ostrzegajo "
    "przed panikom wsrod krolikuf...";

static void load_assets(e3d_EngineContext *engine_ctx) {
  if (assets.agro_main != NULL) return;
  assets.agro_main = engine_ctx->storage->get_sprite(SPRITE_AGRO);
  assets.burak = engine_ctx->storage->get_sprite(SPRITE_BURAK);
  assets.mouth[0] = engine_ctx->storage->get_sprite(SPRITE_AGRO1);
  assets.mouth[1] = engine_ctx->storage->get_sprite(SPRITE_AGRO2);
  assets.mouth[2] = engine_ctx->storage->get_sprite(SPRITE_AGRO3);
  assets.mouth[3] = engine_ctx->storage->get_sprite(SPRITE_AGRO2);
  assets.segment_zero = engine_ctx->storage->get_sprite(SPRITE_SEG_0);
  assets.segment_three = engine_ctx->storage->get_sprite(SPRITE_SEG_3);
  assets.logo = engine_ctx->storage->get_sprite(SPRITE_LOGO_CHANNEL3);
}
static void finish(void) {
  scene_ctx.scene_frame++;
  scene_ctx.scene_elapsed_ms = demo_platform_time_ms() - scene_ctx.scene_start_time_ms;
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
  if ((f >= 5u && f < 10u) || (f >= 30u && f < 35u) ||
      (f >= 45u && f < 50u)) {
    e3d_Painter_DrawPlasma(engine_ctx, noise_colors, 4u, f, 2, 3, 4, 5, 3, &plasma_rect);
  } else if (f >= 35u && f < 45u) {
    e3d_Painter_DrawRectangle(engine_ctx, &program_rect, 0x0000);
  } else {
    e3d_Painter_DrawPlasma(engine_ctx, plasma_colors, 16u, f, 2, 6, 6, 7, 6, &plasma_rect);
    e3d_Painter_DrawSprite(engine_ctx, assets.agro_main, 130, 44, 0, 1);
    e3d_Painter_DrawSprite(engine_ctx, assets.burak, 54, 38, 0, 1);
    e3d_Painter_DrawRectangle(engine_ctx, &news_bar, 0x34b2);
    e3d_Painter_DrawRectangle(engine_ctx, &news_bar_top, 0x3292);
    e3d_Painter_Print(engine_ctx, news_text, (int16_t)(-(int32_t)(f * 3u)), 203, 1, 0xffff);
    e3d_Painter_Print(engine_ctx, "Ceny burakuf rosno!!1", 75, 185, 0, 0xffff);
    e3d_Painter_DrawSprite(engine_ctx, assets.mouth[f % 4u], 208, 79, 0, 1);
    if (f <= 15u) {
      e3d_Painter_DrawSprite(engine_ctx, assets.segment_zero, 56, 36, 0, 2);
      e3d_Painter_DrawSprite(engine_ctx, assets.segment_three, 76, 36, 0, 2);
    }
    e3d_Painter_DrawSprite(engine_ctx, assets.logo, 254, 38, 0, 1);
  }
  scene_tv_frame_draw(&tv_frame, engine_ctx);
  finish(); demo_end_frame();
  }
}
DemoSceneContext *scene_6_news_in_get_context(void) { return &scene_ctx; }
