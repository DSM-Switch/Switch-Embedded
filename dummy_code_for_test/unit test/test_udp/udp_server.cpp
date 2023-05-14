/*
UDP echo server
*/

#if 0

#include <Arduino.h>
#include <WiFiUdp.h>
#include "wifi_something.h"

#define UDP_PORT 56789

static WiFiUDP udp;
static char incoming_packet[256]; // 수신되는 패킷
static const char* reply_packet = "hello from server\r\n";

void setup() {
    Serial.begin(115200);
    delay(10);
    
    wifi_connect();

    udp.begin(UDP_PORT); // 들어오는 패킷을 듣기 시작
}

void loop() {
    int packet_size{ udp.parsePacket() };
    if (!packet_size) return;

    Serial.printf("Received %d bytes from %s, port %u\n",
        packet_size, udp.remoteIP().toString().c_str(), (unsigned)udp.remotePort());
    auto len = udp.read(incoming_packet, sizeof(incoming_packet) - 1);
    incoming_packet[len] = '\0';
    Serial.printf("UDP packet contents: %s\r\n", incoming_packet);

    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write(reply_packet);
    udp.endPacket();
}

#endif