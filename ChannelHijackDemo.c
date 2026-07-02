#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(PLATFORM_PICO)
#include "pico/multicore.h"
#include "pico/time.h"
#elif defined(PLATFORM_WINDOWS)
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#endif

#include "ICameraFactory.h"
#if defined(EUZEBIA3D_DEBUG_MODE)
#include "IDebugMode.h"
#endif
#include "IDisplay.h"
#include "IHardware.h"
#include "ILightFactory.h"
#include "IMeshFactory.h"
#include "IPainter.h"
#include "IRenderer.h"
#include "IStorage.h"
#include "IPuppeteer.h"

#include "cameraFactory.h"
#if defined(EUZEBIA3D_DEBUG_MODE)
#include "debugMode.h"
#endif
#include "lightFactory.h"
#include "meshFactory.h"
#include "renderer.h"
#include "mesh.h"
#include "camera.h"
#include "storage.h"
#include "puppeteer.h"
#include "puppet.h"

#include "fpa.h"

#include "ChannelHijackDemoHelpers.h"

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
#if defined(EUZEBIA3D_DEBUG_MODE)
static const IDebugMode *debugMode;
static char t_char[11];
static bool pause = false;
#endif

#if defined(PLATFORM_WINDOWS)
typedef struct WindowsEventState
{
    bool running;
#if defined(EUZEBIA3D_DEBUG_MODE)
    unsigned int pause_toggle_count;
#endif
} WindowsEventState;

