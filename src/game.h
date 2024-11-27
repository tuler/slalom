#ifndef GAME_H
#define GAME_H

#include <riv.h>

#define MAX_GATES 256
#define MAX_TREES 256
#define MAX_MOGULS 256

struct Skier
{
    float x;       // x-axis position
    float y;       // y-axis position
    int64_t angle; // -4, -3, -2, -1, 1, 2, 3, 4 (0 is crash)
    float sx;      // speed x-axis per frame
    float sy;      // speed y-axis per frame
};

struct Tree
{
    int64_t x;      // x-axis position
    int64_t y;      // y-axis position
    uint64_t color; // 0-3
    int64_t mirror; // 1 or -1
};

struct Mogul
{
    int64_t x;      // x-axis position
    int64_t y;      // y-axis position
    uint64_t color; // 0-1
    int64_t mirror; // 1 or -1
};

struct Game
{
    uint64_t gate_width;
    uint64_t gate_start;
    uint64_t gate_spacing;

    struct Skier skier;

    uint64_t gates[MAX_GATES]; // x-axis positions of gates
    size_t gates_count;

    struct Tree trees[MAX_TREES]; // tree structures
    size_t trees_count;

    struct Mogul moguls[MAX_MOGULS]; // tree structures
    size_t moguls_count;
};

struct Game game_create(uint64_t gate_width);

void game_start(struct Game *game);

#endif
