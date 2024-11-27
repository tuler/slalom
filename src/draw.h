#ifndef DRAW_H
#define DRAW_H

#include <riv.h>

void draw_init(const char *sprite);

void draw_tree(uint64_t x0, uint64_t y0, uint64_t color);

void draw_gate(uint64_t x0, uint64_t y0, uint64_t w, uint64_t color);

void draw_skier(uint64_t x0, uint64_t y0, uint64_t pos);

void draw_mogul(uint64_t x0, uint64_t y0, uint64_t color);

void draw_map();

#endif
