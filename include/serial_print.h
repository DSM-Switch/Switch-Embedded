#ifndef _SERIAL_PRINT_H_
#define _SERIAL_PRINT_H_

#include <Arduino.h>

template <typename T>
void serial_print(T&& a) {
    Serial.print(std::forward<T>(a));
}

template <typename T, typename ... Types>
void serial_print(T&& a, Types&& ... args) {
    Serial.print(std::forward<T>(a));
    serial_print(std::forward<Types>(args)...);
}

template <typename ... Types>
void serial_println(Types&& ... args) {
    serial_print(std::forward<Types>(args)...);
    Serial.println();
}

#endif /* _SERIAL_PRINT_H_ */