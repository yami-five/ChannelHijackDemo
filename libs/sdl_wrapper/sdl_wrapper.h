#ifndef CHANNEL_HIJACK_DEMO_SDL_WRAPPER_H
#define CHANNEL_HIJACK_DEMO_SDL_WRAPPER_H

#include <stdbool.h>
#include <stdint.h>

#if defined(PLATFORM_WINDOWS)
typedef struct {
    bool running;
    bool paused;
    uint32_t target_fps;
    uint64_t frame_begin_ticks;
} SdlApplication;

bool sdl_application_init(SdlApplication *application, uint32_t target_fps);
bool sdl_application_is_running(const SdlApplication *application);
bool sdl_application_is_paused(const SdlApplication *application);
void sdl_application_begin_frame(SdlApplication *application);
void sdl_application_end_frame(const SdlApplication *application);
void sdl_application_shutdown(SdlApplication *application);
#endif

#endif
