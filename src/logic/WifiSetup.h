#pragma once

#include <WiFi.h>
#include <Preferences.h>
#include <vector>
#include "../framework/Logger.h"

class WiFiSetup {
public:
    WiFiSetup();
    void begin();
    void loop();
    bool isConnected();
    std::vector<String> getScannedSSIDs();
    void connectToNetwork(const char* ssid, const char* password);

private:
    Preferences preferences;
    String ssid;
    String password;
    std::vector<String> availableSSIDs;
    enum WiFiState {
        DISCONNECTED,
        CONNECTING,
        CONNECTED
    } state;
    unsigned long connectStartTime;

    void scanNetworks();
    void tryConnect();
    void reconnect();
    void storeCredentials(const char* ssid, const char* password);
    bool loadStoredCredentials();
};
