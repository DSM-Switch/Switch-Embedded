#include "lib_something.h"


Method str2method(const char* str) {
    if (!strcmp(str, "set")) {
        return Method::SET;
    } else if (!strcmp(str, "get")) {
        return Method::GET;
    }
    return Method::INVALID;
}