#include "draw.h"
#include "game.h"

struct Game game_create(uint64_t game_width)
{
    struct Game game = {
        .game_width = game_width,
    };
    return game;
}

void draw_palette()
{
    for (int i = 0; i < 32; i++)
    {
        riv_draw_rect_fill(i * 8, 0, 8, 8, i);
    }
}

void game_start(struct Game *game)
{
    do
    {
        riv_clear(RIV_COLOR_WHITE);

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

        draw_palette();

        // draw_map();
        // draw_skier();
        // draw_gate();
        // draw_tree();
        // riv_flip();
    } while (riv_present());
}
