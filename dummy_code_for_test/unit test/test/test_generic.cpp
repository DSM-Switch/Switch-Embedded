#if 0

#include <ESP8266WiFi.h>
#include "private_settings.h"

static WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;

static bool ledState;

void setup() {
    delay(10);
    Serial.begin(115200);
    Serial.println();

    pinMode(LED_BUILTIN, OUTPUT);

    gotIpEventHandler = WiFi.onStationModeGotIP(
        [] (const WiFiEventStationModeGotIP& event) {
            Serial.print("Station connected, IP: ");
            Serial.println(WiFi.localIP());
        });

    disconnectedEventHandler = WiFi.onStationModeDisconnected(
        [] (const WiFiEventStationModeDisconnected& event) {
            Serial.println("Station disconnected");
        });

    Serial.printf("Connecting to %s ...\n", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PWD);
}


void loop() {
    digitalWrite(LED_BUILTIN, ledState);
    ledState = !ledState;
    delay(250);
}

#endif