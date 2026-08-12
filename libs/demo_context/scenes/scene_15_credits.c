#include "scene_15_credits.h"
#include "../demo_context.h"
#include "../demo_scene.h"
#include "animation.h"
#include "scene_15_credits.inc"
#include "storage/gfx_indices.h"
#include "storage/sprites._indices.h"

#define LINES_NUMBER 93u

static void run_scene(DemoContext *demo_ctx);
static DemoSceneContext scene_ctx = {
    .scene_frame = 0u,
    .scene_elapsed_ms = 0u,
    .scene_start_time_ms = 0u,
    .scene_duration = 1800u,
    .run_scene = run_scene,
};

static void end_scene_frame(DemoSceneContext *scene_ctx) {
  scene_ctx->scene_frame++;
  scene_ctx->scene_elapsed_ms =
      demo_platform_time_ms() - scene_ctx->scene_start_time_ms;
}
static void run_scene(DemoContext *demo_ctx) {
  e3d_EngineContext *engine_ctx = demo_ctx->engine_ctx;
  while (demo_is_running() &&
         (scene_ctx.scene_duration == 0u ||
          scene_ctx.scene_frame < scene_ctx.scene_duration)) {
    demo_begin_frame();
    for (uint8_t i = 0; i < LINES_NUMBER; i++) {
      const text_line current_line = textLines[i];
      e3d_Painter_Print(engine_ctx, current_line.text, current_line.x,
                        current_line.y-scene_ctx.scene_frame+300, current_line.fontIndex,
                        current_line.color);
    }
    end_scene_frame(&scene_ctx);
    demo_end_frame();
  }
}
DemoSceneContext *scene_15_credits_get_context(void) { return &scene_ctx; }
