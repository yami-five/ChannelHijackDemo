#include <stdio.h>
#include <stdint.h>

#if defined(PLATFORM_PICO)
#include "pico/multicore.h"
#include "pico/time.h"
#elif defined(PLATFORM_WINDOWS)
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#endif

#include "ICameraFactory.h"
#include "IDisplay.h"
#include "IHardware.h"
#include "ILightFactory.h"
#include "IMeshFactory.h"
#include "IPainter.h"
#include "IRenderer.h"
#include "IStorage.h"
#include "IPuppeteer.h"

#include "cameraFactory.h"
#include "lightFactory.h"
#include "meshFactory.h"
#include "renderer.h"
#include "mesh.h"
#include "camera.h"
#include "storage.h"
#include "puppeteer.h"
#include "puppet.h"

#if defined(PLATFORM_PICO)
#include "display.h"
#include "hardware.h"
#endif

const IPainter *get_painter(void);

static const IHardware *hardware_core;
static const IDisplay *display;
static const IPainter *painter;
static const IRenderer *renderer;
static const IMeshFactory *meshFactory;
static const ILightFactory *lightFactory;
static const ICameraFactory *cameraFactory;
static const IStorage *storage;
static const IPuppeteer *puppeteer;

#if defined(EUZEBIA3D_PLATFORM_WINDOWS)
#define EUZEBIA3D_WINDOWS_TARGET_FPS 24u

static int require_pointer(const void *pointer, const char *name)
{
    if (pointer != NULL)
    {
        return 1;
    }

    SDL_Log("%s failed", name);
    return 0;
}

static int process_window_events(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            return 0;
        }
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
        {
            return 0;
        }
    }

    return 1;
}

static void cap_window_frame_rate(uint64_t frame_begin_ticks)
{
    uint64_t performance_frequency = SDL_GetPerformanceFrequency();
    if (performance_frequency == 0u || EUZEBIA3D_WINDOWS_TARGET_FPS == 0u)
    {
        return;
    }

    uint64_t target_frame_ticks = performance_frequency / EUZEBIA3D_WINDOWS_TARGET_FPS;
    uint64_t elapsed_ticks = SDL_GetPerformanceCounter() - frame_begin_ticks;
    if (elapsed_ticks >= target_frame_ticks)
    {
        return;
    }

    uint64_t remaining_ticks = target_frame_ticks - elapsed_ticks;
    uint64_t remaining_ms = (remaining_ticks * 1000u) / performance_frequency;
    if (remaining_ms > 0u)
    {
        SDL_Delay((uint32_t)remaining_ms);
    }
}
#endif

