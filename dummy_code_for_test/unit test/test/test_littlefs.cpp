#if 0

#include <Arduino.h>
#include <LittleFS.h>

void setup() {
    delay(10);
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(10);
    Serial.println("yeeeeeeeeeee");

    if (!LittleFS.begin()) {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }

    /* write*/
    File file = LittleFS.open("/text.txt", "w");
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }
    file.write("hello, world!");
    file.close();

    /* read */
    file = LittleFS.open("/text.txt", "r");
    if (!file) {
        Serial.println("Failed to open file reading");
        return;
    }
    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();
}

void loop() { }

#endif