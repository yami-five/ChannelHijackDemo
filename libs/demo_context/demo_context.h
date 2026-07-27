#ifndef DEMO_CONTEXT_H
#define DEMO_CONTEXT_H

#include "engineApi.h"
#include <stdbool.h>
#include <stdint.h>
#if defined(PLATFORM_WINDOWS)
#include "sdl_wrapper.h"
#endif

typedef struct DemoSceneContext DemoSceneContext;

typedef struct DemoContext {
  e3d_EngineContext *engine_ctx;
  DemoSceneContext **scenes;
  uint32_t scenes_count;
  uint32_t active_scene_index;
  uint32_t frame;
  uint32_t elapsed_ms;
  uint32_t start_time_ms;
#if defined(PLATFORM_WINDOWS)
  SdlApplication *sdlApp;
#endif
} DemoContext;

void demo_begin_frame(void);
void demo_end_frame(void);
uint32_t demo_platform_time_ms(void);
bool demo_is_running(void);
void run_demo(void);

#endif
