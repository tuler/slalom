#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collision.h"
#include "draw.h"
#include "game.h"

#define GATE_PENALTY 5000  // time penalty (ms) for missing a gate
#define CRASH_PENALTY 1000 // time penalty (ms) for crashing

void game_load_file(const char *file, struct Game *game)
{
    // open track file
    FILE *f = fopen(file, "r");
    if (!f)
    {
        fprintf(stderr, "error opening file %s\n", file);
        exit(1);
    }

    // first line of file is list of x-coordinates of gates
    char line[1024];
    if (fgets(line, sizeof(line), f))
    {
        char *token = strtok(line, " ");
        game->gates_count = 0;

        while (token && game->gates_count < MAX_GATES)
        {
            game->gates[game->gates_count++] = atoi(token);
            token = strtok(NULL, " ");
        }
    }

    // second line of file is a list of x and y pair of tree coordinates
    if (fgets(line, sizeof(line), f))
    {
        char *token = strtok(line, " ");
        game->trees_count = 0;

        while (token && game->trees_count < MAX_TREES)
        {
            game->trees[game->trees_count].x = atoi(token);
            token = strtok(NULL, " ");
            game->trees[game->trees_count].y = atoi(token);

            // randomize tree color
            game->trees[game->trees_count].color = riv_rand() % 4;

            // randomize tree mirror
            game->trees[game->trees_count].mirror = (riv_rand() % 2) == 0 ? -1 : 1;

            game->trees_count++;
            token = strtok(NULL, " ");
        }
    }

    // third line of file is a list of x and y pair of mogul coordinates
    if (fgets(line, sizeof(line), f))
    {
        char *token = strtok(line, " ");
        game->moguls_count = 0;

        while (token && game->moguls_count < MAX_MOGULS)
        {
            game->moguls[game->moguls_count].x = atoi(token);
            token = strtok(NULL, " ");
            game->moguls[game->moguls_count].y = atoi(token);

            // randomize mogul color
            game->moguls[game->moguls_count].color = riv_rand() % 2;

            // randomize mogul mirror
            game->moguls[game->moguls_count].mirror = (riv_rand() % 2) == 0 ? -1 : 1;

            game->moguls_count++;
            token = strtok(NULL, " ");
        }
    }

    fclose(f);
}

struct Game game_create(const char *file, uint64_t gate_width, uint64_t time_per_gate)
{
    struct Game game = {
        .gate_spacing = 96,
        .gate_start = 64,
        .gate_width = gate_width,
        .gates_missed = 0,
        .next_gate = 0,
        .over = false,
        .skier = {.x = 128.0f, .y = 0.0f, .angle = 1, .sx = 0.0f, .sy = 1.5f},
        .started = true};

    // load track file
    game_load_file(file, &game);

    // initialize game state
    game.time_left = game.max_time = time_per_gate * game.gates_count;
    // game.skier.sy = (4 - abs(game.skier.angle)) * 0.5;

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

void game_write_score(uint64_t score, uint64_t gates_missed)
{
    riv->outcard_len = riv_snprintf(
        (char *)riv->outcard, RIV_SIZE_OUTCARD,
        "JSON{\"score\":%d,\"gates_missed\":%d}",
        score,
        gates_missed);
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

    if (game->skier.angle == 0) // skiier crashed
    {
        if (riv->time_ms > game->crash_recover) // time to stand up
        {
            game->skier.angle = 1; // point down the hill
            game->skier.sx = 0;    // no side moveme
            game->skier.sy = 1.5;  // full speed
        }
    }
    else if (riv->keys[RIV_GAMEPAD_LEFT].press)
    {
        game_move_left(game);
    }
    else if (riv->keys[RIV_GAMEPAD_RIGHT].press)
    {
        game_move_right(game);
    }

    // update x position
    float next_x = min(max(game->skier.x + game->skier.sx, 0.0f), 256.0f - 16.0f);

    // update y position
    float next_y = game->skier.y + game->skier.sy;

    // check tree collision
    for (size_t i = 0; i < game->trees_count; i++)
    {
        if (game->trees[i].hit)
        {
            continue; // skip hit trees
        }

        struct Rect skier_rect = {
            .x = next_x,
            .y = next_y + 10, // only collide with lower body part
            .w = 8 * PIXEL_WIDTH,
            .h = 10}; // only collide with lower body part
        struct Rect tree_rect = {
            .x = game->trees[i].x,
            .y = game->trees[i].y + 24, // only collide with base of the tree
            .w = 16 * PIXEL_WIDTH,
            .h = 8}; // only collide with base of the tree

        if (collision_rects(skier_rect, tree_rect))
        {
            // skier crashed
            game->trees[i].hit = true;                          // mark tree as hit, so we don't hit it again
            game->skier.angle = 0;                              // crash position
            game->skier.sx = 0;                                 // no side movement
            game->skier.sy = 0;                                 // no down movement
            game->crash_recover = riv->time_ms + CRASH_PENALTY; // set time to recover
        }
    }

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
            game->gates_missed++;
        }
        game->next_gate++;
    }

    // calculate time left, accounting for penalties of missed gates
    game->time_left = max(game->max_time - (riv->time_ms - game->start_time) - game->gates_missed * GATE_PENALTY, 0);

    if (game->next_gate == game->gates_count)
    {
        // crossed all gates
        game->over = true;

        // write time left as the score
        game_write_score(game->time_left, game->gates_missed);

        // quit after 3 seconds
        riv->quit_frame = riv->frame + riv->target_fps * 3;
    }

    if (game->time_left <= 0)
    {
        game->over = true;

        // quit after 3 seconds
        riv->quit_frame = riv->frame + riv->target_fps * 3;
    }

    // update skier position
    game->skier.x = next_x;
    game->skier.y = next_y;
}

void game_start(struct Game *game)
{
    game_write_score(0, 0);
    game->started = true;
    game->start_time = riv->time_ms;
    do
    {
        game_update(game);
        riv_clear(RIV_COLOR_WHITE);
        draw_game(game);

    } while (riv_present());
}
