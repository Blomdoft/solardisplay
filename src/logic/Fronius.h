#ifndef FRONIUS_H
#define FRONIUS_H

#include <HTTPClient.h>
#include <Preferences.h>
#include <Arduino.h>


struct Inverter {
    String ip;
    bool isAlive;
};

class Fronius {
public:
    Fronius();
    void begin();
    void loop();
    void scanInverters();
    bool isFroniusInverter(const char* ip);
    void queryInverterData(const char* ip);
    void addOrUpdateInverter(const char* ip, bool isAlive);
    void loadStoredInverterIPs();
    void saveStoredInverterIPs();
    void clearStoredInverterIPs();
    Inverter* getInverters();
    int getInverterCount();
    void startInverterScan();
    bool isScanningInverters();

private:
    Inverter inverters[10];
    int inverterCount;
    bool scanning;
    int currentScanIP;
    Preferences preferences;
};

#endif
