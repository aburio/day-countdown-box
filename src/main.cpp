#include "bsp.h"
#include "wifi_data.h"

#if !defined(SSID) || !defined(PASSWORD)
#error "Create a wifi_data.h header and add SSID & PASSWORD define to allow wifi connection"
#endif

#include "button/button.h"
#include "leddisplay/leddisplay.h"
#include "wifi/wifi.h"

#define BUTTON_PIN 12
#define CS_PIN 5

/**
 * Initialization
 * 
 */
void setup() {
  pinMode(LED_BUILTIN,OUTPUT);

  leddisplay_init(CS_PIN);
  button_init(BUTTON_PIN);
  wifi_init(SSID, PASSWORD);
}

/**
 * Processing loop
 * 
 */
void loop() {
  button_process();
  leddisplay_process();
  wifi_process();
}

/**
 * Press&release detection callback
 * 
 */
void button_press_cb(uint8_t click_count)
{
  leddisplay_set_motif(HEART, false);
}

/**
 * Long press detection callback
 * 
 */
void button_long_press_cb()
{
  leddisplay_set_motif(HEARTRATE);
}

/**
 * Wifi connected callback
 * 
 */
void wifi_connect_cb()
{
  leddisplay_clear();
}

/**
 * Wifi try to connect callback
 * 
 */
void wifi_connecting_cb()
{
  leddisplay_set_motif(DOTBOUNCE);
}

/**
 * Wifi disconnection callback
 * 
 */
void wifi_disconnect_cb()
{
  leddisplay_set_motif(HEARTDEATH);
}