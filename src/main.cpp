#include <Arduino.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include "config.h"
#include "ir_handler.h"

ESP8266WebServer server(80);

uint64_t getUInt64fromHex(String str) {
    uint64_t accumulator = 0;
    for(uint8_t i = 0; i < 8; ++i) {
        char c = str[i];
        accumulator *= 16;
        if(isdigit(c)) /* '0' .. '9'*/
            accumulator += c - '0';
        else if(isupper(c)) /* 'A' .. 'F'*/
            accumulator += c - 'A' + 10;
        else /* 'a' .. 'f'*/
            accumulator += c - 'a' + 10;

    }

    return accumulator;
}

void ICACHE_FLASH_ATTR restart() {
    server.send(200, "text/html",
                F("<p>Your device is restarting...</p><p id=\"a\">It should come online in 15s</p><script>var a=15;setInterval(function(){a--;if(!a)window.location=\"/\";document.getElementById(\"a\").innerHTML=\"It should come online in \"+a+\"s\";},1000)</script>"));
    delay(500);
    ESP.restart();
}

void ICACHE_FLASH_ATTR handleRoot() {
    server.send(200, "text/html", "<p>IR Remote by RouNdeL</p><p>Version code: " + String(VERSION_CODE) +
                                  "</p><p><a href=\"/restart\">Restart</a></p>");
}

void ICACHE_FLASH_ATTR receiveCode() {
    if(server.method() == HTTP_POST) {
        if(server.hasArg("p")) {
            uint8_t protocol = (uint8_t) server.arg("p").toInt();
            if(protocol == PROTOCOL_RAW36K) {
                PRINTLN("Received raw: "+server.arg("r"));
                if(addRaw(server.arg("r"))){
                    server.send(204);
                } else {
                    server.send(507);
                }
            } else {
                PRINTLN("Received code. protocol:" + server.arg("p") + ", v:" + server.arg("v") + ", support:" +
                        server.arg("s"));
                uint64_t value = getUInt64fromHex(server.arg("v"));
                if(server.hasArg("s")) {
                    uint64_t support = getUInt64fromHex(server.arg("s"));
                    addCode(protocol, support);
                }
                if(addCode(protocol, value)) {
                    server.send(204);
                } else {
                    server.send(507);
                }
            }
        } else {
            server.send(400);
        }
    } else {
        server.send(400);
    }
}

void setup() {
    pinMode(INFO_PIN, OUTPUT);
    pinMode(IR_PIN, OUTPUT);
    irBegin();
    Serial.begin(115200);

    PRINTLN("");
    PRINTLN("|----------------------|");
    PRINTLN("|---- Begin Startup ---|");
    PRINTLN("| IR Remote by RouNdeL |");
    PRINTLN("|----------------------|");

    PRINTLN("Version Name: " + String(VERSION_NAME));
    PRINTLN("Version Code: " + String(VERSION_CODE));
    PRINTLN("Build Date: " + BUILD_DATE);
    PRINTLN("Device Id: " + String(DEVICE_ID));

    WiFiManager manager;
#ifndef SERIAL_DEBUG
    manager.setDebugOutput(0);
#endif /* SERIAL_DEBUG */
    manager.setAPStaticIPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
    manager.setConfigPortalTimeout(300);
    manager.autoConnect(AP_NAME);

    PRINTLN("|-----------------------|");
    PRINTLN("| Start HTTP Web Server |");
    PRINTLN("|-----------------------|");

    server.on("/send_code", HTTP_POST, receiveCode);
    server.on("/restart", restart);
    server.on("/", handleRoot);
    server.begin();

    ArduinoOTA.begin();
}

void loop() {
    ArduinoOTA.handle();
    server.handleClient();

    handleCodes();
}