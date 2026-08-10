#include "scene_2_program_in.h"
#include "../demo_context.h"
#include "../demo_scene.h"
#include "grid.h"
#include "scene_tv.h"
#include "storage/gfx_indices.h"
#include "storage/sprites._indices.h"

typedef struct {
  const e3d_Sprite *segment_zero;
  const e3d_Sprite *segment_one;
  const e3d_Sprite *logo;
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
static SceneTvFrame tv_frame;

static const GridConfig program_grid = {
    .x = 44,
    .y = 120,
    .height = 120,
    .width = 240,
    .color = 0x7f4f,
    .line_count = 8,
    .perspective_offset = 200,
    .offset_x = 0,
    .offset_y = 0,
    .direction = GRID_DIRECTION_DOWN,
};

static e3d_Rectangle program_rect = {
    .x = 44,
    .y = 28,
    .height = 182,
    .width = 242,
};

static void load_assets(e3d_EngineContext *engine_ctx) {
  if (assets.logo != NULL) {
    return;
  }

  assets.segment_zero = engine_ctx->storage->get_sprite(SPRITE_SEG_0);
  assets.segment_one = engine_ctx->storage->get_sprite(SPRITE_SEG_1);
  assets.logo = engine_ctx->storage->get_sprite(SPRITE_LOGO_CHANNEL1);
}

static void end_scene_frame(void) {
  scene_ctx.scene_frame++;
  scene_ctx.scene_elapsed_ms =
      demo_platform_time_ms() - scene_ctx.scene_start_time_ms;
}

static void run_scene(DemoContext *demo_ctx) {
  e3d_EngineContext *engine_ctx = demo_ctx->engine_ctx;
  load_assets(engine_ctx);
  scene_tv_frame_load(&tv_frame, engine_ctx);
  while (demo_is_running() &&
         (scene_ctx.scene_duration == 0u ||
          scene_ctx.scene_frame < scene_ctx.scene_duration)) {
    const uint32_t frame = scene_ctx.scene_frame;

  if (frame == 0u) {
    scene_ctx.scene_start_time_ms = demo_platform_time_ms();
  }

  demo_begin_frame();

  e3d_Painter_DrawImage(engine_ctx, GFX_TEXTURE_TV_BIG);
  e3d_Painter_DrawGradient(engine_ctx, 0x01a2, 0x1ec7, &program_rect, UP);
  draw_grid(engine_ctx, &program_grid, frame);

  e3d_Painter_Print(engine_ctx, "PROGRAM WIECZORNY", 56, 52, 2, 0x0000);
  e3d_Painter_Print(engine_ctx, "PROGRAM WIECZORNY", 54, 50, 2, 0xffff);
  e3d_Painter_Print(engine_ctx, "19:20\tPrzeglond dnia", 55, 71, 1, 0x0000);
  e3d_Painter_Print(engine_ctx, "19:20\tPrzeglond dnia", 54, 70, 1, 0xffff);
  e3d_Painter_Print(engine_ctx, "20:00\tWielkie kino: Wojna swiatuf", 55, 86, 1,
                    0x0000);
  e3d_Painter_Print(engine_ctx, "20:00\tWielkie kino: Wojna swiatuf", 54, 85, 1,
                    0xffff);

  if (frame > 15u) {
    e3d_Painter_Print(engine_ctx, "22:00\tNuszka gotuje", 55, 101, 1, 0x0000);
    e3d_Painter_Print(engine_ctx, "22:00\tNuszka gotuje", 54, 100, 1, 0xffff);
  }
  if (frame > 30u) {
    e3d_Painter_Print(engine_ctx, "22:30\tProgram dla dzieci", 55, 116, 1,
                      0x0000);
    e3d_Painter_Print(engine_ctx, "22:30\tProgram dla dzieci", 54, 115, 1,
                      0xffff);
  }
  if (frame > 45u) {
    e3d_Painter_Print(engine_ctx, "00:00\tKoniec programu", 55, 131, 1, 0x0000);
    e3d_Painter_Print(engine_ctx, "00:00\tKoniec programu", 54, 130, 1, 0xffff);
  }
  if (frame <= 15u) {
    e3d_Painter_DrawSprite(engine_ctx, assets.segment_zero, 56, 36, 0, 2);
    e3d_Painter_DrawSprite(engine_ctx, assets.segment_one, 76, 36, 0, 2);
  }

  e3d_Painter_DrawSprite(engine_ctx, assets.logo, 254, 38, 0, 1);
  scene_tv_frame_draw(&tv_frame, engine_ctx);

  end_scene_frame();
  demo_end_frame();
  }
}

DemoSceneContext *scene_2_program_in_get_context(void) { return &scene_ctx; }
