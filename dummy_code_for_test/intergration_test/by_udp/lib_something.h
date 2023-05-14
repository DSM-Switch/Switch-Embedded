#ifndef _LIB_SOMETHING_H_
#define _LIB_SOMETHING_H_

// header =========================================================

#include <ArduinoJson.h>

// type declaration ===============================================

using JsonDoc = DynamicJsonDocument;

struct Action {
    const char* type;
    bool (*action)(const JsonDoc& doc);
};

enum class Method {
    SET, GET, INVALID
};

// function declaration ===========================================

Method str2method(const char* str);

#endif /* _LIB_SOMETHING_H_ */