#ifndef __WIFI_INCLUDED__
#define __WIFI_INCLUDED__

#include "bsp.h"

void wifi_init(const String ssid, const String password);
void wifi_enable_ota_update(uint32_t timeout_ms);
void wifi_process();

void wifi_connect_cb();
void wifi_connecting_cb();
void wifi_ota_timeout_cb();
void wifi_disconnect_cb();

#endif