#ifndef SCENE_15_CREDITS_H
#define SCENE_15_CREDITS_H

#include "stdint.h"

typedef struct DemoSceneContext DemoSceneContext;

typedef struct {
  char *text;
  int16_t x;
  int16_t y;
  uint8_t fontIndex;
  uint16_t color;
} text_line;

DemoSceneContext *scene_15_credits_get_context(void);
#endif
