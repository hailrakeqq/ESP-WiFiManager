#ifndef ESP_WIFIMANAGER_H
#define ESP_WIFIMANAGER_H

#include "Arduino.h"
#include "EEPROM.h"
#include <WebServer.h>

#define EEPROM_SIZE 96
#define SSID_ADDR 0
#define PASS_ADDR 32

// #define ESP8266
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#endif

#ifndef ESP8266
#include "WiFi.h"
#endif

struct WiFiCredential
{
    String ssid;
    String password;
};

class WiFiManager
{
public:
    WiFiManager(const char *apName = "ESP32_ConfigAP", const char *apPassword = nullptr);
    void begin();
    bool isConnected();
    String getSSID();
    String getPassword();

private:
    const char *_apName;
    const char *_apPassword;
    WebServer server;

    void startAPMode();
    void handleRoot();
    void handleSave();
    void loadCredentials(String &ssid, String &pass);
    void saveCredentials(const String &ssid, const String &pass);
    void connectToWiFi(const String &ssid, const String &pass);
};

#endif
