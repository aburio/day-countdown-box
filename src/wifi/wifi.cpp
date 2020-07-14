#include "wifi.h"

#include <ESP8266WiFi.h>

/* local types & variables */
enum WifiStates
{
    UNSET = 0,
    DISCONNECTED,
    CONNECTING,
    CONNECTED
};

typedef struct Wifi
{
    uint8_t state = UNSET;
} wifi_t;

wifi_t *wifi;

/* private functions prototypes */

/* public functions */
void wifi_init(const String ssid, const String password)
{
    wifi = new wifi_t;

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    wifi->state = CONNECTING;
    wifi_connecting_cb();
}

void wifi_process()
{
    if (wifi->state == CONNECTING && WiFi.status() == WL_CONNECTED)
    {
        wifi->state = CONNECTED;
        wifi_connect_cb();
    }
}

__attribute__((weak)) void wifi_connect_cb()
{
    // To implemented by user somewhere in the application code
}

__attribute__((weak)) void wifi_connecting_cb()
{
    // To implemented by user somewhere in the application code
}

__attribute__((weak)) void wifi_disconnect_cb()
{
    // To implemented by user somewhere in the application code
}

/* private functions */
