#include "leddisplay.h"
#include "motif.h"

#include "MaxMatrix.h"

/* local types & variables */
enum MatrixStates
{
    STANDBY = 0,
    CLEAR,
    MOTIF,
    DAY_COUNT
};

typedef struct leddisplay
{
    MaxMatrix *matrix;
    MatrixStates state = STANDBY;

    Motif motif;
    uint8_t frame_id = 0;
    uint32_t frame_time = 0;
    uint16_t frame_timeout = 500;
    bool loop = false;

    uint16_t day_count;
} leddisplay_t;

leddisplay_t *leddisplay;

/* private functions prototypes */
void leddisplay_draw_motif(const frame_t motif_frames[], const uint8_t nb_frames);

/* public functions */
void leddisplay_init(uint8_t cs_pin)
{
    leddisplay = new leddisplay_t;
    leddisplay->matrix = new MaxMatrix(cs_pin);
    leddisplay->matrix->init();
    leddisplay->matrix->setModuleOrientation(MAXMATRIX_MODULE_ORIENTATION_270);
    leddisplay->matrix->setIntensity(1);
}

void leddisplay_set_motif(Motif motif, bool loop)
{
    leddisplay->frame_timeout = 80;
    leddisplay->state = MOTIF;
    leddisplay->loop = loop;
    leddisplay->frame_id = 0;
    leddisplay->motif = motif;
}

void leddisplay_set_day_count(uint32_t day_count, uint8_t nb_digit, bool sign, bool loop)
{
    uint8_t length = nb_digit + 3;
    char* message = new char[length];

    if (sign == true)
    {
        strcpy(message, " J+");
    }
    else
    {
        strcpy(message, " J-");   
    }
    
    strcat(message, String(day_count).c_str());
    message[length] = 0;

    leddisplay->frame_timeout = 80;
    leddisplay->state = DAY_COUNT;
    leddisplay->loop = loop;

    leddisplay->matrix->setTextWithShift(message);   
}

void leddisplay_clear()
{
    leddisplay->frame_timeout = 80;
    leddisplay->state = CLEAR;   
}

void leddisplay_process()
{
    uint32_t current_time = millis();

    if (current_time - leddisplay->frame_time > leddisplay->frame_timeout && leddisplay->state != STANDBY)
    {
        switch (leddisplay->state)
        {
            case CLEAR:
                leddisplay->matrix->clear();
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

                    case SPINNER:
                        leddisplay_draw_motif(SPINNER_FRAMES, SPINNER_LEN);
                    break;

                    default:
                    break;
                }
                break;

            case DAY_COUNT:
                leddisplay->matrix->shiftTask();
                leddisplay->frame_time = millis();
                break;

            case STANDBY:
            default:
                break;
        }
    }
}

__attribute__((weak)) void leddisplay_motif_once_finish_cb()
{
    // To implemented by user somewhere in the application code
}


/* private functions */
void leddisplay_draw_motif(const frame_t motif_frames[], const uint8_t nb_frames)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        leddisplay->matrix->setRow(0,i,motif_frames[leddisplay->frame_id][i]);
    }
    leddisplay->frame_time = millis();

    if (leddisplay->frame_id < (nb_frames - 1))
    {
        leddisplay->frame_id++;
    }
    else
    {
        if (leddisplay->loop == false)
        {
            leddisplay_motif_once_finish_cb();
        }
        leddisplay->frame_id = 0;
    }
}