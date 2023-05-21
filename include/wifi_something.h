#ifndef _WIFI_SOMETHING_H_
#define _WIFI_SOMETHING_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>

static
void wifi_connect(const char* ssid, const char *passphrase) {
    WiFi.begin(ssid, passphrase);             // Connect to the network
    Serial.print("\r\nConnecting to ");
    Serial.print(ssid); Serial.println(" ...");

    int i = 0;
    while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
        delay(1000);
        Serial.print('\r');
        Serial.print(++i);
    }

    Serial.println("\r\nConnection established!");  
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}


#endif /* _WIFI_SOMETHING_H_ */