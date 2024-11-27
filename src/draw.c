#include <riv.h>

#include "color.h"
#include "draw.h"
#include "game.h"

uint64_t img_id = 0;

#define BASE_Y 48
#define abs(x) ((x) < 0 ? -(x) : (x))

void draw_init(const char *sprite)
{
    riv_load_palette(sprite, 16);
    // color_setup();
    img_id = riv_make_image(sprite, 0);
}

void draw_tree(int64_t x0, int64_t y0, uint64_t color)
{
    uint64_t sx0 = 48 + (color * 16);
    riv_draw_image_rect(img_id, x0, y0, 16, 32, sx0, 0, 2, 1);
}

/**
 * color = 0: red
 * color = 1: blue
 */
void draw_gate(int64_t x0, int64_t y0, uint64_t w, uint64_t color)
{
    uint64_t sx0 = 40;
    uint64_t sy0 = color * 16;
    riv_draw_image_rect(img_id, x0 - w / 2, y0, 8, 16, sx0, sy0, 2, 1);
    riv_draw_image_rect(img_id, x0 + w / 2, y0, 8, 16, sx0, sy0, 2, 1);
}

void draw_skier(int64_t x0, int64_t y0, int64_t angle)
{
    uint64_t sx0 = (4 - abs(angle)) * 8;
    int64_t mw = angle < 0 ? -2 : 2;
    riv_draw_image_rect(img_id, x0, y0, 8, 20, sx0, 0, mw, 1);
}

void draw_mogul(int64_t x0, int64_t y0, uint64_t color)
{
    uint64_t sx0 = (color * 16);
    riv_draw_image_rect(img_id, x0, y0, 16, 8, sx0, 24, 2, 1);
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
        draw_tree(game->trees[i].x, game->trees[i].y + BASE_Y + dy, game->trees[i].color);
    }

    for (size_t i = 0; i < game->moguls_count; i++)
    {
        draw_mogul(game->moguls[i].x, game->moguls[i].y + BASE_Y + dy, game->moguls[i].color);
    }
}
