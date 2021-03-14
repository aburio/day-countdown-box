#ifndef __APP_INCLUDED__
#define __APP_INCLUDED__

#include "bsp.h"

#define DDAY_UNIX       1622898000u
#define DDAY_TIMEZONE   TZ_Europe_Paris
#define OTA_TIMEOUT     180000u

void app_init();
void app_process();

#endif