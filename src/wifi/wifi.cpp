#include "wifi.h"

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

/* local types & variables */
enum WifiStates
{
    UNSET = 0,
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    OTA_STARTUP,
    OTA_PROCESS
};

const char* host = "day-counter";
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

typedef struct Wifi
{
    uint8_t state = UNSET;
    uint16_t ota_timeout_ms;
    uint32_t ota_timer_ms;
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

void wifi_enable_ota_update(uint32_t timeout_ms)
{
    wifi->ota_timeout_ms = timeout_ms;
    wifi->state = OTA_STARTUP;
}

void update_started() {
  Serial.println("CALLBACK:  HTTP update process started");
}

void update_finished() {
  Serial.println("CALLBACK:  HTTP update process finished");
}

void update_progress(int cur, int total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err) {
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}

void wifi_process()
{
    if (wifi->state == CONNECTING && WiFi.status() == WL_CONNECTED)
    {
        wifi->state = CONNECTED;
        wifi_connect_cb();
    }
    else if (wifi->state == OTA_STARTUP)
    {
        MDNS.begin(host);

        httpUpdater.setup(&httpServer);
        httpServer.begin();

        MDNS.addService("http", "tcp", 80);
        wifi->ota_timer_ms = millis();
        wifi->state = OTA_PROCESS;
    }
    else if (wifi->state == OTA_PROCESS)
    {
        uint32_t now = millis();

        httpServer.handleClient();
        MDNS.update();
        if (now - wifi->ota_timer_ms > wifi->ota_timeout_ms)
        {
            httpServer.close();
            httpServer.stop();
            MDNS.close();
            MDNS.end();
            wifi->state = CONNECTED;
            wifi_ota_timeout_cb();
        }

    }
    else if (wifi->state == CONNECTED && WiFi.status() != WL_CONNECTED)
    {
        wifi->state = DISCONNECTED;
        wifi_disconnect_cb();
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

__attribute__((weak)) void wifi_ota_timeout_cb()
{
    // To implemented by user somewhere in the application code
}

__attribute__((weak)) void wifi_disconnect_cb()
{
    // To implemented by user somewhere in the application code
}

/* private functions */
