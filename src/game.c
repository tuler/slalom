#include "draw.h"
#include "game.h"

#define abs(x) ((x) < 0 ? -(x) : (x))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

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
        .skier = {.x = 128.0f, .y = 0.0f, .angle = 1, .sx = 0.0f, .sy = 1.0f},
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

void game_move_left(struct Game *game)
{
    game->skier.angle = max(game->skier.angle - 1, -4);
    if (game->skier.angle == 0)
    {
        game->skier.angle = -1;
    }
    game->skier.sx = (abs(game->skier.angle) - 1) * 0.5;
    game->skier.sx = game->skier.angle < 0 ? -game->skier.sx : game->skier.sx;
    game->skier.sy = (4 - abs(game->skier.angle)) * 0.5;
    riv_printf("sy: %f\n", game->skier.sy);
}

void game_move_right(struct Game *game)
{
    game->skier.angle = min(game->skier.angle + 1, 4);
    if (game->skier.angle == 0)
    {
        game->skier.angle = 1;
    }
    game->skier.sx = (abs(game->skier.angle) - 1) * 0.5;
    game->skier.sx = game->skier.angle < 0 ? -game->skier.sx : game->skier.sx;
    game->skier.sy = (4 - abs(game->skier.angle)) * 0.5;
    riv_printf("sy: %f\n", game->skier.sy);
}

void game_update(struct Game *game)
{
    // update x position
    game->skier.x = min(max(game->skier.x + game->skier.sx, 0.0f), 256.0f - 16.0f);

    // update y position
    game->skier.y += game->skier.sy;
}

void game_start(struct Game *game)
{
    do
    {
        if (riv->keys[RIV_GAMEPAD_LEFT].press)
        {
            game_move_left(game);
        }
        if (riv->keys[RIV_GAMEPAD_RIGHT].press)
        {
            game_move_right(game);
        }

        game_update(game);

        riv_clear(RIV_COLOR_WHITE);
        // draw_palette();
        draw_game(game);

    } while (riv_present());
}
