
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

#include "engineApi.h"
#include "ChannelHijackDemoHelpers.h"

static e3d_EngineContext engine;
static e3d_EngineContext *engine_ctx = &engine;
static char t_char[11];
static bool pause = false;

#if defined(PLATFORM_WINDOWS)
typedef struct WindowsEventState
{
    bool running;
    unsigned int pause_toggle_count;
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

    e3d_InitEngine(engine_ctx);
    e3d_Puppet *pogodynka = e3d_Puppetteer_CreatePuppet(engine_ctx, 0);

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

    e3d_Buffer_ClearBuffer(engine_ctx, 0x1100);
    e3d_Buffer_DrawBuffer(engine_ctx);
    // tv
    const e3d_Sprite *tv_big_frame1 = engine_ctx->storage->get_sprite(1);
    const e3d_Sprite *tv_big_frame2 = engine_ctx->storage->get_sprite(2);
    const e3d_Sprite *tv_big_frame3 = engine_ctx->storage->get_sprite(3);
    const e3d_Sprite *segment_numbers[10];
    segment_numbers[0] = engine_ctx->storage->get_sprite(24);
    segment_numbers[1] = engine_ctx->storage->get_sprite(25);
    segment_numbers[2] = engine_ctx->storage->get_sprite(26);
    segment_numbers[3] = engine_ctx->storage->get_sprite(27);
    segment_numbers[4] = engine_ctx->storage->get_sprite(28);
    segment_numbers[5] = engine_ctx->storage->get_sprite(29);
    segment_numbers[6] = engine_ctx->storage->get_sprite(30);
    segment_numbers[7] = engine_ctx->storage->get_sprite(31);
    segment_numbers[8] = engine_ctx->storage->get_sprite(32);
    segment_numbers[9] = engine_ctx->storage->get_sprite(33);
    // weather report
    const e3d_Sprite *map = engine_ctx->storage->get_sprite(0);
    const e3d_Sprite *weather_1_1 = engine_ctx->storage->get_sprite(4);
    const e3d_Sprite *weather_1_2 = engine_ctx->storage->get_sprite(4);
    const e3d_Sprite *weather_1_3 = engine_ctx->storage->get_sprite(4);
    const e3d_Sprite *weather_1_4 = engine_ctx->storage->get_sprite(4);
    const e3d_Sprite *weather_1_5 = engine_ctx->storage->get_sprite(4);
    const e3d_Sprite *weather_2_1 = engine_ctx->storage->get_sprite(5);
    const e3d_Sprite *weather_2_2 = engine_ctx->storage->get_sprite(5);
    const e3d_Sprite *weather_2_3 = engine_ctx->storage->get_sprite(5);
    const e3d_Sprite *weather_2_4 = engine_ctx->storage->get_sprite(5);
    const e3d_Sprite *weather_2_5 = engine_ctx->storage->get_sprite(5);
    const e3d_Sprite *weather_3_1 = engine_ctx->storage->get_sprite(6);
    const e3d_Sprite *weather_3_2 = engine_ctx->storage->get_sprite(6);
    const e3d_Sprite *weather_3_3 = engine_ctx->storage->get_sprite(6);
    const e3d_Sprite *weather_3_4 = engine_ctx->storage->get_sprite(6);
    const e3d_Sprite *weather_3_5 = engine_ctx->storage->get_sprite(6);
    const e3d_Sprite *weather_3_6 = engine_ctx->storage->get_sprite(6);
    const e3d_Sprite *weather_4_1 = engine_ctx->storage->get_sprite(7);
    const e3d_Sprite *weather_4_2 = engine_ctx->storage->get_sprite(7);
    const e3d_Sprite *weather_4_3 = engine_ctx->storage->get_sprite(7);
    const e3d_Sprite *weather_5_1 = engine_ctx->storage->get_sprite(8);
    const e3d_Sprite *weather_5_2 = engine_ctx->storage->get_sprite(8);
    const e3d_Sprite *weather_5_3 = engine_ctx->storage->get_sprite(8);
    // news
    const e3d_Sprite *agro_main = engine_ctx->storage->get_sprite(19);
    const e3d_Sprite *burak = engine_ctx->storage->get_sprite(23);
    const e3d_Sprite *agro_mouth[4];
    agro_mouth[0] = engine_ctx->storage->get_sprite(20);
    agro_mouth[1] = engine_ctx->storage->get_sprite(21);
    agro_mouth[2] = engine_ctx->storage->get_sprite(22);
    agro_mouth[3] = engine_ctx->storage->get_sprite(21);
    const uint32_t agro_mouth_frame_hold = 3u;
    const uint32_t agro_mouth_frame_count = sizeof(agro_mouth) / sizeof(agro_mouth[0]);
    // tv
    static const e3d_Sprite *channels[3];
    for (uint8_t i = 0; i < 3; i++)
    {
        channels[i] = engine_ctx->storage->get_sprite(34 + i);
    }
#define curtainLen 20
    static const e3d_Sprite *curtain[curtainLen];
    for (uint8_t i = 0; i < curtainLen; i++)
    {
        curtain[i] = engine_ctx->storage->get_sprite(37);
    }
    const e3d_Sprite *leftHands[7];
    for (uint8_t i = 0; i < 7; i++)
    {
        leftHands[i] = engine_ctx->storage->get_sprite(38 + i);
    }
    const e3d_Sprite *rightHands[5];
    for (uint8_t i = 0; i < 5; i++)
    {
        rightHands[i] = engine_ctx->storage->get_sprite(45 + i);
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
    static e3d_Rectangle plasmaRect = {
        .x = 28,
        .y = 44,
        .height = 182,
        .width = 242,
    };
    static e3d_Rectangle bar1 = {
        .x = 15,
        .y = 38,
        .height = 20,
        .width = 280,
    };
    static e3d_Rectangle bar2 = {
        .x = 15,
        .y = 28,
        .height = 10,
        .width = 280,
    };
    static e3d_Rectangle rect = {
        .x = 44,
        .y = 28,
        .height = 182,
        .width = 242,
    };
    char *text = "PILNE: Ministerstwo Grabi i Widel uspokaja, ze tegoroczny wysyp kabaczkuf nie zagraza bespieczenstwu panstwa... Rolnicy z gminy Dolne Pole donoszo, ze kombajn pana Zdzislawa sam odmuwil pracy i zazondal urlopu pod gruszom... Ceny marchwi so stabilne, ale eksperty ostrzegajo przed panikom wsrod krolikuf..."; // Na rynku zboz lekkie poruszenie po tym, jak pszenica ozima zaczela zadawac niewygodne pytania o sens rzycia... Kolo Gospodyn Wiejskich zapowiada nowom technologie kiszenia ogurkuf w chmurze, choc starsze mieszkance nadal wolo beczke, bo przynajmniej nie wymaga aktualizaci... Wedlug raportu Instytutu Spraw Slomianych az 73 procent strachuf na wroble czuje sie nie docenionych i rozwaza kariere influenseruf pogodowych... W powiecie ziemniaczanym wykryto podejzanie okronglego buraka, sprawe bada komisja do spraw warzyw geometrycznie nie pokojoncych... Hodofcy kur apelujo o cisze nocnom po tym, jak jeden kogut przeszedl na tryb alarmu 24/7 i zaczol piac takrze w formacie stereo... Prognozy dla rolnictwa so umiarkowanie optymistyczne: bedzie padac, nie bedzie padac albo bedzie padac wtedy, kiedy nikt o to nie prosil... Gielda nawozuf zakonczyla dzien lekkim smrodem, ale analityki twierdzo, ze to naturalna korekcja rynku... W sadach rozpoczeto testy inteligentnych jablek, kture same spadajo do skrzynki, ale tylko po zaakceptowaniu regulaminu... Agencja Restrukturyzaci i Modernizaci Grzondek przypomina, ze wnioski o doplaty do samotnych poruf nalezy skladac do piontku, chyba ze por ma juz wsparcie rodziny... Lokalny soltys zdementowal plotki, jakoby traktor marki Ursus zostal widziany na randce z przyczepom samozbierajoncom... Na koniec przypominamy: kto sieje wiatr, ten zbiera kontrole z urzendu, a kto sieje rzepak, ten przynajmniej wie, po co wstal o czwartej rano.";
    static const e3d_Sprite *logos[4];
    for (uint8_t i = 0; i < 4; i++)
    {
        logos[i] = engine_ctx->storage->get_sprite(50 + i);
    }
    const e3d_Sprite *tv_off = engine_ctx->storage->get_sprite(54);
    const e3d_Sprite *tv_bug = engine_ctx->storage->get_sprite(55);
    const e3d_Sprite *hijacking_background = engine_ctx->storage->get_sprite(56);
    static const e3d_Sprite *hijacking_parts[4];
    for (uint8_t i = 0; i < 4; i++)
    {
        hijacking_parts[i] = engine_ctx->storage->get_sprite(57 + i);
    }
    const e3d_Sprite *big_bug_dark = engine_ctx->storage->get_sprite(61);
    const e3d_Sprite *big_bug = engine_ctx->storage->get_sprite(62);
    const e3d_Sprite *hijacking_break = engine_ctx->storage->get_sprite(63);
    const e3d_Sprite *hijacking = engine_ctx->storage->get_sprite(64);
    const e3d_Sprite *alert = engine_ctx->storage->get_sprite(65);
    const e3d_Sprite *alert_alternate = engine_ctx->storage->get_sprite(66);
    const e3d_Sprite *alert_small = engine_ctx->storage->get_sprite(67);
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
        e3d_Debug_BeginFrame(engine_ctx);
#endif
#endif
        e3d_Buffer_ClearBuffer(engine_ctx, 10);
        if ((scene & 1) == 1)
            // tv zoom in
            e3d_Painter_DrawImage(engine_ctx, 0);
        else
            // tv zoom out
            e3d_Painter_DrawImage(engine_ctx, 1);
        if (scene == 0)
        {
            e3d_Painter_DrawSprite(engine_ctx, tv_off, 91, 76, 0, 1);
            animate_curtain(engine_ctx, curtain, t, curtainLen);
            /*if (t > first_scene_end - 70)
            {
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);
                if (t > (first_scene_end - 60) + 45 && t <= (first_scene_end - 60) + 55)
                    e3d_Painter_DrawSprite(engine_ctx, leftHands[6], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
                else
                    e3d_Painter_DrawSprite(engine_ctx, leftHands[5], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
            }*/
            if (t < scene_start_t + 20)
            {
                e3d_Painter_DrawSprite(engine_ctx, leftHands[0], 0, 244 - (t - scene_start_t) * 5, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, rightHands[0], 168, 204 - (t - scene_start_t) * 7, 0, 1);
            }
            else if (t < scene_start_t + 30)
            {
                e3d_Painter_DrawSprite(engine_ctx, leftHands[1], -1, 78, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, rightHands[1], 160, 112, 0, 1);
            }
            else if (t < scene_start_t + 40)
            {
                e3d_Painter_DrawSprite(engine_ctx, leftHands[2], 0, 78, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, rightHands[2], 167, 89, 0, 1);
            }
            else if (t < scene_start_t + 50)
            {
                e3d_Painter_DrawSprite(engine_ctx, leftHands[0], 0, 135 + (t - (scene_start_t + 40)) * 10, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);
            }
            else if (t < scene_start_t + 65)
            {
                e3d_Painter_DrawSprite(engine_ctx, leftHands[5], -17, 242 - (t - (scene_start_t + 50)) * 7, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);
            }
            else
            {
                if (t > (first_scene_end - 60) + 45 && t <= (first_scene_end - 60) + 55)
                    e3d_Painter_DrawSprite(engine_ctx, leftHands[6], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
                else
                    e3d_Painter_DrawSprite(engine_ctx, leftHands[5], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);

                if (t >= first_scene_end - 40 && t < first_scene_end - 30)
                    e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 - (t - (first_scene_end - 40)) * 9, 0, 1);
                else if (t >= first_scene_end - 30 && t < first_scene_end - 10)
                    e3d_Painter_DrawSprite(engine_ctx, rightHands[4], 175 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + wave_offset(t, 350u, 0u, 2), 0, 1);
                else if (t >= first_scene_end - 10)
                    e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + (t - (first_scene_end - 10)) * 9, 0, 1);
                else
                    e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);
            }
        }
        else if (scene == 1)
        {
            e3d_Painter_DrawGradient(engine_ctx, 0x01a2, 0x1ec7, &rect, UP);
            draw_grid(engine_ctx, 44, 120, 120, 240, 0x7f4f, 8, 200, 0, 0, t, 0);
            e3d_Painter_Print(engine_ctx, "PROGRAM WIECZORNY", 56, 52, 2, 0x0000);
            e3d_Painter_Print(engine_ctx, "PROGRAM WIECZORNY", 54, 50, 2, 0xffff);
            e3d_Painter_Print(engine_ctx, "19:20\tPrzeglond dnia", 55, 71, 1, 0x0000);
            e3d_Painter_Print(engine_ctx, "19:20\tPrzeglond dnia", 54, 70, 1, 0xffff);
            e3d_Painter_Print(engine_ctx, "20:00\tWielkie kino: Wojna swiatuf", 55, 86, 1, 0x0000);
            e3d_Painter_Print(engine_ctx, "20:00\tWielkie kino: Wojna swiatuf", 54, 85, 1, 0xffff);
            if (t > scene_start_t + 15)
            {
                e3d_Painter_Print(engine_ctx, "22:00\tNuszka gotuje", 55, 101, 1, 0x0000);
                e3d_Painter_Print(engine_ctx, "22:00\tNuszka gotuje", 54, 100, 1, 0xffff);
            }
            if (t > scene_start_t + 30)
            {
                e3d_Painter_Print(engine_ctx, "22:30\tProgram dla dzieci", 55, 116, 1, 0x0000);
                e3d_Painter_Print(engine_ctx, "22:30\tProgram dla dzieci", 54, 115, 1, 0xffff);
            }
            if (t > scene_start_t + 45)
            {
                e3d_Painter_Print(engine_ctx, "00:00\tKoniec programu", 55, 131, 1, 0x0000);
                e3d_Painter_Print(engine_ctx, "00:00\tKoniec programu", 54, 130, 1, 0xffff);
            }
            if (t - scene_start_t <= 15)
            {
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[0], 56, 36, 0, 2);
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[1], 76, 36, 0, 2);
            }
            e3d_Painter_DrawSprite(engine_ctx, logos[0], 254, 38, 0, 1);
        }
        else if (scene == 2)
        {
            e3d_Painter_DrawSprite(engine_ctx, channels[0], 91, 76, 0, 1);
            animate_curtain(engine_ctx, curtain, t, curtainLen);
            if (t < scene_start_t + 5)
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 - (t - scene_start_t) * 9, 0, 1);
            else if (t < scene_start_t + 25)
                e3d_Painter_DrawSprite(engine_ctx, rightHands[4], 166 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + wave_offset(t, 350u, 0u, 2), 0, 1);
            else if (t < scene_start_t + 35)
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + (t - (scene_start_t + 30)) * 9, 0, 1);
            else
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);
            if (t > scene_start_t + 45 && t <= scene_start_t + 55)
                e3d_Painter_DrawSprite(engine_ctx, leftHands[4], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
            else
                e3d_Painter_DrawSprite(engine_ctx, leftHands[3], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
        }
        else if (scene == 3)
        {
            // weather report
            if (t >= scene_start_t + 5 && t < scene_start_t + 10)
                e3d_Painter_DrawPlasma(engine_ctx, plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else
            {
                e3d_Painter_DrawSprite(engine_ctx, map, 44, 1, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_1_1, 66, 70, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_1_2, 77, 88, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_1_3, 82, 112, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_1_4, 108, 136, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_1_5, 106, 100, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_2_1, 126, 76, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_2_2, 134, 35, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_2_3, 169, 56, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_2_4, 212, 47, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_2_5, 214, 75, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_3_1, 150, 114, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_3_2, 178, 100, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_3_3, 142, 154, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_3_4, 204, 128, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_3_5, 158, 160, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_3_6, 193, 160, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_4_1, 100, 55, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_4_2, 174, 33, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_4_3, 52, 112, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_5_1, 114, 184, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_5_2, 178, 140, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, weather_5_3, 244, 124, 0, 1);
                e3d_Puppetteer_Perform(engine_ctx, pogodynka, t);
                if (t - scene_start_t <= 15)
                {
                    e3d_Painter_DrawSprite(engine_ctx, segment_numbers[0], 56, 36, 0, 2);
                    e3d_Painter_DrawSprite(engine_ctx, segment_numbers[2], 76, 36, 0, 2);
                }
                e3d_Painter_DrawSprite(engine_ctx, logos[1], 254, 38, 0, 1);
            }
        }
        else if (scene == 4)
        {
            e3d_Painter_DrawSprite(engine_ctx, channels[1], 91, 76, 0, 1);
            animate_curtain(engine_ctx, curtain, t, curtainLen);
            if (t < scene_start_t + 10)
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 - (t - scene_start_t) * 9, 0, 1);
            else if (t < scene_start_t + 30)
                e3d_Painter_DrawSprite(engine_ctx, rightHands[4], 166 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + wave_offset(t, 350u, 0u, 2), 0, 1);
            else if (t < scene_start_t + 40)
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + (t - (scene_start_t + 30)) * 9, 0, 1);
            else
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);
            if (t > scene_start_t + 45 && t <= scene_start_t + 55)
                e3d_Painter_DrawSprite(engine_ctx, leftHands[4], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
            else
                e3d_Painter_DrawSprite(engine_ctx, leftHands[3], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
        }
        else if (scene == 5)
        {
            // news
            if (t >= scene_start_t + 5 && t < scene_start_t + 10)
                e3d_Painter_DrawPlasma(engine_ctx, plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else if (t >= scene_start_t + 30 && t < scene_start_t + 35)
                e3d_Painter_DrawPlasma(engine_ctx, plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else if (t >= scene_start_t + 35 && t < scene_start_t + 45)
                e3d_Painter_DrawRectangle(engine_ctx, &rect, 0x0000);
            else if (t >= scene_start_t + 45 && t < scene_start_t + 50)
                e3d_Painter_DrawPlasma(engine_ctx, plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else
            {
                e3d_Painter_DrawPlasma(engine_ctx, plasmaColors, 16, t, 2, 6, 6, 7, 6, &plasmaRect);
                e3d_Painter_DrawSprite(engine_ctx, agro_main, 130, 44, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, burak, 54, 38, 0, 1);
                e3d_Painter_DrawRectangle(engine_ctx, &bar1, 0x34b2);
                e3d_Painter_DrawRectangle(engine_ctx, &bar2, 0x3292);
                e3d_Painter_Print(engine_ctx, text, 0 - (t * 3), 203, 1, 0xffff);
                e3d_Painter_Print(engine_ctx, "Ceny burakuf rosno!!1", 75, 185, 0, 0xffff);
                e3d_Painter_DrawSprite(engine_ctx, agro_mouth[t % agro_mouth_frame_count], 208, 79, 0, 1);
                if (t - scene_start_t <= 15)
                {
                    e3d_Painter_DrawSprite(engine_ctx, segment_numbers[0], 56, 36, 0, 2);
                    e3d_Painter_DrawSprite(engine_ctx, segment_numbers[3], 76, 36, 0, 2);
                }
                e3d_Painter_DrawSprite(engine_ctx, logos[2], 254, 38, 0, 1);
            }
        }
        else if (scene == 6)
        {
            e3d_Painter_DrawSprite(engine_ctx, channels[2], 91, 76, 0, 1);
            animate_curtain(engine_ctx, curtain, t, curtainLen);
            if (t >= scene_start_t + 10 && t < scene_start_t + 20)
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 - (t - (scene_start_t + 10)) * 9, 0, 1);
            else if (t >= scene_start_t + 20 && t < scene_start_t + 40)
                e3d_Painter_DrawSprite(engine_ctx, rightHands[4], 175 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + wave_offset(t, 350u, 0u, 2), 0, 1);
            else if (t >= scene_start_t + 40 && t < scene_start_t + 50)
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + (t - (scene_start_t + 40)) * 9, 0, 1);
            else
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);

            if (t > scene_start_t + 45 && t <= scene_start_t + 55)
                e3d_Painter_DrawSprite(engine_ctx, leftHands[4], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
            else
                e3d_Painter_DrawSprite(engine_ctx, leftHands[3], -22 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 147 + wave_offset(t, 350u, TABLE_SIZE / 2u, 2), 0, 1);
        }
        else if (scene == 7)
        {
            if (t - scene_start_t <= 15)
            {
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[0], 56, 36, 0, 2);
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[4], 76, 36, 0, 2);
            }
            // bug
            if (t < scene_start_t + 15)
            {
                e3d_Painter_DrawSprite(engine_ctx, tv_bug, 44, 1, 0, 1);
                e3d_Painter_DrawSprite(engine_ctx, logos[3], 254, 38, 0, 1);
            }
            else if (t >= scene_start_t + 15 && t < scene_start_t + 20)
                e3d_Painter_DrawPlasma(engine_ctx, plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else if (t >= scene_start_t + 45 && t < scene_start_t + 50)
                e3d_Painter_DrawPlasma(engine_ctx, plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else if (t >= scene_start_t + 70 && t < scene_start_t + 75)
                e3d_Painter_DrawPlasma(engine_ctx, plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else if (t >= scene_start_t + 75 && t < scene_start_t + 90)
                e3d_Painter_DrawSprite(engine_ctx, hijacking_break, 44, 1, 0, 1);
            else if (t >= scene_start_t + 90 && t < scene_start_t + 95)
                e3d_Painter_DrawPlasma(engine_ctx, plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else
            {
                e3d_Painter_DrawSprite(engine_ctx, hijacking_background, 44, 1, 0, 1);
                if (t >= scene_start_t + 35)
                    e3d_Painter_DrawSprite(engine_ctx, hijacking_parts[0], 44, 31, 0, 1);
                if (t >= scene_start_t + 65)
                    e3d_Painter_DrawSprite(engine_ctx, hijacking_parts[3], 152, 105, 0, 1);
                if (t >= scene_start_t + 95)
                    e3d_Painter_DrawSprite(engine_ctx, hijacking_parts[2], 44, 121, 0, 1);
                if (t >= scene_start_t + 125)
                    e3d_Painter_DrawSprite(engine_ctx, hijacking_parts[1], 164, 30, 0, 1);
                if (t >= scene_start_t + 170)
                    e3d_Painter_DrawSprite(engine_ctx, big_bug, 112, 53, 0, 1);
                else
                    e3d_Painter_DrawSprite(engine_ctx, big_bug_dark, 112, 62, 0, 1);
            }
            if (t >= scene_start_t + 100 && t < scene_start_t + 115)
            {
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[1], 56, 36, 0, 2);
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[0], 76, 36, 0, 2);
            }
            else if (t >= scene_start_t + 90 && t < scene_start_t + 100)
            {
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[0], 56, 36, 0, 2);
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[9], 76, 36, 0, 2);
            }
            else if (t >= scene_start_t + 75 && t < scene_start_t + 90)
            {
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[0], 56, 36, 0, 2);
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[8], 76, 36, 0, 2);
            }
            else if (t >= scene_start_t + 60 && t < scene_start_t + 75)
            {
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[0], 56, 36, 0, 2);
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[7], 76, 36, 0, 2);
            }
            else if (t >= scene_start_t + 45 && t < scene_start_t + 60)
            {
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[0], 56, 36, 0, 2);
                e3d_Painter_DrawSprite(engine_ctx, segment_numbers[6], 76, 36, 0, 2);
            }
        }
        else if (scene == 8)
        {
            e3d_Painter_DrawSprite(engine_ctx, hijacking, 91, 76, 0, 1);
            animate_curtain(engine_ctx, curtain, t, curtainLen);
            if (t >= scene_start_t + 10 && t < scene_start_t + 20)
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 - (t - (scene_start_t + 10)) * 9, 0, 1);
            else if (t >= scene_start_t + 20 && t < scene_start_t + 70)
                e3d_Painter_DrawSprite(engine_ctx, rightHands[4], 175 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + wave_offset(t, 350u, 0u, 2), 0, 1);
            else if (t >= scene_start_t + 70 && t < scene_start_t + 80)
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 0 + (t - (scene_start_t + 70)) * 9, 0, 1);
            else
                e3d_Painter_DrawSprite(engine_ctx, rightHands[3], 165 + wave_offset(t, 350u, -TABLE_SIZE / 2u, 2), 90 + wave_offset(t, 350u, 0u, 2), 0, 1);
        }
        else if (scene == 9)
        {
            e3d_Painter_DrawSprite(engine_ctx, hijacking_background, 44, 1, 0, 1);
            e3d_Painter_DrawSprite(engine_ctx, hijacking_parts[0], 44, 31, 0, 1);
            e3d_Painter_DrawSprite(engine_ctx, hijacking_parts[3], 152, 105, 0, 1);
            e3d_Painter_DrawSprite(engine_ctx, hijacking_parts[2], 44, 121, 0, 1);
            e3d_Painter_DrawSprite(engine_ctx, hijacking_parts[1], 164, 30, 0, 1);
            if (t >= scene_start_t + 30)
                e3d_Painter_DrawSprite(engine_ctx, big_bug, 112, 53, 0, 1);
            else
                e3d_Painter_DrawSprite(engine_ctx, big_bug_dark, 112, 62, 0, 1);
            if (t >= scene_start_t + 45 && t < scene_start_t + 50)
                e3d_Painter_DrawPlasma(engine_ctx, plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else if (t >= scene_start_t + 50 && t < scene_start_t + 80)
                e3d_Painter_DrawSprite(engine_ctx, alert, 44, 1, 0, 1);
            if (t >= scene_start_t + 80 && t < scene_start_t + 85)
                e3d_Painter_DrawPlasma(engine_ctx, plasmaNoiseColors, 4, t, 2, 3, 4, 5, 3, &plasmaRect);
            else if (t >= scene_start_t + 85)
                e3d_Painter_DrawSprite(engine_ctx, alert_alternate, 44, 1, 0, 1);
        }
        else if (scene == 10)
        {
            e3d_Painter_DrawSprite(engine_ctx, alert_small, 91, 76, 0, 1);
            animate_curtain(engine_ctx, curtain, t, curtainLen);
        }
        if ((scene & 1) == 1)
        {
            // tv frame
            e3d_Painter_DrawSprite(engine_ctx, tv_big_frame1, 272, 20, 0, 1);
            e3d_Painter_DrawSprite(engine_ctx, tv_big_frame2, 44, 30, 0, 1);
            e3d_Painter_DrawSprite(engine_ctx, tv_big_frame3, 0, 169, 0, 1);
        }
        float qt = t * 0.02f;
        (void)qt;
#if defined(EUZEBIA3D_DEBUG_MODE)
        e3d_Debug_ShowInfo(engine_ctx);
        snprintf(t_char, sizeof(t_char), "%lu", (unsigned long)t);
        e3d_Painter_Print(engine_ctx, t_char, 300, 220, 1, 0xffff);
        e3d_Debug_BeginDrawBuffer(engine_ctx);
#endif
        e3d_Buffer_DrawBuffer(engine_ctx);
#if defined(EUZEBIA3D_DEBUG_MODE)
        e3d_Debug_EndDrawBuffer(engine_ctx);
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
        else if (t == first_scene_end + 360)
        {
            scene = 7;
            scene_start_t = first_scene_end + 361;
        }
        else if (t == first_scene_end + 475)
        {
            scene = 8;
            scene_start_t = first_scene_end + 476;
        }
        else if (t == first_scene_end + 555)
        {
            scene = 9;
            scene_start_t = first_scene_end + 556;
        }
        else if (t == first_scene_end + 655)
        {
            scene = 10;
            scene_start_t = first_scene_end + 656;
        }
#if defined(PLATFORM_WINDOWS)
        cap_window_frame_rate(frame_begin_ticks);
#endif
#if defined(EUZEBIA3D_DEBUG_MODE)
        e3d_Debug_EndFrame(engine_ctx);
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
