/*
access point
*/

#if 0

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "private_settings.h"

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    Serial.print("Setting soft-AP ... ");
    bool result = WiFi.softAP(WIFI_SSID, WIFI_PWD);
    
    if (result) {
        Serial.println("Ready");
    } else {
        Serial.println("Failed!");
    }
}

void loop() {
    Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
    delay(3000);
}

#endif