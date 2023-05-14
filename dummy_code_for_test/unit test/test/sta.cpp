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

    wifi_connect();
}

void loop() { }

#endif