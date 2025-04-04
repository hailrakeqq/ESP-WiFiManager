#include "../include/ESP_WiFiManager.h"

WiFiManager::WiFiManager(const char *apName, const char *apPassword)
    : _apName(apName), _apPassword(apPassword), server(80) {}

void WiFiManager::begin()
{
    EEPROM.begin(EEPROM_SIZE);
    String ssid, pass;
    loadCredentials(ssid, pass);

    if (ssid.length() > 0 && pass.length() > 0)
    {
        connectToWiFi(ssid, pass);
        unsigned long startAttemptTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000)
        {
            delay(100);
        }
        if (WiFi.status() == WL_CONNECTED)
            return;
    }

    startAPMode();
}

bool WiFiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getSSID()
{
    return WiFi.SSID();
}

String WiFiManager::getPassword()
{
    String pass;
    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0; i < 32; ++i)
    {
        char c = EEPROM.read(PASS_ADDR + i);
        if (c == 0)
            break;
        pass += c;
    }
    return pass;
}

void WiFiManager::loadCredentials(String &ssid, String &pass)
{
    char ssidBuff[33], passBuff[33];
    for (int i = 0; i < 32; ++i)
    {
        ssidBuff[i] = EEPROM.read(SSID_ADDR + i);
        passBuff[i] = EEPROM.read(PASS_ADDR + i);
    }
    ssidBuff[32] = passBuff[32] = 0;
    ssid = String(ssidBuff);
    pass = String(passBuff);
}

void WiFiManager::saveCredentials(const String &ssid, const String &pass)
{
    for (int i = 0; i < 32; ++i)
    {
        EEPROM.write(SSID_ADDR + i, i < ssid.length() ? ssid[i] : 0);
        EEPROM.write(PASS_ADDR + i, i < pass.length() ? pass[i] : 0);
    }
    EEPROM.commit();
}

void WiFiManager::connectToWiFi(const String &ssid, const String &pass)
{
    WiFi.begin(ssid.c_str(), pass.c_str());
}

void WiFiManager::startAPMode()
{
    WiFi.softAP(_apName, _apPassword);
    server.on("/", [this]()
              { handleRoot(); });
    server.on("/save", [this]()
              { handleSave(); });
    server.begin();
    while (true)
    {
        server.handleClient();
        delay(10);
    }
}

void WiFiManager::handleRoot()
{
    String html = R"rawliteral(
        <html><body>
        <form action="/save" method="post">
        SSID:<br><input type="text" name="ssid"><br>
        Password:<br><input type="password" name="pass"><br>
        <input type="submit" value="Save">
        </form></body></html>
    )rawliteral";
    server.send(200, "text/html", html);
}

void WiFiManager::handleSave()
{
    if (server.hasArg("ssid") && server.hasArg("pass"))
    {
        String ssid = server.arg("ssid");
        String pass = server.arg("pass");
        saveCredentials(ssid, pass);
        server.send(200, "text/html", "<html><body><h1>Saved! Rebooting...</h1></body></html>");
        delay(3000);
        ESP.restart();
    }
    else
    {
        server.send(400, "text/plain", "Missing ssid/pass");
    }
}
