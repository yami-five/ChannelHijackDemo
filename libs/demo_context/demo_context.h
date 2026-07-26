#ifndef DEMOCONTEXT_h
#define DEMOCONTEXT_h

#include "engineApi.h"
#include <stdint.h>

typedef struct {
  e3d_EngineContext ctx;
  uint64_t t;
  uint64_t elapsed_ms;
  uint64_t start_time_ms;
#if defined(PLATFORM_WINDOWS)
  SdlApplication sdlApp;
#endif
} DemoContext;

#endif