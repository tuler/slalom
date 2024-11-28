#include <stdio.h>
#include <riv.h>

#include "draw.h"
#include "game.h"

uint64_t img_id = 0;

#define BASE_Y 48

void draw_init(const char *sprite)
{
    riv_load_palette(sprite, 16);
    img_id = riv_make_image(sprite, 0);
}

void draw_palette()
{
    for (int i = 0; i < 32; i++)
    {
        riv_draw_rect_fill(i * 8, 0, 8, 8, i);
    }
}

void draw_gates(uint64_t gates_left)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%lu", gates_left);
    riv_draw_text(buffer, RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 128, 16, 2, RIV_COLOR_BLACK);
}

void draw_time_left(uint64_t time_left)
{
    char buffer[256];
    uint64_t minutes = time_left / (60 * 1000);
    uint64_t seconds = (time_left % (60 * 1000)) / 1000;
    uint64_t ms = time_left % 1000;

    // format milliseconds to minutes, seconds and milliseconds
    if (minutes > 0)
    {
        snprintf(buffer, sizeof(buffer), "%lu:%02lu.%03lu", minutes, seconds, ms);
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "%lu.%03lu", seconds, ms);
    }

    riv_draw_text(buffer, RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 128, 32, 1, RIV_COLOR_BLACK);
}

/**
 * color = 0: red
 * color = 1: blue
 */
void draw_gate(int64_t x0, int64_t y0, uint64_t w, uint64_t color)
{
    uint64_t sx0 = 40;
    uint64_t sy0 = color * 16;
    riv_draw_image_rect(img_id, x0 - w / 2, y0, 8, 16, sx0, sy0, PIXEL_WIDTH, 1);
    riv_draw_image_rect(img_id, x0 + w / 2, y0, 8, 16, sx0, sy0, PIXEL_WIDTH, 1);
}

void draw_skier(int64_t x0, int64_t y0, int64_t angle)
{
    uint64_t sx0 = (4 - abs(angle)) * 8;
    int64_t r = angle < 0 ? -1 : 1;
    riv_draw_image_rect(img_id, x0, y0, 8, 20, sx0, 0, r * PIXEL_WIDTH, 1);
}

void draw_tree(int64_t x0, int64_t y0, uint64_t color, int64_t mirror)
{
    uint64_t sx0 = 48 + (color * 16);
    riv_draw_image_rect(img_id, x0, y0, 16, 32, sx0, 0, PIXEL_WIDTH * mirror, 1);
}

void draw_mogul(int64_t x0, int64_t y0, uint64_t color, int64_t mirror)
{
    uint64_t sx0 = (color * 16);
    riv_draw_image_rect(img_id, x0, y0, 16, 8, sx0, 24, PIXEL_WIDTH * mirror, 1);
}

void draw_game(struct Game *game)
{
    int64_t dy = -(int64_t)game->skier.y;
    draw_skier((int64_t)game->skier.x, BASE_Y, game->skier.angle);

    for (size_t i = 0; i < game->gates_count; i++)
    {
        int64_t y = BASE_Y + dy + game->gate_start + (i * game->gate_spacing);
        uint64_t color = i == game->gates_count - 1 ? 0 : 1;
        draw_gate(game->gates[i], y, game->gate_width, color);
    }

    for (size_t i = 0; i < game->trees_count; i++)
    {
        draw_tree(game->trees[i].x, game->trees[i].y + BASE_Y + dy, game->trees[i].color, game->trees[i].mirror);
    }

    for (size_t i = 0; i < game->moguls_count; i++)
    {
        draw_mogul(game->moguls[i].x, game->moguls[i].y + BASE_Y + dy, game->moguls[i].color, game->moguls[i].mirror);
    }

    draw_gates(game->gates_count - game->next_gate);
    draw_time_left(game->time_left);
}
