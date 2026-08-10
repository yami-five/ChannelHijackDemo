#include "../demo_context.h"
#include "../demo_scene.h"
#include "scene_13_end_in.h"
#include "scene_tv.h"
#include "storage/gfx_indices.h"
#include "storage/sprites._indices.h"

static void run_scene(DemoContext *demo_ctx);
static DemoSceneContext scene_ctx = {
    .scene_frame = 0u,
    .scene_elapsed_ms = 0u,
    .scene_start_time_ms = 0u,
    .scene_duration = 70u,
    .run_scene = run_scene,
};
static const e3d_Sprite *background, *end;
static uint16_t noise_colors[4] = {0xffff, 0xad75, 0x5acb, 0x0000};
static e3d_Rectangle plasma_rect = {28, 44, 182, 242};
static SceneTvFrame tv_frame;
static void load_assets(e3d_EngineContext *engine_ctx) {
  if (background != NULL) return;
  end = engine_ctx->storage->get_sprite(SPRITE_END);
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
  demo_begin_frame(); 
  e3d_Painter_DrawImage(engine_ctx, GFX_TEXTURE_TV_BIG);
  e3d_Painter_DrawSprite(engine_ctx, end, 44, 1, 0, 1);
  scene_tv_frame_draw(&tv_frame, engine_ctx);
  finish(); demo_end_frame();
  }
}
DemoSceneContext *scene_13_end_in_get_context(void) { return &scene_ctx; }
