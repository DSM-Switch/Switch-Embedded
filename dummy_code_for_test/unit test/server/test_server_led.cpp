#if 0

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include "wifi_something.h"

static void handleRoot();              // function prototypes for HTTP handlers
static void handleLED();
static void handleNotFound();

static ESP8266WebServer server(80);
static bool led_state;

static
void setup_server() {
    server.on("/", HTTP_GET, handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
    server.on("/LED", HTTP_POST, handleLED);  // Call the 'handleLED' function when a POST request is made to URI "/LED"
    server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

    server.begin();                           // Actually start the server
    Serial.println("HTTP server started");
}

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    wifi_connect();
    setup_server();
}

void loop() {
    server.handleClient();                    // Listen for HTTP requests from clients

    static unsigned long prev;
    auto curr = millis();

    if (curr - prev >= 1000) {
        prev = curr;
        Serial.println(led_state ? "on!" : "off!");
    }
}

static
void handleRoot() {                         // When URI / is requested, send a web page with a button to toggle the LED
    server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
}

static
void handleLED() {                            // If a POST request is made to URI /LED
    led_state = !led_state;                   // Change the state of the LED
    server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
    server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

static
void handleNotFound(){
    server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

#endif