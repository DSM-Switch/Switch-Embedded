#if 1

// header =========================================================

#include "private_settings.h"
#include "settings.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "my_server.h"
#include "debug.h"

// macro ==========================================================

#define BUF_SIZE 1024

#define LED LED_BUILTIN

#define AP_SSID "esp8266"
// #define AP_PSWD "qwerty1234"
#define AP_PSWD ""

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

static WiFiEventHandler gotIpEventHandler;
static WiFiEventHandler disconnectedEventHandler;

static constexpr
const MyServer::Action actions[] PROGMEM {
    /* 스위치의 이름 */ {
        .method = MyServer::ANY,
        .type   = "name",
        .action = handle_name
    },
    /* 스위치가 연결될 AP를 설정 */ {
        .method = MyServer::SET,
        .type   = "accesspoint",
        .action = handle_ap,
    },
    /* 스위치의 on/off */ {
        .method = MyServer::ANY,
        .type   = "onoff",
        .action = handle_onoff,
    },
};

static MyServer server(actions, size(actions));

// function definition ============================================

static
void station_disconnected() {
    bool ap_success = WiFi.softAP(AP_SSID, AP_PSWD);
    bool server_success = server || server.begin(SERVER_PORT);
    (void)server_success;
    DEBUG_PRINTF("softAP: %s\n", ap_success ? "success" : "fail");
    DEBUG_PRINTF("server.begin: %s", server_success ? "success" : "fail");
    if (ap_success) disconnectedEventHandler = WiFiEventHandler();
}

static
void station_mode_got_ip(const WiFiEventStationModeGotIP& event) {
    bool disconnect_success = WiFi.softAPdisconnect();
    bool server_begin_success = server || server.begin(SERVER_PORT);
    (void)disconnect_success;
    (void)server_begin_success;

    DEBUG_PRINTF("softAPdisconnect: %s\n", disconnect_success ? "success" : "fail");
    DEBUG_PRINTF("server.begin: %s", server_begin_success ? "success" : "fail");

    auto handler = [] (const auto& event) { station_disconnected(); };
    disconnectedEventHandler = WiFi.onStationModeDisconnected(handler);
}

static
void setup_wifi() {
    gotIpEventHandler = WiFi.onStationModeGotIP(station_mode_got_ip);

    if (WiFi.begin() != WL_CONNECTED) {
        station_disconnected();
    }
}

void setup() {
    Serial.begin(115200);
#ifdef DEBUG
    // Serial.setDebugOutput(true);
#endif
    delay(10);
    Serial.println();

    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

    setup_wifi();

    if (!LittleFS.begin()) {
        DEBUG_PRINTLN("An Error has occurred while mounting LittleFS");
    }

    File file = LittleFS.open("/name", "r");
    if (!file) {
        if (!(file = LittleFS.open("/name", "w"))) {
            DEBUG_PRINTLN("file(/name) open failed!");
        } else {
            file.write("default_name");
            file.close();
        }
    } else {
        file.close();
    }

    if (!WiFi.setSleepMode(WIFI_LIGHT_SLEEP, DTIM)) {
        DEBUG_PRINTLN("setSleepMode failed!");
    }
}

void loop() {
    if (!server.handleClient()) {
        DEBUG_PRINTLN("handle client failed!");
    }

    digitalWrite(LED, LED_STATE ? LOW : HIGH);
}

static
bool handle_name(MyServer& server, MyServer::Method method, const MyServer::Packet& packet) {
    if (method == MyServer::SET) {
        const char* name = packet["content"];
        if (!name) return false;
        
        File file = LittleFS.open("/name", "w");
        if (!file) return false;
        file.write(name);
        file.close();
        
        return true;
    } else if (method == MyServer::GET) {
        File file = LittleFS.open("/name", "r");
        if (!file) return false;
        
        MyServer::Packet spacket;
        spacket["content"] = file.readString();
        file.close();
        spacket["type"] = "name";

        return server.sendPacket(spacket);
    }

    return false;
}

static
bool handle_ap(MyServer& server, MyServer::Method method, const MyServer::Packet& packet) {
    const char* ssid = packet["ssid"];
    const char* password = packet["password"];

    if (!ssid) return false;

    bool oldPersistent = WiFi.getPersistent();
    WiFi.persistent(true);
    WiFi.begin(ssid, password);
    WiFi.persistent(oldPersistent);

    return true;
}

static
bool handle_onoff(MyServer& server, MyServer::Method method, const MyServer::Packet& packet) {
    if (method == MyServer::SET) {
        LED_STATE = long(packet["content"]);
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