#if 0

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "wifi_something.h"

static WiFiServer server(80);

static
void setup_server() {
    server.begin();
    Serial.println("server started!");
}

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    wifi_connect();
    setup_server();
}


// prepare a web page to be send to a client (web browser)
String prepareHtmlPage()
{
  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation
  htmlPage = F("HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Connection: close\r\n"  // the connection will be closed after completion of the response
               "Refresh: 5\r\n"         // refresh the page automatically every 5 sec
               "\r\n"
               "<!DOCTYPE HTML>"
               "<html>"
               "Analog input:  ");
  htmlPage += analogRead(A0);
  htmlPage += F("</html>"
                "\r\n");
  return htmlPage;
}


void loop()
{
    WiFiClient client = server.accept();
    // wait for a client (web browser) to connect
    if (!client) return;
    Serial.println("\n[Client connected]");

    while (client.connected()) {
        // read line by line what the client (web browser) is requesting
        if (client.available())
        {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
            client.println(prepareHtmlPage());
            break;
        }
        }
    }

    while (client.available()) {
        // but first, let client finish its request
        // that's diplomatic compliance to protocols
        // (and otherwise some clients may complain, like curl)
        // (that is an example, prefer using a proper webserver library)
        client.read();
    }

    // close the connection:
    client.stop();
    Serial.println("[Client disconnected]");
}

#endif