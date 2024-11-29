#include <riv.h>

#include "sfx.h"

riv_waveform_desc gate_pass_sfx = {
    .type = RIV_WAVEFORM_NOISE,
    .attack = 0.125,
    .decay = 0.1,
    .sustain = 0.0,
    .release = 0.075,
    .start_frequency = 90,
    .end_frequency = 25,
    .amplitude = 0.25,
    .sustain_level = 0.5,
    .duty_cycle = 0.5,
    .pan = 0.0,
};

riv_waveform_desc gate_miss_sfx = {
    .type = RIV_WAVEFORM_SAWTOOTH,
    .attack = 0.075,
    .decay = 0.15,
    .sustain = 0.075,
    .release = 0.225,
    .start_frequency = 42,
    .end_frequency = 42,
    .amplitude = 0.2,
    .sustain_level = 0.1,
    .duty_cycle = 0.5,
    .pan = 0.0,
};

riv_waveform_desc crash_sfx = {
    .type = RIV_WAVEFORM_NOISE,
    .attack = 0.025,
    .decay = 0.1,
    .sustain = 0.125,
    .release = 0.025,
    .start_frequency = 110,
    .end_frequency = 27,
    .amplitude = 0.25,
    .sustain_level = 0.5,
    .duty_cycle = 0.5,
    .pan = 0.0,
};

void sfx_game_start()
{
}

void sfx_gate_pass()
{
    riv_waveform(&gate_pass_sfx);
}

void sfx_gate_miss()
{
    riv_waveform(&gate_miss_sfx);
}

void sfx_crash()
{
    riv_waveform(&crash_sfx);
}

void sfx_game_over()
{
}
