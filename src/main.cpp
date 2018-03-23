#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <FS.h>

const char* ap_ssid = "C1C";
const char* ap_pass = "dgrrew328sfe@";
const char* OTAhost = "c1c";

String statuss = "80";
String shutter = "open";
String projFreeze = "nope";
int8 powerChecker = 0;
u32 timing;
String comanda = "";
bool waitAnswer = false;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
SoftwareSerial softSerial(14, 12, false, 256);

void saveStatus(String cmd, String stat) {
    if(cmd == "CR0")
        statuss = stat;
    else if(cmd == "C0E")
        shutter = "open";
    else if(cmd == "C0D")
        shutter = "close";
    else if(cmd == "C43")
        projFreeze = "nope";
    else if(cmd == "C44")
        projFreeze = "freez";
}

void sendCmdToDevice(char* data) {
    comanda = String(data);
    softSerial.printf("%s\r", data);
    waitAnswer = true;
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
    if(type == WS_EVT_CONNECT) {
        //client connected
        Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
        client->printf("Stats: %s %s %s", statuss.c_str(), shutter.c_str(), projFreeze.c_str());
        client->printf("Hello Client %u :)", client->id());
        client->ping();
    } else if(type == WS_EVT_DISCONNECT){
        //client disconnected
        Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
    } else if(type == WS_EVT_ERROR){
        //error was received from the other end
        Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
    } else if(type == WS_EVT_PONG){
        //pong message was received (in response to a ping request maybe)
        Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
    } else if(type == WS_EVT_DATA) {
        //data packet
        AwsFrameInfo * info = (AwsFrameInfo*)arg;
        if(info->final && info->index == 0 && info->len == len){
            //the whole message is in a single frame and we got all of it's data
            if(info->opcode == WS_TEXT){
                data[len] = 0;
                if(strcmp((char*)data, "power") == 0) { 
                    if(statuss == "80" || statuss == "04")
                        sendCmdToDevice((char*)"C00");
                    else if(statuss == "00")
                        sendCmdToDevice((char*)"C01");
                    delay(1000);
                    powerChecker = 1;
                } else { 
                    sendCmdToDevice((char*)data);
                }
            }
        } else {
            //message is comprised of multiple frames or the frame is split into multiple packets
            client->printf("too much data");
        }
    }
}

void setup() {
    delay(1000);
    Serial.begin(115200);
    softSerial.begin(19200);

    WiFi.persistent(false);
    WiFi.mode(WIFI_AP);
    delay(10);
    WiFi.setPhyMode(WIFI_PHY_MODE_11B);
    WiFi.softAP(ap_ssid, ap_pass, 7);

    SPIFFS.begin();

    ws.onEvent(onEvent);
    server.addHandler(&ws);

    server.serveStatic("/", SPIFFS, "/i").setDefaultFile("index.htm");
    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404);
    });
    server.begin();

    ArduinoOTA.setHostname(OTAhost);
    ArduinoOTA.onStart([]() {
      SPIFFS.end();
      Serial.end();
      softSerial.end();
      ws.enable(false);
      ws.closeAll();          
    });
    ArduinoOTA.begin();
}

void loop() {
    ArduinoOTA.handle();

    if(powerChecker > 0 && powerChecker < 3 && millis() - timing > 15000) {
        timing = millis();
        delay(1000);
        sendCmdToDevice((char*)"CR0");
        powerChecker++;
    }

    if(waitAnswer) {
        String answer = "noans";
        String serialText = "nil";
        while(softSerial.available() == 0) {
            delay(250);
        }
        if(softSerial.available() > 0) {
            serialText = softSerial.readStringUntil('\r');
            if(serialText == "\u0006") serialText = "<i class='icon-ok-circled'></i>";
            answer = comanda;
            answer += ": ";
            answer += serialText;
        }
        saveStatus(comanda, serialText);
        ws.textAll(answer);
        waitAnswer = false;
    }
}