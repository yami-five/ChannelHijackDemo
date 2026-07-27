#ifndef DEMO_SCENE_H
#define DEMO_SCENE_H

#include <stdint.h>

typedef struct DemoContext DemoContext;

typedef struct DemoSceneContext {
  uint32_t scene_frame;
  uint32_t scene_elapsed_ms;
  uint32_t scene_start_time_ms;
  uint32_t scene_duration;
  void (*run_scene)(DemoContext *demo_ctx);
} DemoSceneContext;

#endif
