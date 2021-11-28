# Day Countdown Box

The day countdown box is a connected device based on an ESP8266 and a 8x8 Monochrome LED matrix.

It have been imagine to answer questions like "How many days remain before our Wedding ?" 😉

## Hardware requirements

- ESP12e
- MAX7219 LED matrix drive
- 8x8 Monochrome LED matrix

## Software dependencies

- [Platformio](https://platformio.org/) IDE
- [MaxMatrix](https://github.com/AndreasBur/MaxMatrix) library
- [NTPClient](https://github.com/arduino-libraries/NTPClient) library

## Settings

In order to compile this project you have to create a `wifi_data.h` header in the `include` folder

```
#ifndef __WIFI_DATA_INCLUDED__
#define __WIFI_DATA_INCLUDED__

#define SSID        "xxx"
#define PASSWORD    "yyy"

#endif
```

replace `xxx` with your Wifi SSID and `yyy` with your Wifi password

## Note

This repository was initially private. In the commit history personal Wifi information (SSID and password) have been exposed. Those information have been depreciate by updating network router settings :)
