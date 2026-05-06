#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <Ticker.h>

// biến global (extern)
extern String ssid;
extern String password;

extern int wifiMode;

extern ESP8266WebServer webServer;
extern Ticker blinker;

extern int ledPin;
extern int btnPin;

// function
void setupWifi();
void setupWebServer();
void checkButton();
void ledControl();
void handleSetTime();
void handleSetAlarm();
void handleDevice();
// EEPROM helper
void writeStringToEEPROM(int addr, String data);
String readStringFromEEPROM(int addr, int maxLen);

#endif