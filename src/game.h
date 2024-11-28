#ifndef GAME_H
#define GAME_H

#include <riv.h>

#define MAX_GATES 256
#define MAX_TREES 256
#define MAX_MOGULS 256
#define PIXEL_WIDTH 2

#define abs(x) ((x) < 0 ? -(x) : (x))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

struct Skier
{
    float x;       // x-axis position
    float y;       // y-axis position
    int64_t angle; // -4, -3, -2, -1, 1, 2, 3, 4 (0 is crash)
    float sx;      // speed x-axis per frame
    float sy;      // speed y-axis per frame
};

struct Gate
{
    int64_t x;   // x-axis position
    bool missed; // true if skier missed the gate
};

struct Tree
{
    int64_t x;      // x-axis position
    int64_t y;      // y-axis position
    uint64_t color; // 0-3
    int64_t mirror; // 1 or -1
    bool hit;       // true if skier has hit the tree
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
    uint64_t gate_width;             // distance between gate poles
    uint64_t gate_start;             // y-axis position of first gate
    uint64_t gate_spacing;           // vertical distance between gates
    uint64_t gates_missed;           // number of gates missed
    size_t next_gate;                // index of next gate
    bool started;                    // true if game has started
    uint64_t start_time;             // time game started
    bool over;                       // true if game is over
    uint64_t max_time;               // maximum time in milliseconds
    int64_t time_left;               // time left in milliseconds
    struct Skier skier;              // skier structure
    uint64_t crash_recover;          // time skier will recover from crash
    struct Gate gates[MAX_GATES];    // gate structures
    size_t gates_count;              // number of gates
    struct Tree trees[MAX_TREES];    // tree structures
    size_t trees_count;              // number of trees
    struct Mogul moguls[MAX_MOGULS]; // mogul structures
    size_t moguls_count;             // number of moguls
};

struct Game game_create(const char *file, uint64_t gate_width, uint64_t time_per_gate);

void game_start(struct Game *game);

#endif
