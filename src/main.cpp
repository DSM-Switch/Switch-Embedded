#include <Arduino.h>
#include <ESP8266WiFi.h>        // Include the Wi-Fi library

#include "private_settings.h"

void setup() {
    Serial.begin(115200);         // Start the Serial communication to send messages to the computer
    delay(10);
    Serial.println('\n');
    
    auto stat = WiFi.begin(WIFI_SSID, WIFI_PWD);             // Connect to the network
    Serial.print("stat: "); Serial.println(stat);
    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID); Serial.println(" ...");

    int i = 0;
    while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
        delay(1000);
        Serial.print(++i); Serial.print(' ');
    }

    Serial.println('\n');
    Serial.println("Connection established!");  
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}

void loop() { }