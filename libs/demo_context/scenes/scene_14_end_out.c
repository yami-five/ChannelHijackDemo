#include "scene_14_end_out.h"
#include "../demo_context.h"
#include "../demo_scene.h"
#include "animation.h"
#include "storage/gfx_indices.h"
#include "storage/sprites._indices.h"

#define CURTAIN_LENGTH 20u
static void run_scene(DemoContext *demo_ctx);
static DemoSceneContext scene_ctx = {
    .scene_frame = 0u,
    .scene_elapsed_ms = 0u,
    .scene_start_time_ms = 0u,
    .scene_duration = 60u,
    .run_scene = run_scene,
};
static const e3d_Sprite *end_small;
static const e3d_Sprite *curtain[CURTAIN_LENGTH];
const e3d_Sprite *left_hands[7];
static const SpriteWaveAnimation curtain_animation = {-55, 0,   0,   12, 6,
                                                      0,   500, 700, 0,  1};
static void load_assets(e3d_EngineContext *engine_ctx) {
  if (end_small != NULL)
    return;
  end_small = engine_ctx->storage->get_sprite(SPRITE_END_SMALL);
  for (uint32_t i = 0u; i < CURTAIN_LENGTH; i++)
    curtain[i] = engine_ctx->storage->get_sprite(SPRITE_FIRANKA);
  for (uint32_t i = 0u; i < 7u; i++) {
    left_hands[i] =
        engine_ctx->storage->get_sprite((uint8_t)(SPRITE_LEFT1 + i));
  }
}
static void finish(void) {
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
    uint32_t f = scene_ctx.scene_frame;
    if (f == 0u)
      scene_ctx.scene_start_time_ms = demo_platform_time_ms();
    demo_begin_frame();
    e3d_Painter_DrawImage(engine_ctx, GFX_TEXTURE_TV);
    e3d_Painter_DrawSprite(engine_ctx, end_small, 91, 76, 0, 1);
    draw_sprite_wave(engine_ctx, curtain, CURTAIN_LENGTH, f,
                     &curtain_animation);
    const uint32_t left_hand_index =
        (scene_ctx.scene_frame > 50u && scene_ctx.scene_frame <= 60u) ? 6u : 5u;
    e3d_Painter_DrawSprite(
        engine_ctx, left_hands[left_hand_index],
        (int16_t)(-22 +
                  wave_offset(scene_ctx.scene_frame, 350u, TABLE_SIZE / 2u, 2)),
        (int16_t)(147 +
                  wave_offset(scene_ctx.scene_frame, 350u, TABLE_SIZE / 2u, 2)),
        0, 1);
    finish();
    demo_end_frame();
  }
}
DemoSceneContext *scene_14_end_out_get_context(void) { return &scene_ctx; }
