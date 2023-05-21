#include <Arduino.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "wifi_something.h"
#include "private_settings.h"

using JsonDoc = DynamicJsonDocument;

void setup() {
    Serial.begin(115200);
    // Serial.setDebugOutput(true);
    delay(10);
    Serial.println();

    JsonDoc doc(1024);
    if (deserializeJson(doc, R"({ "a": "hi" })") != DeserializationError::Ok) {
        Serial.println("deserialization failed");
    }

    Serial.printf((const char*)doc["a"] ?: "null");
}

void loop() { 
    
}