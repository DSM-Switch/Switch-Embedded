#include "my_server.h"
#include "debug.h"

static
MyServer::Method str2method(const char* str) {
    struct STRnMETHOD {
        const char* str;
        MyServer::Method method;
    };

    static constexpr const
    STRnMETHOD method_table[] PROGMEM {
        {
            .str = "any",
            .method = MyServer::ANY,
        }, {
            .str = "set",
            .method = MyServer::SET,
        }, {
            .str = "get",
            .method = MyServer::GET, 
        }
    };

    if (!str) return MyServer::INVALID;
    
    for (const auto& a: method_table) {
        if (!strcmp(str, a.str)) {
            return a.method;
        }
    }
    return MyServer::INVALID;
}

size_t MyServer::Packet::printTo(Print& p) const {
    return serializeJson(*this, p);
}

MyServer::MyServer(const Action* actions, size_t action_size)
    : _udp{}, _actions{ actions }, _action_size{ action_size }
{

}

bool MyServer::parse_packet(Packet& packet) {
    return deserializeJson(packet, _udp) == DeserializationError::Ok;
}

bool MyServer::handleClient() {
    int packet_size{ _udp.parsePacket() };
    if (!packet_size) return true;

    Packet packet;
    if (!parse_packet(packet)) return false;

    DEBUG_PRINTLN(packet);

    return call_handler(packet);
}

bool MyServer::call_handler(const Packet& packet) {
    Method method = str2method(packet["method"]);
    if (method == Method::INVALID || method == Method::ANY) return false;

    const char* type = packet["type"];
    if (!type) return false;

    for (size_t i = 0; i < _action_size; ++i) {
        const auto& action = _actions[i];
        if ((method == action.method || action.method == ANY)
                && !strcmp(type, action.type)) {
            return action.action(*this, method, packet);
        }
    }

    return false;
}

bool MyServer::sendPacket(const Packet& packet) {
    if (!_udp.beginPacket(remoteIP(), remotePort())) return false;
    serializeJson(packet, _udp);
    return _udp.endPacket();
}