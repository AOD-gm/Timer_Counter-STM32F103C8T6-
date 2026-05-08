#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <Ticker.h>
#include <DS1306.h>
// biến global (extern)
extern String ssid;
extern String password;

extern int wifiMode;

extern ESP8266WebServer webServer;
extern Ticker blinker;

extern int ledPin;
extern int btnPin;

#define EN_PIN GPIO_PIN_10
#define EN_PORT GPIOB
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