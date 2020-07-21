#include "app.h"
#include "data.h"

#include <TZ.h>
#include <coredecls.h>
#include <time.h>
#include <sys/time.h>

#include "button/button.h"
#include "leddisplay/leddisplay.h"
#include "wifi/wifi.h"

#define TZ TZ_Europe_Paris

/* local types & variables */
enum AppStates
{
    IDLE = 0,
    WAITING_WIFI,
    WAITING_NTP,
    LAUNCHING_OTA,
    WAITING_OTA,
    DISPLAY_DAYCOUNT,
    DISPLAY_HEART,
    WAITING_HEART
};

AppStates state;

static time_t now;

/* private functions prototypes */
void app_time_updated();

/* public functions */
void app_init()
{
  configTime(TZ, "pool.ntp.org");
  state = WAITING_WIFI;
}

void app_process()
{
    if (state == DISPLAY_HEART)
    {
      leddisplay_set_motif(HEART, false);
      state = WAITING_HEART;
    }
    if (state == DISPLAY_DAYCOUNT)
    {
      time_t date_wday = 1622898000;
      time_t number;
      uint8_t digit = 0;
      bool sign = true;

      now = time(nullptr);

      if(date_wday > now)
      {
        sign = false;
        now = (date_wday - now) / 86400;
      }
      else
      {
        now = (now - date_wday) / 86400;
      }

      number = now;
      do
      {
        /* Increment digit count */
        digit++;
        number /= 10;
      } while(number != 0);
      leddisplay_set_day_count((uint32_t)now, digit, sign, false);
      state = IDLE;
    }
    else if (state == LAUNCHING_OTA)
    {
      leddisplay_set_motif(HEARTRATE);
      wifi_enable_ota_update(180000);
      state = WAITING_OTA;
    }
}

/* private functions */
void app_time_updated()
{
  if (state == WAITING_NTP)
  {
    state = IDLE;
  }
}

/**
 * Press&release detection callback
 * 
 */
void button_press_cb(uint8_t click_count)
{
  if (state == IDLE)
  {
    state = DISPLAY_HEART;
  }
}

/**
 * Long press detection callback
 * 
 */
void button_long_press_cb()
{
  state = LAUNCHING_OTA;
}

void leddisplay_motif_once_finish_cb()
{
  if (state == WAITING_HEART)
  {
    state = DISPLAY_DAYCOUNT;
  }
}

/**
 * Wifi connected callback
 * 
 */
void wifi_connect_cb()
{
  leddisplay_clear();
  settimeofday_cb(app_time_updated);
  state = WAITING_NTP;
}

/**
 * Wifi try to connect callback
 * 
 */
void wifi_connecting_cb()
{
  leddisplay_set_motif(SPINNER);
  state = WAITING_WIFI;
}

void wifi_ota_timeout_cb()
{
  if (state == WAITING_OTA)
  {
    leddisplay_clear();
    state = IDLE;
  }
}

/**
 * Wifi disconnection callback
 * 
 */
void wifi_disconnect_cb()
{
  leddisplay_set_motif(HEARTDEATH);
  state = WAITING_WIFI;
}