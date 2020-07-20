#ifndef __LEDDISPLAY_INCLUDED__
#define __LEDDISPLAY_INCLUDED__

#include "bsp.h"

enum Motif
{
    NONE = 0,
    HEART,
    HEARTRATE,
    HEARTDEATH,
    SPINNER
};

void leddisplay_init(uint8_t cs_pin);
void leddisplay_process();

void leddisplay_set_motif(Motif motif, bool loop = true);
void leddisplay_set_day_count(uint32_t day_count, uint8_t nb_digit, bool sign, bool loop = false);
void leddisplay_clear();
void leddisplay_motif_once_finish_cb();

#endif