#include "collision.h"
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
        .gates_missed = 0,
        .moguls = {
            {.x = 20, .y = 100, .color = 0, .mirror = 1},
            {.x = 220, .y = 116, .color = 0, .mirror = 1},
        },
        .moguls_count = 2,
        .next_gate = 0,
        .over = false,
        .skier = {.x = 128.0f, .y = 0.0f, .angle = 1, .sx = 0.0f, .sy = 1.0f},
        .started = true,
        .trees = {
            {.x = 10, .y = 30, .color = 0, .mirror = 1},
            {.x = 220, .y = 130, .color = 1, .mirror = 1},
        },
        .trees_count = 2};

    return game;
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
}

void game_update(struct Game *game)
{
    if (!game->started)
    {
        return;
    }
    if (game->over)
    {
        return;
    }

    if (riv->keys[RIV_GAMEPAD_LEFT].press)
    {
        game_move_left(game);
    }
    if (riv->keys[RIV_GAMEPAD_RIGHT].press)
    {
        game_move_right(game);
    }

    // update x position
    float next_x = min(max(game->skier.x + game->skier.sx, 0.0f), 256.0f - 16.0f);

    // update y position
    float next_y = game->skier.y + game->skier.sy;

    // check if crossed gate
    int64_t gate_y = game->gate_start + game->next_gate * game->gate_spacing;
    if (next_y >= gate_y)
    {
        // check if crossed between poles
        struct Line skier_line = {
            .a = {.x = game->skier.x, .y = game->skier.y},
            .b = {.x = next_x, .y = next_y}};
        struct Line gate_line = {
            .a = {.x = game->gates[game->next_gate] - game->gate_width / 2, .y = gate_y},
            .b = {.x = game->gates[game->next_gate] + game->gate_width / 2, .y = gate_y}};
        if (!collision_lines(skier_line, gate_line))
        {
            // missed gate
            riv_printf("missed gate %lu\n", game->next_gate);
            game->gates_missed++;
        }
        else
        {
            // passed gate
            riv_printf("passed gate %lu\n", game->next_gate);
        }
        game->next_gate++;

        if (game->next_gate == game->gates_count)
        {
            game->over = true;
        }
    }

    game->skier.x = next_x;
    game->skier.y = next_y;
}

void game_start(struct Game *game)
{
    do
    {
        game_update(game);
        riv_clear(RIV_COLOR_WHITE);
        draw_game(game);

    } while (riv_present());
}
