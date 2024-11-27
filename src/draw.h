#ifndef DRAW_H
#define DRAW_H

#include <riv.h>

#include "game.h"

void draw_init(const char *sprite);

void draw_tree(int64_t x0, int64_t y0, uint64_t color);

void draw_gate(int64_t x0, int64_t y0, uint64_t w, uint64_t color);

void draw_skier(int64_t x0, int64_t y0, int64_t angle);

void draw_mogul(int64_t x0, int64_t y0, uint64_t color);

void draw_game(struct Game *game);

#endif
