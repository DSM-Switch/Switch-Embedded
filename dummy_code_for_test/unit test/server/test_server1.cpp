#if 0

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include "wifi_something.h"

static ESP8266WebServer server(80);

static void handleRoot();              // function prototypes for HTTP handlers
static void handleNotFound();

void setup() {
    Serial.begin(115200);
    // Serial.setDebugOutput(true);
    Serial.println();

    wifi_connect();

    server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
    server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

    server.begin();                           // Actually start the server
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();                    // Listen for HTTP requests from clients
}

static
void handleRoot() {
    server.send(200, "text/plain", "Hello world!");   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

static
void handleNotFound(){
    server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

#endif