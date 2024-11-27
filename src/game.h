#include <riv.h>

struct Game
{
    uint64_t gate_width;
    uint64_t x;
    uint64_t y;
    uint64_t angle; // -4, -3, -2, -1, 1, 2, 3, 4 (0 is crash)
};

struct Game game_create(uint64_t game_width);

void game_start(struct Game *game);
