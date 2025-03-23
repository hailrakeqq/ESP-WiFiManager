#include "../lib/ESP_WiFiManager.h"

ESP_WiFiManager::ESP_WiFiManager(){

}

void ESP_WiFiManager::begin(){
    auto wifiCred = getWiFiCredentialsFromEEPROM()
    if(wifiCred != NULL){
        String ssid = wifiCred

    } else {
        //
    }
}

void ESP_WiFiManager::handleClient(){

}

void ESP_WiFiManager::startAP(){

}


bool ESP_WiFiManager::saveWiFiCredentialsToEEPROM(String ssid, String password){
    if( (ssid != "" || ssid != NULL) && (password != "" || password != NULL) )
    {
        EEPROM.writeString(SSID_ADDRESS, ssid);
        EEPROM.writeString(PASSWORD_ADDRESS, password);
        EEPROM.commit();
        return true;
    }
    return false;
}

WiFiCredential* ESP_WiFiManager::getWiFiCredentialsFromEEPORM(){
    WiFiCredential wificred;
    wificred.ssid = EEPROM.readString(SSID_ADDRESS);
    wificred.password = EEPROM.readString(PASSWORD_ADDRESS);

    if(wificred.ssid != NULL && wificred.password != NULL)
        return &wificred;

    return NULL;
}