static bool SDLCALL handle_window_event(void *userdata, SDL_Event *event)
{
    WindowsEventState *state = (WindowsEventState *)userdata;
    if (state == NULL || event == NULL)
    {
        return true;
    }

    if (event->type == SDL_EVENT_QUIT)
    {
        state->running = false;
    }
    else if (event->type == SDL_EVENT_KEY_DOWN && !event->key.repeat)
    {
        if (event->key.key == SDLK_ESCAPE)
        {
            state->running = false;
        }
#if defined(EUZEBIA3D_DEBUG_MODE)
        else if (event->key.key == SDLK_SPACE)
        {
            state->pause_toggle_count++;
        }
#endif
    }

    return true;
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
    set_sys_clock_khz(320000, true);

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

#if defined(EUZEBIA3D_DEBUG_MODE)
    debugMode = get_debugMode();
#if defined(PLATFORM_WINDOWS)
    if (!require_pointer(debugMode, "get_debugMode"))
    {
        return 1;
    }
#endif
    debugMode->init_debug_mode(hardware_core, painter);
#endif

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

#if defined(PLATFORM_WINDOWS)
    WindowsEventState event_state = {
        .running = true,
#if defined(EUZEBIA3D_DEBUG_MODE)
        .pause_toggle_count = 0u,
#endif
    };
    if (!SDL_AddEventWatch(handle_window_event, &event_state))
    {
        SDL_Log("SDL_AddEventWatch failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
#endif

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
    static Sprite *channels[3];
    for (uint8_t i = 0; i < 3; i++)
    {
        channels[i] = storage->get_sprite(34 + i);
    }
#define curtainLen 20
    static Sprite *curtain[curtainLen];
    for (uint8_t i = 0; i < curtainLen; i++)
    {
        curtain[i] = storage->get_sprite(37);
    }
    Sprite *leftHands[7];
    for (uint8_t i = 0; i < 7; i++)
    {
        leftHands[i] = storage->get_sprite(38 + i);
    }
    Sprite *rightHands[5];
    for (uint8_t i = 0; i < 5; i++)
    {
        rightHands[i] = storage->get_sprite(45 + i);
    }
    static uint16_t plasmaColors[16] = {
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
    static uint16_t plasmaNoiseColors[4] = 
    {
        0xffff,
        0xad75,
        0x5acb,
        0x0000,
    };
    static Rectangle plasmaRect = {
        .x = 28,
        .y = 44,
        .height = 182,
        .width = 242,
    };
    static Rectangle bar1 = {
        .x = 15,
        .y = 38,
        .height = 20,
        .width = 280,
    };
    static Rectangle bar2 = {
        .x = 15,
        .y = 28,
        .height = 10,
        .width = 280,
    };
    static Rectangle rect = {
        .x = 44,
        .y = 28,
        .height = 182,
        .width = 242,
    };
    char *text = "PILNE: Ministerstwo Grabi i Widel uspokaja, ze tegoroczny wysyp kabaczkuf nie zagraza bespieczenstwu panstwa... Rolnicy z gminy Dolne Pole donoszo, ze kombajn pana Zdzislawa sam odmuwil pracy i zazondal urlopu pod gruszom... Ceny marchwi so stabilne, ale eksperty ostrzegajo przed panikom wsrod krolikuf..."; // Na rynku zboz lekkie poruszenie po tym, jak pszenica ozima zaczela zadawac niewygodne pytania o sens rzycia... Kolo Gospodyn Wiejskich zapowiada nowom technologie kiszenia ogurkuf w chmurze, choc starsze mieszkance nadal wolo beczke, bo przynajmniej nie wymaga aktualizaci... Wedlug raportu Instytutu Spraw Slomianych az 73 procent strachuf na wroble czuje sie nie docenionych i rozwaza kariere influenseruf pogodowych... W powiecie ziemniaczanym wykryto podejzanie okronglego buraka, sprawe bada komisja do spraw warzyw geometrycznie nie pokojoncych... Hodofcy kur apelujo o cisze nocnom po tym, jak jeden kogut przeszedl na tryb alarmu 24/7 i zaczol piac takrze w formacie stereo... Prognozy dla rolnictwa so umiarkowanie optymistyczne: bedzie padac, nie bedzie padac albo bedzie padac wtedy, kiedy nikt o to nie prosil... Gielda nawozuf zakonczyla dzien lekkim smrodem, ale analityki twierdzo, ze to naturalna korekcja rynku... W sadach rozpoczeto testy inteligentnych jablek, kture same spadajo do skrzynki, ale tylko po zaakceptowaniu regulaminu... Agencja Restrukturyzaci i Modernizaci Grzondek przypomina, ze wnioski o doplaty do samotnych poruf nalezy skladac do piontku, chyba ze por ma juz wsparcie rodziny... Lokalny soltys zdementowal plotki, jakoby traktor marki Ursus zostal widziany na randce z przyczepom samozbierajoncom... Na koniec przypominamy: kto sieje wiatr, ten zbiera kontrole z urzendu, a kto sieje rzepak, ten przynajmniej wie, po co wstal o czwartej rano.";
    static Sprite *logos[4];
    for (uint8_t i = 0; i < 4; i++)
    {
        logos[i] = storage->get_sprite(50 + i);
    }
    const Sprite* tv_off = storage->get_sprite(54);
    //
    uint32_t t = 0;
    uint8_t scene = 0;
    uint32_t scene_start_t = t;
    uint32_t first_scene_end = 120;
#if defined(PLATFORM_WINDOWS)
    while (event_state.running)
    {
        uint64_t frame_begin_ticks = SDL_GetPerformanceCounter();
#else
    while (1)
    {
#if defined(EUZEBIA3D_DEBUG_MODE)
        debugMode->begin_frame();
#endif
#endif
        painter->clear_buffer(10);
        if (scene == 1 || scene == 3 || scene == 5)
            // tv zoom in
            painter->draw_background(tv_big_background);
        else
            // tv zoom out
            painter->draw_background(tv_background);
        if (scene == 0)
        {
            painter->draw_sprite(tv_off, 91, 76, 0, 1);
            animate_curtain(painter, curtain, t, curtainLen);
            /*if (t > first_scene_end - 70)
            {
                painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);
                if (t > (first_scene_end - 60) + 45 && t <= (first_scene_end - 60) + 55)
                    painter->draw_sprite(leftHands[6], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
                else
                    painter->draw_sprite(leftHands[5], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
            }*/
            if (t < scene_start_t + 20)
            {
                painter->draw_sprite(leftHands[0], 0, 244 - (t - scene_start_t) * 5, 0, 1);
                painter->draw_sprite(rightHands[0], 168, 204 - (t - scene_start_t) * 7, 0, 1);
            }
            else if (t < scene_start_t + 30)
            {
                painter->draw_sprite(leftHands[1], -1, 78, 0, 1);
                painter->draw_sprite(rightHands[1], 160, 112, 0, 1);
            }
            else if (t < scene_start_t + 40)
            {
                painter->draw_sprite(leftHands[2], 0, 78, 0, 1);
                painter->draw_sprite(rightHands[2], 167, 89, 0, 1);
            }
            else if (t < scene_start_t + 50)
            {
                painter->draw_sprite(leftHands[0], 0, 135 + (t - (scene_start_t + 40)) * 10, 0, 1);
                painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);
            }
            else if (t < scene_start_t + 65)
            {
                painter->draw_sprite(leftHands[5], -17, 242 - (t - (scene_start_t + 50)) * 7, 0, 1);
                painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);
            }
            else
            {
                if (t > (first_scene_end - 60) + 45 && t <= (first_scene_end - 60) + 55)
                    painter->draw_sprite(leftHands[6], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
                else
                    painter->draw_sprite(leftHands[5], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);

                if (t >= first_scene_end - 40 && t < first_scene_end - 30)
                    painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 - (t - (first_scene_end - 40)) * 9, 0, 1);
                else if (t >= first_scene_end - 30 && t < first_scene_end - 10)
                    painter->draw_sprite(rightHands[4], 175 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + wave_offset(t, 350u, 0u, 2), 0, 1);
                else if (t >= first_scene_end - 10)
                    painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + (t - (first_scene_end - 10)) * 9, 0, 1);
                else
                    painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);
            }
        }
        else if (scene == 1)
        {
            painter->draw_gradient(0x01a2, 0x1ec7, &rect, UP);
            draw_grid(painter, 44, 120, 120, 240, 0x7f4f, 8, 200, 0, 0, t, 0);
            painter->print("PROGRAM WIECZORNY", 54, 50, 2, 0xffff);
            painter->print("19:20\tPrzeglond dnia", 54, 70, 1, 0xffff);
            painter->print("20:00\tWielkie kino: Wojna swiatuf", 54, 85, 1, 0xffff);
            if (t > scene_start_t + 15)
                painter->print("22:00\tNuszka gotuje", 54, 100, 1, 0xffff);
            if (t > scene_start_t + 30)
                painter->print("22:30\tProgram dla dzieci", 54, 115, 1, 0xffff);
            if (t > scene_start_t + 45)
                painter->print("00:00\tKoniec programu", 54, 130, 1, 0xffff);
            if (t - scene_start_t <= 15)
            {
                painter->draw_sprite(segment_numbers[0], 56, 36, 0, 2);
                painter->draw_sprite(segment_numbers[1], 76, 36, 0, 2);
            }
            painter->draw_sprite(logos[0], 254, 38, 0, 1);
        }
        else if (scene == 2)
        {
            painter->draw_sprite(channels[0], 91, 76, 0, 1);
            animate_curtain(painter, curtain, t, curtainLen);
            if (t < scene_start_t + 5)
                painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 - (t - scene_start_t) * 9, 0, 1);
            else if (t < scene_start_t + 25)
                painter->draw_sprite(rightHands[4], 166 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + wave_offset(t, 350u, 0u, 2), 0, 1);
            else if (t < scene_start_t + 35)
                painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + (t - (scene_start_t + 30)) * 9, 0, 1);
            else
                painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);
            if (t > scene_start_t + 45 && t <= scene_start_t + 55)
                painter->draw_sprite(leftHands[4], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
            else
                painter->draw_sprite(leftHands[3], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
        }
        else if (scene == 3)
        {
            // weather report
            if (t >= scene_start_t + 5 && t < scene_start_t + 10)
                painter->draw_plasma(plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else
            {
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
                if (t - scene_start_t <= 15)
                {
                    painter->draw_sprite(segment_numbers[0], 56, 36, 0, 2);
                    painter->draw_sprite(segment_numbers[2], 76, 36, 0, 2);
                }
                painter->draw_sprite(logos[1], 254, 38, 0, 1);
            }
        }
        else if (scene == 4)
        {
            painter->draw_sprite(channels[1], 91, 76, 0, 1);
            animate_curtain(painter, curtain, t, curtainLen);
            if (t < scene_start_t + 10)
                painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 - (t - scene_start_t) * 9, 0, 1);
            else if (t < scene_start_t + 30)
                painter->draw_sprite(rightHands[4], 166 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + wave_offset(t, 350u, 0u, 2), 0, 1);
            else if (t < scene_start_t + 40)
                painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + (t - (scene_start_t + 30)) * 9, 0, 1);
            else
                painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);
            if (t > scene_start_t + 45 && t <= scene_start_t + 55)
                painter->draw_sprite(leftHands[4], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
            else
                painter->draw_sprite(leftHands[3], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
        }
        else if (scene == 5)
        {
            // news
            if(t>=scene_start_t+5 && t<scene_start_t+10)
                painter->draw_plasma(plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else if (t >= scene_start_t + 30 && t < scene_start_t + 35)
                painter->draw_plasma(plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else if (t >= scene_start_t + 35 && t < scene_start_t + 45)
                painter->draw_rectangle(&rect,0x0000);
            else if (t >= scene_start_t + 45 && t < scene_start_t + 50)
                painter->draw_plasma(plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else
            {
                painter->draw_plasma(plasmaColors, 16, t, 2, 6, 6, 7, 6, &plasmaRect);
                painter->draw_sprite(agro_main, 130, 44, 0, 1);
                painter->draw_sprite(burak, 54, 38, 0, 1);
                painter->draw_rectangle(&bar1, 0x34b2);
                painter->draw_rectangle(&bar2, 0x3292);
                painter->print(text, 0 - (t * 3), 203, 1, 0xffff);
                painter->print("Ceny burakuf rosno!!1", 75, 185, 0, 0xffff);
                painter->draw_sprite(agro_mouth[t % agro_mouth_frame_count], 208, 79, 0, 1);
                if (t - scene_start_t <= 15)
                {
                    painter->draw_sprite(segment_numbers[0], 56, 36, 0, 2);
                    painter->draw_sprite(segment_numbers[3], 76, 36, 0, 2);
                }
                painter->draw_sprite(logos[2], 254, 38, 0, 1);
            }

        }
        else if (scene == 6)
        {
            painter->draw_sprite(channels[2], 91, 76, 0, 1);
            animate_curtain(painter, curtain, t, curtainLen);
            if (t >= scene_start_t + 10 && t < scene_start_t + 20)
                painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 - (t - (scene_start_t + 10)) * 9, 0, 1);
            else if (t >= scene_start_t + 20 && t < scene_start_t + 40)
                painter->draw_sprite(rightHands[4], 175 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + wave_offset(t, 350u, 0u, 2), 0, 1);
            else if (t >= scene_start_t + 40 && t < scene_start_t + 50)
                painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + (t - (scene_start_t + 40)) * 9, 0, 1);
            else
                painter->draw_sprite(rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);

            if (t > scene_start_t + 45 && t <= scene_start_t + 55)
                painter->draw_sprite(leftHands[4], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
            else
                painter->draw_sprite(leftHands[3], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
        }
        if (scene == 1 || scene == 3 || scene == 5)
        {
            // tv frame
            painter->draw_sprite(tv_big_frame1, 272, 20, 0, 1);
            painter->draw_sprite(tv_big_frame2, 44, 30, 0, 1);
            painter->draw_sprite(tv_big_frame3, 0, 169, 0, 1);
        }
        float qt = t * 0.02f;
        (void)qt;
#if defined(EUZEBIA3D_DEBUG_MODE)
        debugMode->show_info();
        snprintf(t_char, sizeof(t_char), "%lu", (unsigned long)t);
        painter->print(t_char, 300, 220, 1, 0xffff);
        debugMode->begin_draw_buffer();
#endif
        painter->draw_buffer();
#if defined(EUZEBIA3D_DEBUG_MODE)
        debugMode->end_draw_buffer();
#if defined(PLATFORM_WINDOWS)
        while (event_state.pause_toggle_count > 0u)
        {
            pause = !pause;
            event_state.pause_toggle_count--;
        }
#endif
        if (pause == false)
            t++;
#else
        t++;
#endif
        if (t == first_scene_end)
        {
            scene = 1;
            scene_start_t = first_scene_end + 1;
        }
        else if (t == first_scene_end + 60)
        {
            scene = 2;
            scene_start_t = first_scene_end + 61;
        }
        else if (t == first_scene_end + 120)
        {
            scene = 3;
            scene_start_t = first_scene_end + 121;
        }
        else if (t == first_scene_end + 180)
        {
            scene = 4;
            scene_start_t = first_scene_end + 181;
        }
        else if (t == first_scene_end + 240)
        {
            scene = 5;
            scene_start_t = first_scene_end + 241;
        }
        else if (t == first_scene_end + 300)
        {
            scene = 6;
            scene_start_t = first_scene_end + 301;
        }
#if defined(PLATFORM_WINDOWS)
        cap_window_frame_rate(frame_begin_ticks);
#endif
#if defined(EUZEBIA3D_DEBUG_MODE)
        debugMode->end_frame();
#endif
    }

#if defined(PLATFORM_WINDOWS)
    SDL_RemoveEventWatch(handle_window_event, &event_state);
    SDL_Quit();
    return 0;
#else
    // multicore_launch_core1(core1_main);
    return 0;
#endif
}
