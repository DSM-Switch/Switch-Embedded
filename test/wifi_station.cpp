#include "settings.h"
#ifdef ENABLE_WIFI_STATAION

#include <Arduino.h>
#include <unity.h>
#include <ESP8266WiFi.h>
#include <stdio.h>
#include "private_settings.h"

#define WAIT_TIME 10'000

void setUp() { }

void tearDown() { }

void test1() {
    Serial.println('\n');

    WiFi.begin(WIFI_SSID, WIFI_PWD);             // Connect to the network

    TEST_MESSAGE("Connecting to ");
    TEST_MESSAGE(WIFI_SSID);

    unsigned long prev = millis();
    while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
        if (millis() - prev >= WAIT_TIME) break;
        yield();
    }

    TEST_ASSERT_EQUAL(WL_CONNECTED, WiFi.status());

    TEST_MESSAGE("Connection established!");  
    TEST_MESSAGE("IP address:\t");
    TEST_MESSAGE(WiFi.localIP().toString().c_str());
}

void setup() {
    delay(2000);
    Serial.begin(115200);
    delay(10);

    UNITY_BEGIN();

    RUN_TEST(test1);

    UNITY_END();
}

void loop() { }

#endif /* ENABLE_WIFI_STATAION */