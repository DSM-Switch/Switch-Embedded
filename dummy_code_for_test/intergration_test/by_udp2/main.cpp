/*
UDP server
*/

#if 1

// header =========================================================

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include "wifi_something.h"
#include "my_server.h"
#include <ESP8266WebServer.h>

// macro ==========================================================

#define BUF_SIZE 1024

// function declaration ===========================================

static bool handle_name(MyServer&, MyServer::Method, const MyServer::Packet&);
static bool handle_ap(MyServer&, MyServer::Method, const MyServer::Packet&);
static bool handle_onoff(MyServer&, MyServer::Method, const MyServer::Packet&);

template <typename T, size_t N>
constexpr size_t size(T (&arr)[N]) {
    return N;
}

// variable definition ============================================

static bool LED_STATE;
static char NAME[64] = "default_name";

static constexpr
const MyServer::Action actions[] PROGMEM {
    /* 스위치의 이름 */ {
        MyServer::ANY,
        .type   = "name",
        .action = handle_name
    },
    /* 스위치가 연결될 AP를 설정 */ {
        MyServer::ANY,
        .type   = "accesspoint",
        .action = handle_ap,
    },
    /* 스위치의 on/off */ {
        MyServer::ANY,
        .type   = "onoff",
        .action = handle_onoff,
    },
};

static MyServer server(actions, size(actions));

// function definition ============================================

void setup() {
    Serial.begin(115200);
    // Serial.setDebugOutput(true);
    delay(10);
    Serial.println();

    wifi_connect();
    server.begin(UDP_PORT);
}

void loop() {
    if (!server.handleClient()) {
        Serial.println("handle client failed!");
    }

    static unsigned long prev;
    auto curr = millis();
    if (curr - prev >= 3'000) {
        prev = curr;
        Serial.println(LED_STATE ? "ON!" : "OFF!");
    }
}

static
bool handle_name(MyServer& server, MyServer::Method method, const MyServer::Packet& packet) {
    // Serial.println("call handle_name!");

    if (method == MyServer::SET) {
        strcpy(NAME, packet["content"]);
        return true;
    } else if (method == MyServer::GET) {
        MyServer::Packet spacket;
        spacket["type"] = "name";
        spacket["content"] = NAME;
        return server.sendPacket(spacket);
    }

    return false;
}

static
bool handle_ap(MyServer& server, MyServer::Method method, const MyServer::Packet& packet) {
    return false;
}

static
bool handle_onoff(MyServer& server, MyServer::Method method, const MyServer::Packet& packet) {
    if (method == MyServer::SET) {
        LED_STATE = long(packet["content"]);
        // Serial.println("adsfasdfsadf");
        return true;
    } else if (method == MyServer::GET) {
        MyServer::Packet spacket;
        spacket["type"] = "onoff";
        spacket["content"] = long(LED_STATE);
        return server.sendPacket(spacket);
    }

    return false;
}


#endif