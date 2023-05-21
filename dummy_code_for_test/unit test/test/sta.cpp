/*
station
*/

#if 0

#include <Arduino.h>
#include "wifi_something.h"

void setup()
{
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    // wifi_connect();

    // 1
    // WiFi.begin();
    // Serial.printf("SSID: %s\n", WiFi.SSID().c_str()); // SSID:
// 2
    // WiFi.begin("hi", "hello");
    // Serial.printf("SSID: %s\n", WiFi.SSID().c_str()); // SSID: hi
// 3
    // WiFi.persistent(true);
    // WiFi.begin("hi", "hello");
    // Serial.printf("SSID: %s\n", WiFi.SSID().c_str()); // SSID: hi
// 4
    // wifi_connect("esp8266", "qwerty1234");
    // wifi_connect(WIFI_SSID, WIFI_PWD);
}

void loop() { }

#endif