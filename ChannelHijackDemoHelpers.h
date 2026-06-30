#ifndef CHANNELHIJACKDEMOHELPERS_H
#define CHANNELHIJACKDEMOHELPERS_H

#include <stdint.h>

#include "IPainter.h"

#define GRID_MOVE_DOWN 0u
#define GRID_MOVE_UP 1u
#define GRID_MOVE_LEFT 2u
#define GRID_MOVE_RIGHT 3u

#if defined(PLATFORM_WINDOWS)
int require_pointer(const void *pointer, const char *name);
int process_window_events(void);
void cap_window_frame_rate(uint64_t frame_begin_ticks);
#endif

void animate_curtain(const IPainter *painter, Sprite *curtain[], int t, uint16_t bands);
void draw_grid(const IPainter *painter, int16_t x, int16_t y, uint16_t height, uint16_t width, uint16_t color, uint8_t linesNum, uint8_t offset, int16_t offsetX, int16_t offsetY, int t, uint8_t direction);

#endif
