#include "grid.h"

#define EUZEBIA3D_DISPLAY_HEIGHT 240

static int16_t grid_lerp(int16_t start, int16_t end, uint8_t line, uint8_t linesNum)
{
    if (linesNum <= 1u)
        return (int16_t)(start + (((int32_t)end - start) >> 1));

    return (int16_t)(start + (((int32_t)line * ((int32_t)end - start)) / (linesNum - 1u)));
}

static int16_t grid_lerp_position(int16_t start, int16_t end, uint16_t position, uint16_t span)
{
    if (span <= 1u)
        return (int16_t)(start + (((int32_t)end - start) >> 1));

    return (int16_t)(start + (((int32_t)position * ((int32_t)end - start)) / (span - 1u)));
}

static e3d_Point grid_lerp_point(e3d_Point start, e3d_Point end, uint16_t position, uint16_t span)
{
    e3d_Point point = {
        .x = grid_lerp_position(start.x, end.x, position, span),
        .y = grid_lerp_position(start.y, end.y, position, span),
    };
    return point;
}

static uint16_t grid_positive_mod(int value, uint16_t divisor)
{
    int remainder = value % divisor;
    if (remainder < 0)
        remainder += divisor;

    return (uint16_t)remainder;
}

static uint16_t grid_wrapped_position(uint16_t span, uint8_t line, uint8_t linesNum, int t, uint8_t reverse)
{
    uint16_t step = span / linesNum;
    if (step == 0u)
        step = 1u;

    uint16_t phase = grid_positive_mod(t, step);
    uint16_t base = (uint16_t)(((uint32_t)line * span) / linesNum);
    int position = reverse
                       ? ((phase == 0u) ? (int)base : ((int)base + (int)step - (int)phase))
                       : ((int)base + (int)phase);
    return grid_positive_mod(position, span);
}

static int16_t grid_screen_y_to_buffer_y(int16_t screenY)
{
    return (int16_t)((EUZEBIA3D_DISPLAY_HEIGHT - 1) - screenY);
}

static e3d_Point grid_screen_point_to_buffer(e3d_Point point)
{
    point.y = grid_screen_y_to_buffer_y(point.y);
    return point;
}

static uint8_t grid_clip_out_code(int32_t x, int32_t y, int32_t left, int32_t top, int32_t right, int32_t bottom)
{
    uint8_t code = 0u;
    if (x < left)
        code |= 1u;
    else if (x > right)
        code |= 2u;

    if (y < top)
        code |= 4u;
    else if (y > bottom)
        code |= 8u;

    return code;
}

static uint8_t grid_clip_line_to_rect(e3d_Point *start, e3d_Point *end, int16_t left, int16_t top, int16_t right, int16_t bottom)
{
    int32_t x0 = start->x;
    int32_t y0 = start->y;
    int32_t x1 = end->x;
    int32_t y1 = end->y;
    uint8_t code0 = grid_clip_out_code(x0, y0, left, top, right, bottom);
    uint8_t code1 = grid_clip_out_code(x1, y1, left, top, right, bottom);

    while (1)
    {
        if ((code0 | code1) == 0u)
        {
            start->x = (int16_t)x0;
            start->y = (int16_t)y0;
            end->x = (int16_t)x1;
            end->y = (int16_t)y1;
            return 1u;
        }
        if ((code0 & code1) != 0u)
        {
            return 0u;
        }

        uint8_t outside_code = (code0 != 0u) ? code0 : code1;
        int32_t x = 0;
        int32_t y = 0;

        if ((outside_code & 4u) != 0u)
        {
            if (y1 == y0)
                return 0u;

            x = x0 + ((x1 - x0) * (top - y0)) / (y1 - y0);
            y = top;
        }
        else if ((outside_code & 8u) != 0u)
        {
            if (y1 == y0)
                return 0u;

            x = x0 + ((x1 - x0) * (bottom - y0)) / (y1 - y0);
            y = bottom;
        }
        else if ((outside_code & 2u) != 0u)
        {
            if (x1 == x0)
                return 0u;

            y = y0 + ((y1 - y0) * (right - x0)) / (x1 - x0);
            x = right;
        }
        else
        {
            if (x1 == x0)
                return 0u;

            y = y0 + ((y1 - y0) * (left - x0)) / (x1 - x0);
            x = left;
        }

        if (outside_code == code0)
        {
            x0 = x;
            y0 = y;
            code0 = grid_clip_out_code(x0, y0, left, top, right, bottom);
        }
        else
        {
            x1 = x;
            y1 = y;
            code1 = grid_clip_out_code(x1, y1, left, top, right, bottom);
        }
    }
}

static void grid_draw_horizontal_line(e3d_EngineContext *engine_ctx, e3d_Point start, e3d_Point end, int16_t left, int16_t top, int16_t right, int16_t bottom, uint16_t color)
{
    if (start.y < top || start.y > bottom)
        return;

    int16_t start_x = start.x;
    int16_t end_x = end.x;
    if (start_x > end_x)
    {
        int16_t swap = start_x;
        start_x = end_x;
        end_x = swap;
    }

    if (end_x < left || start_x > right)
        return;

    if (start_x < left)
        start_x = left;
    if (end_x > right)
        end_x = right;

    e3d_Point clipped_start = {
        .x = start_x,
        .y = start.y,
    };
    e3d_Point clipped_end = {
        .x = end_x,
        .y = start.y,
    };
    clipped_start = grid_screen_point_to_buffer(clipped_start);
    clipped_end = grid_screen_point_to_buffer(clipped_end);
    e3d_Painter_DrawLine(engine_ctx, &clipped_start, &clipped_end, color);
}

