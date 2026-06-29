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
    // tv
    const Image *tv_big_background = storage->get_image(0);
    const Image *tv_background = storage->get_image(1);
    const Sprite *tv_big_frame1 = storage->get_sprite(1);
    const Sprite *tv_big_frame2 = storage->get_sprite(2);
    const Sprite *tv_big_frame3 = storage->get_sprite(3);
    Sprite *segment_numbers[10];
    segment_numbers[0] = storage->get_sprite(24);
    segment_numbers[1] = storage->get_sprite(25);
    segment_numbers[2] = storage->get_sprite(26);
    segment_numbers[3] = storage->get_sprite(27);
    segment_numbers[4] = storage->get_sprite(28);
    segment_numbers[5] = storage->get_sprite(29);
    segment_numbers[6] = storage->get_sprite(30);
    segment_numbers[7] = storage->get_sprite(31);
    segment_numbers[8] = storage->get_sprite(32);
    segment_numbers[9] = storage->get_sprite(33);
    // weather report
    const Sprite *map = storage->get_sprite(0);
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
    const Sprite *burak = storage->get_sprite(23);
    Sprite *agro_mouth[4];
    agro_mouth[0] = storage->get_sprite(20);
    agro_mouth[1] = storage->get_sprite(21);
    agro_mouth[2] = storage->get_sprite(22);
    agro_mouth[3] = storage->get_sprite(21);
    const uint32_t agro_mouth_frame_hold = 3u;
    const uint32_t agro_mouth_frame_count = sizeof(agro_mouth) / sizeof(agro_mouth[0]);
    // tv
    const Sprite *channel1 = storage->get_sprite(34);
    const Sprite *channel2 = storage->get_sprite(35);

    uint16_t plasmaColors[16] = {
        0x1be6,
        0x2427,
        0x3447,
        0x4488,
        0x54c8,
        0x5d09,
        0x6d49,
        0x7d8a,
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
        .height = 182,
        .width = 242,
    };
    Rectangle bar1 = {
        .x = 15,
        .y = 38,
        .height = 20,
        .width = 280,
    };
    Rectangle bar2 = {
        .x = 15,
        .y = 28,
        .height = 10,
        .width = 280,
    };
    char *text = "PILNE: Ministerstwo Grabi i Widel uspokaja, ze tegoroczny wysyp kabaczkuf nie zagraza bespieczenstwu panstwa...";// Rolnicy z gminy Dolne Pole donoszo, ze kombajn pana Zdzislawa sam odmuwil pracy i zazondal urlopu pod gruszom... Ceny marchwi so stabilne, ale eksperty ostrzegajo przed panikom wsrod krolikuf... Na rynku zboz lekkie poruszenie po tym, jak pszenica ozima zaczela zadawac niewygodne pytania o sens rzycia... Kolo Gospodyn Wiejskich zapowiada nowom technologie kiszenia ogurkuf w chmurze, choc starsze mieszkance nadal wolo beczke, bo przynajmniej nie wymaga aktualizaci... Wedlug raportu Instytutu Spraw Slomianych az 73 procent strachuf na wroble czuje sie nie docenionych i rozwaza kariere influenseruf pogodowych... W powiecie ziemniaczanym wykryto podejzanie okronglego buraka, sprawe bada komisja do spraw warzyw geometrycznie nie pokojoncych... Hodofcy kur apelujo o cisze nocnom po tym, jak jeden kogut przeszedl na tryb alarmu 24/7 i zaczol piac takrze w formacie stereo... Prognozy dla rolnictwa so umiarkowanie optymistyczne: bedzie padac, nie bedzie padac albo bedzie padac wtedy, kiedy nikt o to nie prosil... Gielda nawozuf zakonczyla dzien lekkim smrodem, ale analityki twierdzo, ze to naturalna korekcja rynku... W sadach rozpoczeto testy inteligentnych jablek, kture same spadajo do skrzynki, ale tylko po zaakceptowaniu regulaminu... Agencja Restrukturyzaci i Modernizaci Grzondek przypomina, ze wnioski o doplaty do samotnych poruf nalezy skladac do piontku, chyba ze por ma juz wsparcie rodziny... Lokalny soltys zdementowal plotki, jakoby traktor marki Ursus zostal widziany na randce z przyczepom samozbierajoncom... Na koniec przypominamy: kto sieje wiatr, ten zbiera kontrole z urzendu, a kto sieje rzepak, ten przynajmniej wie, po co wstal o czwartej rano.";
    //
    uint32_t t = 0;
    uint8_t scene = 0;
    uint32_t scene_start_t = t;
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
        if (scene < 2)
            // tv zoom in
            painter->draw_background(tv_big_background);
        else
            // tv zoom out
            painter->draw_background(tv_background);
        if (scene == 0)
        {
            // weather report
            painter->draw_sprite(map, 44, 1, 0, 1);
            painter->draw_sprite(weather_1_1, 66, 70, 0, 1);
            painter->draw_sprite(weather_1_2, 77, 88, 0, 1);
            painter->draw_sprite(weather_1_3, 82, 112, 0, 1);
            painter->draw_sprite(weather_1_4, 108, 136, 0, 1);
            painter->draw_sprite(weather_1_5, 106, 100, 0, 1);
            painter->draw_sprite(weather_2_1, 126, 76, 0, 1);
            painter->draw_sprite(weather_2_2, 134, 35, 0, 1);
            painter->draw_sprite(weather_2_3, 169, 56, 0, 1);
            painter->draw_sprite(weather_2_4, 212, 47, 0, 1);
            painter->draw_sprite(weather_2_5, 214, 75, 0, 1);
            painter->draw_sprite(weather_3_1, 150, 114, 0, 1);
            painter->draw_sprite(weather_3_2, 178, 100, 0, 1);
            painter->draw_sprite(weather_3_3, 142, 154, 0, 1);
            painter->draw_sprite(weather_3_4, 204, 128, 0, 1);
            painter->draw_sprite(weather_3_5, 158, 160, 0, 1);
            painter->draw_sprite(weather_3_6, 193, 160, 0, 1);
            painter->draw_sprite(weather_4_1, 100, 55, 0, 1);
            painter->draw_sprite(weather_4_2, 174, 33, 0, 1);
            painter->draw_sprite(weather_4_3, 52, 112, 0, 1);
            painter->draw_sprite(weather_5_1, 114, 184, 0, 1);
            painter->draw_sprite(weather_5_2, 178, 140, 0, 1);
            painter->draw_sprite(weather_5_3, 244, 124, 0, 1);
            puppeteer->perform(pogodynka, t);
            if (t - scene_start_t <= 10)
            {
                painter->draw_sprite(segment_numbers[0], 56, 36, 0, 2);
                painter->draw_sprite(segment_numbers[1], 76, 36, 0, 2);
            }
        }
        else if (scene == 1)
        {
            // news
            painter->draw_plasma(plasmaColors, 16, t, 2, 6, 6, 7, 6, &plasmaRect);
            painter->draw_sprite(agro_main, 130, 44, 0, 1);
            painter->draw_sprite(burak, 54, 38, 0, 1);
            painter->draw_rectangle(&bar1, 0x34b2);
            painter->draw_rectangle(&bar2, 0x3292);
            painter->print(text, 0 - (t * 3), 203, 1, 0xffff);
            painter->print("Ceny burakuf rosno!!1", 75, 185, 0, 0xffff);
            painter->draw_sprite(agro_mouth[t % agro_mouth_frame_count], 208, 79, 0, 1);
            if (t - scene_start_t <= 5)
            {
                painter->draw_sprite(segment_numbers[0], 56, 36, 0, 2);
                painter->draw_sprite(segment_numbers[2], 76, 36, 0, 2);
            }
        }
        else if (scene == 2)
        {
            painter->draw_sprite(channel1, 91, 76, 0, 1);
        }
        else
        {
            painter->draw_sprite(channel2, 91, 76, 0, 1);
        }
        if (scene < 2)
        {
            // tv frame
            painter->draw_sprite(tv_big_frame1, 272, 20, 0, 1);
            painter->draw_sprite(tv_big_frame2, 44, 31, 0, 1);
            painter->draw_sprite(tv_big_frame3, 0, 169, 0, 1);
        }
        float qt = t * 0.02f;
        (void)qt;
        painter->draw_buffer();
        t++;
        if (t % 60 == 0)
        {
            scene++;
            if (scene > 3)
                scene = 0;
            scene_start_t = t + 1;
        }
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
