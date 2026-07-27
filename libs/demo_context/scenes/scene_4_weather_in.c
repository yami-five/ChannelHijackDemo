#include "../demo_context.h"
#include "../demo_scene.h"
#include "scene_4_weather_in.h"
#include "scene_tv.h"

typedef struct {
  uint8_t sprite_index;
  int16_t x;
  int16_t y;
  const e3d_Sprite *sprite;
} WeatherMarker;

static void run_scene(DemoContext *demo_ctx);
static DemoSceneContext scene_ctx = {
    .scene_frame = 0u,
    .scene_elapsed_ms = 0u,
    .scene_start_time_ms = 0u,
    .scene_duration = 60u,
    .run_scene = run_scene,
};
static const e3d_Sprite *map;
static const e3d_Sprite *segment_zero;
static const e3d_Sprite *segment_two;
static const e3d_Sprite *logo;
static e3d_Puppet *weather_presenter;
static SceneTvFrame tv_frame;
static WeatherMarker markers[] = {
    {4u, 66, 70, NULL},  {4u, 77, 88, NULL},  {4u, 82, 112, NULL},
    {4u, 108, 136, NULL},{4u, 106, 100, NULL},{5u, 126, 76, NULL},
    {5u, 134, 35, NULL}, {5u, 169, 56, NULL}, {5u, 212, 47, NULL},
    {5u, 214, 75, NULL}, {6u, 150, 114, NULL},{6u, 178, 100, NULL},
    {6u, 142, 154, NULL},{6u, 204, 128, NULL},{6u, 158, 160, NULL},
    {6u, 193, 160, NULL},{7u, 100, 55, NULL}, {7u, 174, 33, NULL},
    {7u, 52, 112, NULL}, {8u, 114, 184, NULL},{8u, 178, 140, NULL},
    {8u, 244, 124, NULL},
};
static uint16_t noise_colors[] = {0xffff, 0xad75, 0x5acb, 0x0000};
static e3d_Rectangle plasma_rect = {28, 44, 182, 242};

static void load_assets(e3d_EngineContext *engine_ctx) {
  if (map != NULL) return;
  map = engine_ctx->storage->get_sprite(0u);
  segment_zero = engine_ctx->storage->get_sprite(24u);
  segment_two = engine_ctx->storage->get_sprite(26u);
  logo = engine_ctx->storage->get_sprite(51u);
  weather_presenter = e3d_Puppetteer_CreatePuppet(engine_ctx, 0u);
  for (uint32_t i = 0u; i < sizeof(markers) / sizeof(markers[0]); i++)
    markers[i].sprite = engine_ctx->storage->get_sprite(markers[i].sprite_index);
}

static void end_frame(void) {
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
  uint32_t frame = scene_ctx.scene_frame;
  if (frame == 0u) scene_ctx.scene_start_time_ms = demo_platform_time_ms();
  demo_begin_frame();
  e3d_Painter_DrawImage(engine_ctx, 0u);
  if (frame >= 5u && frame < 10u) {
    e3d_Painter_DrawPlasma(engine_ctx, noise_colors, 4u, frame, 2, 3, 4, 5, 3,
                           &plasma_rect);
  } else {
    e3d_Painter_DrawSprite(engine_ctx, map, 44, 1, 0, 1);
    for (uint32_t i = 0u; i < sizeof(markers) / sizeof(markers[0]); i++)
      e3d_Painter_DrawSprite(engine_ctx, markers[i].sprite, markers[i].x,
                             markers[i].y, 0, 1);
    e3d_Puppetteer_Perform(engine_ctx, weather_presenter, frame);
    if (frame <= 15u) {
      e3d_Painter_DrawSprite(engine_ctx, segment_zero, 56, 36, 0, 2);
      e3d_Painter_DrawSprite(engine_ctx, segment_two, 76, 36, 0, 2);
    }
    e3d_Painter_DrawSprite(engine_ctx, logo, 254, 38, 0, 1);
  }
  scene_tv_frame_draw(&tv_frame, engine_ctx);
  end_frame();
  demo_end_frame();
  }
}

DemoSceneContext *scene_4_weather_in_get_context(void) { return &scene_ctx; }
