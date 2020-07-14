#include "leddisplay.h"
#include "motif.h"

#include <LedMatrix.h>
#include <SPI.h>

#define FRAME_TIMEOUT 80

/* local types & variables */
enum MatrixStates
{
    STANDBY = 0,
    CLEAR,
    MOTIF,
    TEXT
};

typedef struct leddisplay
{
    uint8_t cs_pin;
    LedMatrix *matrix;
    MatrixStates state = STANDBY;

    Motif motif;
    uint8_t frame_id = 0;
    uint32_t frame_time = 0;

    bool loop = false;
} leddisplay_t;

leddisplay_t *leddisplay;

/* private functions prototypes */
void leddisplay_draw_motif(const frame_t motif_frames[], const uint8_t nb_frames);

/* public functions */
void leddisplay_init(uint8_t cs_pin)
{
    leddisplay = new leddisplay_t;
    leddisplay->cs_pin = cs_pin;
    leddisplay->matrix = new LedMatrix(1, cs_pin);
    leddisplay->matrix->init();
}

void leddisplay_set_motif(Motif motif, bool loop)
{
    leddisplay->state = MOTIF;
    leddisplay->loop = loop;
    leddisplay->motif = motif;
}

void leddisplay_set_text(String text, bool loop)
{
    leddisplay->state = TEXT;
    leddisplay->loop = loop;
    leddisplay->matrix->setText(text);
}

void leddisplay_clear()
{
    leddisplay->state = CLEAR;   
}

void leddisplay_process()
{
    uint32_t current_time = millis();

    if (current_time - leddisplay->frame_time > FRAME_TIMEOUT && leddisplay->state != STANDBY)
    {
        switch (leddisplay->state)
        {
            case CLEAR:
                leddisplay->matrix->clear();
                leddisplay->matrix->commit();
                leddisplay->state = STANDBY;
                break;
            
            case MOTIF:
                switch (leddisplay->motif)
                {
                    case HEART:
                        leddisplay_draw_motif(HEART_FRAMES, HEART_LEN);
                    break;

                    case HEARTRATE:
                        leddisplay_draw_motif(HEARTRATE_FRAMES, HEARTRATE_LEN);
                    break;

                    case HEARTDEATH:
                        leddisplay_draw_motif(HEARTDEATH_FRAMES, HEARTDEATH_LEN);
                    break;

                    case DOTBOUNCE:
                        leddisplay_draw_motif(DOTBOUNCE_FRAMES, DOTBOUNCE_LEN);
                    break;

                    default:
                    break;
                }
                break;

            case TEXT:
                leddisplay->matrix->drawText();
                leddisplay->matrix->commit();
                leddisplay->frame_time = millis();
                break;

            case STANDBY:
            default:
                break;
        }
    }
}

/* private functions */
void leddisplay_draw_motif(const frame_t motif_frames[], const uint8_t nb_frames)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        leddisplay->matrix->setColumn(i,motif_frames[leddisplay->frame_id][i]);
    }
    leddisplay->matrix->commit();
    leddisplay->frame_time = millis();

    if (leddisplay->frame_id < (nb_frames - 1))
    {
        leddisplay->frame_id++;
    }
    else
    {
        leddisplay->frame_id = 0;
    }
}