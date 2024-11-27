#include <riv.h>

#include "color.h"
#include "draw.h"

uint64_t img_id = 0;

void draw_init(const char *sprite)
{
    uint32_t res = riv_load_palette(sprite, 16);
    riv_printf("palette loaded: %d\n", res);
    // color_setup();
    img_id = riv_make_image(sprite, 0);
}

void draw_tree(uint64_t x0, uint64_t y0, uint64_t color)
{
    uint64_t sx0 = 48 + (color * 16);
    riv_draw_image_rect(img_id, x0, y0, 16, 32, sx0, 0, 2, 1);
}

/**
 * color = 0: red
 * color = 1: blue
 */
void draw_gate(uint64_t x0, uint64_t y0, uint64_t w, uint64_t color)
{
    uint64_t sx0 = 40;
    uint64_t sy0 = color * 16;
    riv_draw_image_rect(img_id, x0 - w / 2, y0, 8, 16, sx0, sy0, 2, 1);
    riv_draw_image_rect(img_id, x0 + w / 2, y0, 8, 16, sx0, sy0, 2, 1);
}

void draw_skier(uint64_t x0, uint64_t y0, uint64_t pos)
{
    uint64_t sx0 = pos * 8;
    riv_draw_image_rect(img_id, x0, y0, 8, 20, sx0, 0, 2, 1);
}

void draw_mogul(uint64_t x0, uint64_t y0, uint64_t color)
{
    uint64_t sx0 = (color * 16);
    riv_draw_image_rect(img_id, x0, y0, 16, 8, sx0, 24, 2, 1);
}

void draw_map()
{
}
