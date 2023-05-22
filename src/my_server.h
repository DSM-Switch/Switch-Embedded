#ifndef _MY_SERVER_H_
#define _MY_SERVER_H_

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

class MyServer {
private:
    using JsonDoc = DynamicJsonDocument;

    static constexpr const
    size_t BUF_SIZE = 1024;
public:
    enum Method {
        ANY, SET, GET, INVALID
    };

    struct Packet: JsonDoc, Printable {
        Packet() : JsonDoc{ BUF_SIZE } { }
        virtual size_t printTo(Print& p) const;
    };

    struct Action {
        Method method;
        const char* type;
        bool (*action)(MyServer& server, Method method, const Packet& packet);
    };

public:
    MyServer(const Action* actions, size_t action_size);

    MyServer(const MyServer& rhs) = delete;
    MyServer(MyServer&& rhs) = delete;
    MyServer& operator=(const MyServer& rhs) = delete;
    MyServer& operator=(MyServer&& rhs) = delete;

    bool begin(uint16_t port) { return _udp.begin(port); }

    bool handleClient();

    uint16_t localPort() const { return _udp.localPort(); }
    IPAddress remoteIP() { return _udp.remoteIP(); }
    uint16_t remotePort() { return _udp.remotePort(); }

    bool sendPacket(const Packet& packet);

    operator bool() const { return _udp; }

private:
    bool parse_packet(Packet& packet);
    bool call_handler(const Packet& packet);

private:
    WiFiUDP _udp;
    const Action* _actions;
    size_t _action_size;
};

#endif /* _MY_SERVER_H_ */