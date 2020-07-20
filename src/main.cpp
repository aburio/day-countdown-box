#include "bsp.h"
#include "data.h"

#if !defined(SSID) || !defined(PASSWORD)
#error "Create a wifi_data.h header and add SSID & PASSWORD define to allow wifi connection"
#endif

#include "app/app.h"
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
  leddisplay_init(CS_PIN);
  button_init(BUTTON_PIN);
  wifi_init(SSID, PASSWORD);
  app_init();
}

/**
 * Processing loop
 * 
 */
void loop() {
  app_process();
  button_process();
  leddisplay_process();
  wifi_process();
}