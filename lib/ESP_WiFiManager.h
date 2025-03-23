#ifndef ESP_WIFIMANAGER_H
#define ESP_WIFIMANAGER_H


#include "EEPROM.h"
#define SSID_ADDRESS 0
#define PASSWORD_ADDRESS 64

#define ESP8266
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

class ESP_WiFiManager {
public:
    ESP_WiFiManager();
    void begin();
    void handleClient();
    
private:
    void startAP();
    bool saveWiFiCredentialsToEEPROM(String ssid, String password);
    WiFiCredential* getWiFiCredentialsFromEEPORM();
    WiFiCredential _wifiCred;
};

#endif
