#ifndef CHANNEL_HIJACK_DEMO_GRID_H
#define CHANNEL_HIJACK_DEMO_GRID_H

#include <stdint.h>

#include "engineApi.h"

typedef enum {
    GRID_DIRECTION_DOWN = 0,
    GRID_DIRECTION_UP,
    GRID_DIRECTION_LEFT,
    GRID_DIRECTION_RIGHT,
} GridDirection;

typedef struct {
    int16_t x;
    int16_t y;
    uint16_t height;
    uint16_t width;
    uint16_t color;
    uint8_t line_count;
    uint8_t perspective_offset;
    int16_t offset_x;
    int16_t offset_y;
    GridDirection direction;
} GridConfig;

void draw_grid(e3d_EngineContext *engine_ctx, const GridConfig *config,
               uint32_t frame);

#endif
