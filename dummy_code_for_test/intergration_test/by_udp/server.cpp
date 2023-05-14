/*
UDP server
*/

#if 0

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
static bool handle(const JsonDoc& doc);
static bool handle_name(const JsonDoc& doc);
static bool handle_ap(const JsonDoc& doc);
static bool handle_onoff(const JsonDoc& doc);
static bool handle_ip(const JsonDoc& doc);

// variable definition ============================================

static bool LED_STATE;

static WiFiUDP udp;

static constexpr
const Action actions[] PROGMEM {
    /* 스위치의 이름 */ {
        .type   = "name",
        .action = handle_name
    },
    /* 스위치가 연결될 AP를 설정 */ {
        .type   = "accesspoint",
        .action = handle_ap,
    },
    /* 스위치의 on/off */ {
        .type   = "onoff",
        .action = handle_onoff,
    },
    /* 스위치의 IP */ {
        .type   = "ip",
        .action = handle_ip,
    },
};

// function definition ============================================

static
void setup_server() {
    udp.begin(UDP_PORT);
}

void setup() {
    Serial.begin(115200);
    // Serial.setDebugOutput(true);
    delay(10);
    Serial.println();
    
    wifi_connect();
    setup_server();
}

void loop() {
    handle_server();

    static unsigned long prev;
    auto curr = millis();
    if (curr - prev >= 3'000) {
        prev = curr;
        Serial.println(LED_STATE ? "ON!" : "OFF!");
    }
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

    if (!handle(doc)) {
        Serial.println("handle failed!");
        return;
    }
}

static
bool handle(const JsonDoc& doc) {
    const char* type = doc["type"];

    for (const Action& act: actions) {
        if (!strcmp(type, act.type)) {
            return act.action(doc);
        }
    }

    return false;
}

static
bool handle_name(const JsonDoc& doc) {
    return true;
}

static
bool handle_ap(const JsonDoc& doc) {
    return true;
}

static
bool handle_onoff(const JsonDoc& doc) {
    Method method = str2method(doc["method"]);
    if (method == Method::INVALID) return false;

    if (method == Method::SET) {
        long onoff = doc["content"];
        LED_STATE = onoff;
    } else if (method == Method::GET) {
        JsonDoc doc(BUF_SIZE);
        doc["type"] = "onoff";
        doc["content"] = long(LED_STATE);

        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        serializeJson(doc, udp);
        udp.endPacket();
    }

    return true;
}

static
bool handle_ip(const JsonDoc& doc) {
    Method method = str2method(doc["method"]);
    if (method == Method::INVALID) return false;
    
    if (method == Method::GET) {
        JsonDoc doc(BUF_SIZE);
        doc["type"] = "ip";
        doc["content"] = WiFi.localIP().toString();

        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        serializeJson(doc, udp);
        udp.endPacket();
    }

    return true;
}

#endif