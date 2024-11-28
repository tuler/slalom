#include "collision.h"
#include "game.h"

bool collision_lines(struct Line l1, struct Line l2)
{
    float x1 = l1.a.x;
    float y1 = l1.a.y;
    float x2 = l1.b.x;
    float y2 = l1.b.y;
    float x3 = l2.a.x;
    float y3 = l2.a.y;
    float x4 = l2.b.x;
    float y4 = l2.b.y;

    float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (d == 0)
    {
        return false;
    }

    float x = ((x3 - x4) * (x1 * y2 - y1 * x2) - (x1 - x2) * (x3 * y4 - y3 * x4)) / d;
    float y = ((y3 - y4) * (x1 * y2 - y1 * x2) - (y1 - y2) * (x3 * y4 - y3 * x4)) / d;

    if (x < min(x1, x2) || x > max(x1, x2))
    {
        return false;
    }
    if (x < min(x3, x4) || x > max(x3, x4))
    {
        return false;
    }
    if (y < min(y1, y2) || y > max(y1, y2))
    {
        return false;
    }
    if (y < min(y3, y4) || y > max(y3, y4))
    {
        return false;
    }

    return true;
}

bool collision_rects(struct Rect r1, struct Rect r2)
{
    if (r1.x + r1.w < r2.x)
    {
        return false;
    }
    if (r1.x > r2.x + r2.w)
    {
        return false;
    }
    if (r1.y + r1.h < r2.y)
    {
        return false;
    }
    if (r1.y > r2.y + r2.h)
    {
        return false;
    }

    return true;
}
