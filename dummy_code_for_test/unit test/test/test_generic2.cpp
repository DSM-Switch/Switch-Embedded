#if 0

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "private_settings.h"

#define AP_SSID "esp8266"
#define AP_PSWD "qwerty1234"
#define LED LED_BUILTIN

static void station_disconnected(const WiFiEventStationModeDisconnected& event);

static WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;

void setup() {
    Serial.begin(115200);
    delay(10);
    Serial.setDebugOutput(true);
    Serial.println();

    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

    gotIpEventHandler = WiFi.onStationModeGotIP(
        [] (const WiFiEventStationModeGotIP& event) {
            Serial.printf("softAPdisconnect: %s\n", WiFi.softAPdisconnect() ? "success" : "fail");
            disconnectedEventHandler = WiFi.onStationModeDisconnected(station_disconnected);
        });

    disconnectedEventHandler = WiFi.onStationModeDisconnected(station_disconnected);

    WiFi.begin(WIFI_SSID, WIFI_PWD);
}

void loop() {
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    delay(500);
}

static
void station_disconnected(const WiFiEventStationModeDisconnected& event) {
    bool success = WiFi.softAP(AP_SSID, AP_PSWD);
    Serial.printf("softAP: %s\n", success ? "success" : "fail");
    if (success) disconnectedEventHandler = WiFiEventHandler();
}

#endif