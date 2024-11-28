#ifndef DRAW_H
#define DRAW_H

#include <riv.h>

#include "game.h"

void draw_init(const char *sprite);

void draw_palette();

void draw_game(struct Game *game);

#endif
