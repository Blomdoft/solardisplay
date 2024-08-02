#ifndef HUE_H
#define HUE_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <Preferences.h>

class Hue {
public:
    Hue();
    void begin();
    void loop();
    void scanHueBridges();
    bool isHueBridge(const char* ip);
    void registerWithHueBridge(const char* bridgeIP);
    void listSmartOutlets();
    void switchOutlet(const char* outletId, bool state);
    void loadStoredBridgeIP();
    void saveStoredBridgeIP(const char* ip, const char* username);
    void clearStoredBridgeIP();
    bool isBridgeRegistered();
    const char* getStoredBridgeIP();
    const char* getStoredUsername();

private:
    String bridgeIP;
    String username;
    bool bridgeRegistered;
    Preferences preferences;
};

#endif
