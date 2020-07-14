#ifndef __LEDDISPLAY_INCLUDED__
#define __LEDDISPLAY_INCLUDED__

#include "bsp.h"

enum Motif
{
    NONE = 0,
    HEART,
    HEARTRATE,
    HEARTDEATH,
    DOTBOUNCE
};

void leddisplay_init(uint8_t cs_pin);
void leddisplay_process();

void leddisplay_set_motif(Motif motif, bool loop = true);
void leddisplay_set_text(String text, bool loop = true);
void leddisplay_clear();

#endif