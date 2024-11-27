#include "draw.h"
#include "game.h"

struct Game game_create(uint64_t gate_width)
{
    struct Game game = {
        .gate_spacing = 96,
        .gate_start = 64,
        .gate_width = gate_width,
        .gates = {
            120,
            145,
            120,
            80,
            60},
        .gates_count = 5,
        .moguls = {
            {.x = 20, .y = 100, .color = 0, .mirror = 1},
            {.x = 220, .y = 116, .color = 0, .mirror = 1},
        },
        .moguls_count = 2,
        .skier = {.x = 128, .y = 0, .angle = 1, .sx = 0, .sy = 1},
        .trees = {
            {.x = 10, .y = 30, .color = 0, .mirror = 1},
            {.x = 220, .y = 130, .color = 1, .mirror = 1},
        },
        .trees_count = 2};
    return game;
}

void draw_palette()
{
    for (int i = 0; i < 32; i++)
    {
        riv_draw_rect_fill(i * 8, 0, 8, 8, i);
    }
}

void game_update(struct Game *game)
{
    // update x position
    game->skier.x += game->skier.sx;

    // update y position
    game->skier.y += game->skier.sy;
}

void game_start(struct Game *game)
{
    do
    {
        riv_clear(RIV_COLOR_WHITE);

        game_update(game);

        /*
        draw_tree(20, 20, 0);
        draw_tree(60, 20, 1);
        draw_tree(100, 20, 2);
        draw_tree(140, 20, 3);
        draw_gate(100, 100, 32, 1);
        draw_gate(128, 160, 32, 0);

        draw_skier(80, 56, 0);
        draw_skier(100, 56, 1);
        draw_skier(120, 56, 2);
        draw_skier(140, 56, 3);
        draw_skier(160, 56, 4);

        draw_mogul(110, 200, 0);
        draw_mogul(140, 220, 1);
        */

        draw_palette();
        draw_game(game);

    } while (riv_present());
}
