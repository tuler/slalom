#ifndef COLLISION_H
#define COLLISION_H

#include <stdbool.h>

struct Point
{
    float x;
    float y;
};

struct Line
{
    struct Point a;
    struct Point b;
};

struct Rect
{
    float x;
    float y;
    float w;
    float h;
};

bool collision_lines(struct Line l1, struct Line l2);

bool collision_rects(struct Rect r1, struct Rect r2);

#endif
