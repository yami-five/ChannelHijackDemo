#include "demo_context.h"
#include "demo_scene.h"
#include "scenes/scene_10_alert_in.h"
#include "scenes/scene_11_alert_out.h"
#include "scenes/scene_12_space.h"
#include "scenes/scene_1_tv_off.h"
#include "scenes/scene_2_program_in.h"
#include "scenes/scene_3_program_out.h"
#include "scenes/scene_4_weather_in.h"
#include "scenes/scene_5_weather_out.h"
#include "scenes/scene_6_news_in.h"
#include "scenes/scene_7_news_out.h"
#include "scenes/scene_8_hijacking_in.h"
#include "scenes/scene_9_hijacking_out.h"
#include <stdbool.h>
#include <stdint.h>

#if defined(PLATFORM_WINDOWS)
#include <windows.h>
#else
#include "pico/time.h"
#endif

#define DEMO_SCENES_COUNT 12u

static e3d_EngineContext engine;
static DemoSceneContext *scenes[DEMO_SCENES_COUNT];
#if defined(PLATFORM_WINDOWS)
static SdlApplication sdl_application;
#endif

static DemoContext demo_ctx = {
    .engine_ctx = &engine,
    .scenes = NULL,
    .scenes_count = 0u,
    .active_scene_index = 0u,
    .frame = 0u,
    .elapsed_ms = 0u,
    .start_time_ms = 0u,
#if defined(PLATFORM_WINDOWS)
    .sdlApp = &sdl_application,
#endif
};

uint32_t demo_platform_time_ms(void) {
#if defined(PLATFORM_WINDOWS)
  return (uint32_t)GetTickCount();
#else
  return (uint32_t)to_ms_since_boot(get_absolute_time());
#endif
}

bool demo_is_running(void) {
#if defined(PLATFORM_WINDOWS)
  return sdl_application_is_running(demo_ctx.sdlApp);
#else
  return true;
#endif
}

void demo_begin_frame(void) {
  e3d_Buffer_ClearBuffer(demo_ctx.engine_ctx, 0);
#if defined(PLATFORM_WINDOWS)
  sdl_application_begin_frame(demo_ctx.sdlApp);
#endif
}

void demo_end_frame(void) {
  e3d_Buffer_DrawBuffer(demo_ctx.engine_ctx);
  demo_ctx.frame++;
  demo_ctx.elapsed_ms = demo_platform_time_ms() - demo_ctx.start_time_ms;

#if defined(PLATFORM_WINDOWS)
  sdl_application_end_frame(demo_ctx.sdlApp);
#endif
}

void run_demo(void) {
#if defined(PLATFORM_WINDOWS)
  if (!sdl_application_init(demo_ctx.sdlApp, 30u)) {
    return;
  }
#endif
  e3d_InitEngine(demo_ctx.engine_ctx);
  demo_ctx.start_time_ms = demo_platform_time_ms();
  scenes[0] = scene_1_tv_off_get_context();
  scenes[1] = scene_2_program_in_get_context();
  scenes[2] = scene_3_program_out_get_context();
  scenes[3] = scene_4_weather_in_get_context();
  scenes[4] = scene_5_weather_out_get_context();
  scenes[5] = scene_6_news_in_get_context();
  scenes[6] = scene_7_news_out_get_context();
  scenes[7] = scene_8_hijacking_in_get_context();
  scenes[8] = scene_9_hijacking_out_get_context();
  scenes[9] = scene_10_alert_in_get_context();
  scenes[10] = scene_11_alert_out_get_context();
  scenes[11] = scene_12_space_get_context();
  demo_ctx.scenes_count = DEMO_SCENES_COUNT;
  demo_ctx.scenes = scenes;

  for (demo_ctx.active_scene_index = 0u;
       demo_ctx.active_scene_index < demo_ctx.scenes_count && demo_is_running();
       demo_ctx.active_scene_index++) {
    DemoSceneContext *scene =
        demo_ctx.scenes[demo_ctx.active_scene_index];

    scene->scene_frame = 0u;
    scene->scene_elapsed_ms = 0u;
    scene->scene_start_time_ms = 0u;

    scene->run_scene(&demo_ctx);
  }

#if defined(PLATFORM_WINDOWS)
  sdl_application_shutdown(demo_ctx.sdlApp);
#endif
}
