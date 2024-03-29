#if 0

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include "wifi_something.h"

static void handleRoot();              // function prototypes for HTTP handlers
static void handleLogin();
static void handleNotFound();

static ESP8266WebServer server(80);

static
void setup_server() {
    server.on("/", HTTP_GET, handleRoot);        // Call the 'handleRoot' function when a client requests URI "/"
    server.on("/login", HTTP_POST, handleLogin); // Call the 'handleLogin' function when a POST request is made to URI "/login"
    server.onNotFound(handleNotFound);           // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

    server.begin();                            // Actually start the server
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
}

void handleRoot() {                          // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html", "<form action=\"/login\" method=\"POST\"><input type=\"text\" name=\"username\" placeholder=\"Username\"></br><input type=\"password\" name=\"password\" placeholder=\"Password\"></br><input type=\"submit\" value=\"Login\"></form><p>Try 'John Doe' and 'password123' ...</p>");
}

void handleLogin() {                         // If a POST request is made to URI /login
    if( ! server.hasArg("username") || ! server.hasArg("password") 
            || server.arg("username") == NULL || server.arg("password") == NULL) { // If the POST request doesn't have username and password data
        server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
        return;
    }
    if (server.arg("username") == "John Doe" && server.arg("password") == "password123") { // If both the username and the password are correct
        server.send(200, "text/html", "<h1>Welcome, " + server.arg("username") + "!</h1><p>Login successful</p>");
    } else {                                                                              // Username and password don't match
        server.send(401, "text/plain", "401: Unauthorized");
    }
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

#endif