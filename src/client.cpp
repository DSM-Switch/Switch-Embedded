/*
UDP echo client
*/

#if 0

// header =========================================================

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "private_settings.h"
#include "wifi_something.h"
#include "settings.h"

// macro ==========================================================

#define BUF_SIZE 1024
#define LED LED_BUILTIN

// function declaration ===========================================

static void handle_server();
static void send_signal();
static void send_ap();
static void broadcast();

// variable definition ============================================

using JsonDoc = DynamicJsonDocument;

static WiFiUDP udp;

static IPAddress addrs[4];
static size_t addrs_len;

// function definition ============================================

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(10);
    Serial.println();

    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
    
    // wifi_connect("esp8266", "qwerty1234");
    wifi_connect(WIFI_SSID, WIFI_PWD);

    udp.begin(SERVER_PORT);


}

void loop() {
    static unsigned long prev_led;
    static unsigned long prev_bcast;

    auto curr = millis();
    if (curr - prev_led >= 3'000) {
        prev_led = curr;
        send_signal();
    }

    if (curr - prev_bcast >= 10'000) {
        prev_bcast = curr;
        broadcast();
    }

    handle_server();
}

/* 스위치를 찾기 위한 broadcast */
static
void broadcast() {
    JsonDoc doc(BUF_SIZE);
    doc["method"] = "get";
    doc["type"] = "name";
    udp.beginPacket(0xFFFFFFFF, SERVER_PORT);
    // udp.beginPacket({192,168,4,1}, SERVER_PORT);
    serializeJson(doc, udp);
    udp.endPacket();
}

static
void handle_server() {
    int packet_size{ udp.parsePacket() };
    if (!packet_size) return;

    JsonDoc doc(BUF_SIZE);

    if (deserializeJson(doc, udp) != DeserializationError::Ok) {
        Serial.println("json deserialization failed!");
        return;
    }

    const char* type = doc["type"];
    if (strcmp(type, "name")) return;

    auto addr = udp.remoteIP();
    for (size_t i = 0; i < addrs_len; ++i) {
        if (addrs[i] == addr) return;
    }
    addrs[addrs_len++] = addr;
    Serial.println(udp.remoteIP());
    Serial.printf("name: %s\n", (const char*)doc["content"]);
}

static
void send_signal() {
    static bool LED_STATE;

    if (!addrs_len) return;

    JsonDoc doc(BUF_SIZE);
    doc["method"] = "set";
    doc["type"] = "onoff";
    doc["content"] = long(LED_STATE);

    for (size_t i = 0; i < addrs_len; ++i) {
        if (!udp.beginPacket(addrs[i], SERVER_PORT)) {
            Serial.println("udp.beginPacket failed!");
            continue;
        }
        serializeJson(doc, udp);
        if (!udp.endPacket()) {
            Serial.println("udp.endPacket failed!");
        }
    }

    // Serial.println(LED_STATE ? "ON!" : "OFF!");
    digitalWrite(LED, LED_STATE ? LOW : HIGH);

    LED_STATE = !LED_STATE;
}

static
void send_ap() {
    if (!addrs_len) return;

    JsonDoc doc(BUF_SIZE);
    doc["method"] = "set";
    doc["type"] = "accesspoint";
    doc["ssid"] = WIFI_SSID;
    doc["password"] = WIFI_PWD;

    while (addrs_len--) {
        const auto& addr = addrs[addrs_len];
        if (!udp.beginPacket(addr, SERVER_PORT)) {
            Serial.println("udp.beginPacket failed!");
            continue;
        }
        serializeJson(doc, udp);
        if (!udp.endPacket()) {
            Serial.println("udp.endPacket failed!");
        }
    }
}

#endif