#include "sdl_wrapper.h"

#if defined(PLATFORM_WINDOWS)
#include <SDL3/SDL.h>

static bool SDLCALL handle_window_event(void *userdata, SDL_Event *event)
{
    SdlApplication *application = (SdlApplication *)userdata;

    if (application == NULL || event == NULL)
    {
        return true;
    }

    if (event->type == SDL_EVENT_QUIT)
    {
        application->running = false;
    }
    else if (event->type == SDL_EVENT_KEY_DOWN && !event->key.repeat)
    {
        if (event->key.key == SDLK_ESCAPE)
        {
            application->running = false;
        }
#if defined(EUZEBIA3D_DEBUG_MODE)
        else if (event->key.key == SDLK_SPACE)
        {
            application->paused = !application->paused;
        }
#endif
    }

    return true;
}

bool sdl_application_init(SdlApplication *application, uint32_t target_fps)
{
    if (application == NULL)
    {
        return false;
    }

    *application = (SdlApplication){
        .running = true,
        .paused = false,
        .target_fps = target_fps,
        .frame_begin_ticks = 0u,
    };

    if (!SDL_AddEventWatch(handle_window_event, application))
    {
        SDL_Log("SDL_AddEventWatch failed: %s", SDL_GetError());
        application->running = false;
        return false;
    }

    return true;
}

bool sdl_application_is_running(const SdlApplication *application)
{
    return application != NULL && application->running;
}

bool sdl_application_is_paused(const SdlApplication *application)
{
    return application != NULL && application->paused;
}

void sdl_application_begin_frame(SdlApplication *application)
{
    if (application != NULL)
    {
        application->frame_begin_ticks = SDL_GetPerformanceCounter();
    }
}

void sdl_application_end_frame(const SdlApplication *application)
{
    if (application == NULL || application->target_fps == 0u)
    {
        return;
    }

    uint64_t performance_frequency = SDL_GetPerformanceFrequency();
    if (performance_frequency == 0u)
    {
        return;
    }

    uint64_t target_ticks = performance_frequency / application->target_fps;
    uint64_t elapsed_ticks =
        SDL_GetPerformanceCounter() - application->frame_begin_ticks;
    if (elapsed_ticks >= target_ticks)
    {
        return;
    }

    uint64_t remaining_ns =
        ((target_ticks - elapsed_ticks) * 1000000000ull) /
        performance_frequency;
    if (remaining_ns > 0u)
    {
        SDL_DelayPrecise(remaining_ns);
    }
}

void sdl_application_shutdown(SdlApplication *application)
{
    if (application == NULL)
    {
        return;
    }

    SDL_RemoveEventWatch(handle_window_event, application);
    application->running = false;
    SDL_Quit();
}
#endif