static void draw_grid_values(e3d_EngineContext *engine_ctx, int16_t x, int16_t y, uint16_t height, uint16_t width, uint16_t color, uint8_t linesNum, uint8_t offset, int16_t offsetX, int16_t offsetY, int t, uint8_t direction)
{
    if (engine_ctx == NULL || linesNum == 0u || width == 0u || height == 0u)
        return;

    int16_t clipLeft = x;
    int16_t clipTop = y;
    int16_t clipRight = (int16_t)(x + width - 1u);
    int16_t clipBottom = (int16_t)(y + height - 1u);
    int16_t gridX = (int16_t)(x + offsetX);
    int16_t gridY = (int16_t)(y + offsetY);
    e3d_Point topLeft = {
        .x = gridX,
        .y = gridY,
    };
    e3d_Point topRight = {
        .x = (int16_t)(gridX + width - 1u),
        .y = gridY,
    };
    e3d_Point bottomLeft = {
        .x = gridX,
        .y = (int16_t)(gridY + height - 1u),
    };
    e3d_Point bottomRight = {
        .x = (int16_t)(gridX + width - 1u),
        .y = (int16_t)(gridY + height - 1u),
    };

    if (direction == GRID_DIRECTION_DOWN)
    {
        bottomLeft.x -= offset;
        bottomRight.x += offset;
    }
    else if (direction == GRID_DIRECTION_UP)
    {
        topLeft.x -= offset;
        topRight.x += offset;
    }
    else if (direction == GRID_DIRECTION_LEFT)
    {
        topLeft.y -= offset;
        bottomLeft.y += offset;
    }
    else if (direction == GRID_DIRECTION_RIGHT)
    {
        topRight.y -= offset;
        bottomRight.y += offset;
    }

    uint8_t moveVertical = direction == GRID_DIRECTION_LEFT || direction == GRID_DIRECTION_RIGHT;
    for (uint8_t i = 0; i < linesNum; i++)
    {
        e3d_Point top;
        e3d_Point bottom;
        if (moveVertical)
        {
            uint16_t position = grid_wrapped_position(width, i, linesNum, t, direction == GRID_DIRECTION_LEFT);
            top = grid_lerp_point(topLeft, topRight, position, width);
            bottom = grid_lerp_point(bottomLeft, bottomRight, position, width);
        }
        else
        {
            top.x = grid_lerp(topLeft.x, topRight.x, i, linesNum);
            top.y = grid_lerp(topLeft.y, topRight.y, i, linesNum);
            bottom.x = grid_lerp(bottomLeft.x, bottomRight.x, i, linesNum);
            bottom.y = grid_lerp(bottomLeft.y, bottomRight.y, i, linesNum);
        }
        if (grid_clip_line_to_rect(&top, &bottom, clipLeft, clipTop, clipRight, clipBottom))
        {
            top = grid_screen_point_to_buffer(top);
            bottom = grid_screen_point_to_buffer(bottom);
            e3d_Painter_DrawLine(engine_ctx, &top, &bottom, color);
        }
    }

    uint8_t moveHorizontal = direction == GRID_DIRECTION_DOWN || direction == GRID_DIRECTION_UP;
    for (uint8_t i = 0; i < linesNum; i++)
    {
        e3d_Point left;
        e3d_Point right;
        if (moveHorizontal)
        {
            uint16_t position = grid_wrapped_position(height, i, linesNum, t, direction == GRID_DIRECTION_UP);
            left = grid_lerp_point(topLeft, bottomLeft, position, height);
            right = grid_lerp_point(topRight, bottomRight, position, height);
        }
        else
        {
            left.x = grid_lerp(topLeft.x, bottomLeft.x, i, linesNum);
            left.y = grid_lerp(topLeft.y, bottomLeft.y, i, linesNum);
            right.x = grid_lerp(topRight.x, bottomRight.x, i, linesNum);
            right.y = grid_lerp(topRight.y, bottomRight.y, i, linesNum);
        }
        if (left.y == right.y)
        {
            grid_draw_horizontal_line(engine_ctx, left, right, clipLeft, clipTop, clipRight, clipBottom, color);
        }
        else if (grid_clip_line_to_rect(&left, &right, clipLeft, clipTop, clipRight, clipBottom))
        {
            left = grid_screen_point_to_buffer(left);
            right = grid_screen_point_to_buffer(right);
            e3d_Painter_DrawLine(engine_ctx, &left, &right, color);
        }
    }
}

void draw_grid(e3d_EngineContext *engine_ctx, const GridConfig *config,
               uint32_t frame)
{
    if (config == NULL)
    {
        return;
    }

    draw_grid_values(engine_ctx, config->x, config->y, config->height,
                     config->width, config->color, config->line_count,
                     config->perspective_offset, config->offset_x,
                     config->offset_y, (int)frame, (uint8_t)config->direction);
}
