#if 0

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include "private_settings.h"

// void setup() {
//     Serial.begin(115200);
//     // Serial.setDebugOutput(true);
//     delay(10);
//     Serial.println();

//     if (!LittleFS.begin()) {
//         Serial.println("An Error has occurred while mounting LittleFS");
//         return;
//     }

//     File file = LittleFS.open("/name", "w");
//     if (!file) {
//         Serial.println("Failed to open file for writing");
//         return;
//     }
//     file.write("myroom1");
//     file.close();
// }

void setup() {
    Serial.begin(115200);
    // Serial.setDebugOutput(true);
    delay(10);
    Serial.println();

    // WiFi.persistent(true);
    // WiFi.begin(WIFI_SSID, WIFI_PWD);


    Serial.printf("SSID: %s\n", WiFi.SSID().c_str() ?: "null");
    Serial.printf("STATUS: %d\n", (int)WiFi.begin()); // 0
    Serial.printf("SSID: %s\n", WiFi.SSID().c_str() ?: "null");
    Serial.printf("STATUS: %d\n", (int)WiFi.begin("hi")); // 7
    Serial.printf("SSID: %s\n", WiFi.SSID().c_str() ?: "null"); 
}

void loop() {

}

#endif