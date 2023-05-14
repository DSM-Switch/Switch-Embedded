/*
UDP echo client
*/

#if 1

// header =========================================================

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "wifi_something.h"
#include "lib_something.h"

// macro ==========================================================

#define BUF_SIZE 1024

// function declaration ===========================================

static void handle_server();
static void send_signal();

// variable definition ============================================

static WiFiUDP udp;

static IPAddress addrs[4];
static size_t addrs_len;

// function definition ============================================

void setup() {
    Serial.begin(115200);
    delay(10);
    
    wifi_connect();

    udp.begin(UDP_PORT);

    /* 스위치를 찾기 위한 broadcast */
    JsonDoc doc(BUF_SIZE);
    doc["method"] = "get";
    doc["type"] = "ip";
    udp.beginPacket(0xFFFFFFFF, UDP_PORT);
    serializeJson(doc, udp);
    udp.endPacket();
}

void loop() {
    static unsigned long prev;
    auto curr = millis();
    if (curr - prev >= 10'000) {
        prev = curr;
        send_signal();
    }

    handle_server();
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
    if (!strcmp(type, "ip")) {
        const char* ip_str = doc["content"];
        IPAddress ip;
        if (ip.fromString(ip_str)) {
            addrs[addrs_len++] = std::move(ip);
        } else {
            Serial.println("invalid ip address!");
            return;
        }
    }
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
        udp.beginPacket(addrs[i], UDP_PORT);
        serializeJson(doc, udp);
        udp.endPacket();
    }

    LED_STATE = !LED_STATE;
}

#endif