#if defined(PLATFORM_WINDOWS)
int main(int argc, char **argv)
#else
int main(void)
#endif
{
#if defined(PLATFORM_WINDOWS)
    (void)argc;
    (void)argv;
#endif

#if defined(PLATFORM_PICO)
    set_sys_clock_khz(300000, true);

    hardware_core = get_hardware();
    hardware_core->init_hardware();

    display = get_display();
    display->init_display(hardware_core);

    storage = get_storage();
#else
    storage = get_storage();
    if (storage == NULL)
    {
        SDL_Log("get_storage failed");
        return 1;
    }
#endif

    painter = get_painter();
#if defined(PLATFORM_WINDOWS)
    if (painter == NULL)
    {
        SDL_Log("get_painter failed");
        return 1;
    }
#endif
    painter->init_painter(display, hardware_core, storage);

    renderer = get_renderer();
#if defined(PLATFORM_WINDOWS)
    if (renderer == NULL)
    {
        SDL_Log("get_renderer failed");
        return 1;
    }
#endif
    renderer->init_renderer(hardware_core, painter);
    renderer->set_scale(1);

    meshFactory = get_meshFactory();
#if defined(PLATFORM_WINDOWS)
    if (meshFactory == NULL)
    {
        SDL_Log("get_meshFactory failed");
        return 1;
    }
#endif
    lightFactory = get_lightFactory();
#if defined(PLATFORM_WINDOWS)
    if (lightFactory == NULL)
    {
        SDL_Log("get_lightFactory failed");
        return 1;
    }
#endif
    cameraFactory = get_cameraFactory();
#if defined(PLATFORM_WINDOWS)
    if (cameraFactory == NULL)
    {
        SDL_Log("get_cameraFactory failed");
        return 1;
    }
#endif
    puppeteer = get_puppeteer();
#if defined(PLATFORM_WINDOWS)
    if (cameraFactory == NULL)
    {
        SDL_Log("get_puppeteer failed");
        return 1;
    }
#endif
    puppeteer->init_puppeteer(storage, painter);
    Puppet *pogodynka = puppeteer->create_puppet(0);

    painter->clear_buffer(0x1100);
    painter->draw_buffer();
    // weather report
    const Image *tv_big_background = storage->get_image(0);
    const Sprite *map = storage->get_sprite(0);
    const Sprite *tv_big_frame1 = storage->get_sprite(1);
    const Sprite *tv_big_frame2 = storage->get_sprite(2);
    const Sprite *tv_big_frame3 = storage->get_sprite(3);
    const Sprite *weather_1_1 = storage->get_sprite(4);
    const Sprite *weather_1_2 = storage->get_sprite(4);
    const Sprite *weather_1_3 = storage->get_sprite(4);
    const Sprite *weather_1_4 = storage->get_sprite(4);
    const Sprite *weather_1_5 = storage->get_sprite(4);
    const Sprite *weather_2_1 = storage->get_sprite(5);
    const Sprite *weather_2_2 = storage->get_sprite(5);
    const Sprite *weather_2_3 = storage->get_sprite(5);
    const Sprite *weather_2_4 = storage->get_sprite(5);
    const Sprite *weather_2_5 = storage->get_sprite(5);
    const Sprite *weather_3_1 = storage->get_sprite(6);
    const Sprite *weather_3_2 = storage->get_sprite(6);
    const Sprite *weather_3_3 = storage->get_sprite(6);
    const Sprite *weather_3_4 = storage->get_sprite(6);
    const Sprite *weather_3_5 = storage->get_sprite(6);
    const Sprite *weather_3_6 = storage->get_sprite(6);
    const Sprite *weather_4_1 = storage->get_sprite(7);
    const Sprite *weather_4_2 = storage->get_sprite(7);
    const Sprite *weather_4_3 = storage->get_sprite(7);
    const Sprite *weather_5_1 = storage->get_sprite(8);
    const Sprite *weather_5_2 = storage->get_sprite(8);
    const Sprite *weather_5_3 = storage->get_sprite(8);
    // news
    const Sprite *agro_main = storage->get_sprite(19);
    Sprite *agro_mouth[4];
    agro_mouth[0] = storage->get_sprite(20);
    agro_mouth[1] = storage->get_sprite(21);
    agro_mouth[2] = storage->get_sprite(22);
    agro_mouth[3] = storage->get_sprite(21);
    const uint32_t agro_mouth_frame_hold = 3u;
    const uint32_t agro_mouth_frame_count = sizeof(agro_mouth) / sizeof(agro_mouth[0]);

    uint16_t plasmaColors[15] = {
        0x1be6,
        0x2427,
        0x3447,
        0x4488,
        0x54c8,
        0x5d09,
        0x6d49,
        0x7d8a,
        0x6d49,
        0x5d09,
        0x54c8,
        0x4488,
        0x3447,
        0x2427,
        0x1be6,
    };
    Rectangle plasmaRect = {
        .x = 28,
        .y = 44,
        .height = 181,
        .width = 241,
    };
    //
    uint32_t t = 0;
#if defined(PLATFORM_WINDOWS)
    int running = 1;
    while (running)
    {
        uint64_t frame_begin_ticks = SDL_GetPerformanceCounter();

        running = process_window_events();
#else
    while (1)
    {
#endif
        painter->clear_buffer(10);
        painter->draw_background(tv_big_background);
        // weather report
        // painter->draw_sprite(map, 44, 1, 0, 1);
        // painter->draw_sprite(weather_1_1, 66, 70, 0, 1);
        // painter->draw_sprite(weather_1_2, 77, 88, 0, 1);
        // painter->draw_sprite(weather_1_3, 82, 112, 0, 1);
        // painter->draw_sprite(weather_1_4, 108, 136, 0, 1);
        // painter->draw_sprite(weather_1_5, 106, 100, 0, 1);
        // painter->draw_sprite(weather_2_1, 126, 76, 0, 1);
        // painter->draw_sprite(weather_2_2, 134, 35, 0, 1);
        // painter->draw_sprite(weather_2_3, 169, 56, 0, 1);
        // painter->draw_sprite(weather_2_4, 212, 47, 0, 1);
        // painter->draw_sprite(weather_2_5, 214, 75, 0, 1);
        // painter->draw_sprite(weather_3_1, 150, 114, 0, 1);
        // painter->draw_sprite(weather_3_2, 178, 100, 0, 1);
        // painter->draw_sprite(weather_3_3, 142, 154, 0, 1);
        // painter->draw_sprite(weather_3_4, 204, 128, 0, 1);
        // painter->draw_sprite(weather_3_5, 158, 160, 0, 1);
        // painter->draw_sprite(weather_3_6, 193, 160, 0, 1);
        // painter->draw_sprite(weather_4_1, 100, 55, 0, 1);
        // painter->draw_sprite(weather_4_2, 174, 33, 0, 1);
        // painter->draw_sprite(weather_4_3, 52, 112, 0, 1);
        // painter->draw_sprite(weather_5_1, 114, 184, 0, 1);
        // painter->draw_sprite(weather_5_2, 178, 140, 0, 1);
        // painter->draw_sprite(weather_5_3, 244, 124, 0, 1);
        // puppeteer->perform(pogodynka, t);

        // news
        painter->draw_plasma(plasmaColors, 15, t, 7, 7, 8, 7, &plasmaRect);
        painter->draw_sprite(agro_main, 130, 44, 0, 1);
        painter->draw_sprite(agro_mouth[(t / agro_mouth_frame_hold) % agro_mouth_frame_count], 208, 79, 0, 1);
        painter->draw_sprite(tv_big_frame1, 272, 20, 0, 1);
        painter->draw_sprite(tv_big_frame2, 44, 31, 0, 1);
        painter->draw_sprite(tv_big_frame3, 23, 200, 0, 1);
        float qt = t * 0.02f;
        (void)qt;
        painter->draw_buffer();
        t++;
#if defined(PLATFORM_WINDOWS)

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
            {
                running = 0;
            }
        }

        cap_window_frame_rate(frame_begin_ticks);
#endif
    }

#if defined(PLATFORM_WINDOWS)
    SDL_Quit();
    return 0;
#else
    // multicore_launch_core1(core1_main);
    return 0;
#endif
}
