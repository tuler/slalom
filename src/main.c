#include <getopt.h>
#include <riv.h>
#include <stdlib.h>
#include <stdio.h>

#include "draw.h"
#include "game.h"

struct Options
{
    uint64_t gate_width;    // distance between gate poles
    uint64_t time_per_gate; // how many miliseconds per gate
    uint64_t level;         // number of the level to start (0-based)
    const char *file;       // levels file to load
};

static struct option long_options[] = {
    {"gate_width", required_argument, 0, 'g'},
    {"time_per_gate", required_argument, 0, 't'},
    {"level", required_argument, 0, 'l'},
    {"file", required_argument, 0, 'f'},
    {0, 0, 0, 0}};

struct Options parse_args(int argc, char **argv)
{
    struct Options opts = {
        .file = "map.txt",
        .gate_width = 48,
        .level = 0,
        .time_per_gate = 5000};

    int opt;
    while ((opt = getopt_long(argc, (char *const *)argv, "g:t:l:f:", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'g':
            opts.gate_width = atoi(optarg);
            break;
        case 't':
            opts.time_per_gate = atoi(optarg);
            break;
        case 'l':
            opts.level = atoi(optarg);
            break;
        case 'f':
            opts.file = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s [--gate-width w] [--time-per-gate t] [--level n] [--file path]\n", argv[0]);
            exit(1);
        }
    }
    return opts;
}

int main(int argc, char *argv[])
{
    riv->width = 256;
    riv->height = 256;
    riv->target_fps = 60;

    // customize palette
    // color_setup();

    // parse options
    struct Options opts = parse_args(argc, argv);

    // load levels file
    /*
    struct Levels levels;
    if (riv->incard_len > 0)
    {
        level_load_incard(&levels, opts.level);
    }
    else
    {
        level_load_file(&levels, opts.file, opts.level);
    }
    */

    // initialize draw module
    draw_init("sprite.png");

    // create and start the game
    // struct Game game = game_create(&levels, opts.gate_width);
    struct Game game = game_create(opts.gate_width, opts.time_per_gate);
    game_start(&game);

    // free up dynamic allocated resources
    // level_free(&levels);
}
