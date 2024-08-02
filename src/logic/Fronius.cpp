#include "Fronius.h"
#include "../framework/Logger.h"

#define FRONIUS_API_PATH "/solar_api/v1/GetPowerFlowRealtimeData.fcgi"


Fronius::Fronius() : inverterCount(0), scanning(false), currentScanIP(1) {}

void Fronius::begin() {
    preferences.begin("inverters", false);
    loadStoredInverterIPs();
}

void Fronius::loop() {
    if (scanning) {
        scanInverters();
    } else {
        for (int i = 0; i < inverterCount; i++) {
            queryInverterData(inverters[i].ip.c_str());
        }
    }
}

void Fronius::scanInverters() {
    if (currentScanIP < 255) {
        String ip = "192.168.1." + String(currentScanIP);
        if (isFroniusInverter(ip.c_str())) {
            addOrUpdateInverter(ip.c_str(), true);
        }
        currentScanIP++;
    } else {
        scanning = false;
        saveStoredInverterIPs();
    }
}

bool Fronius::isFroniusInverter(const char* ip) {
    HTTPClient http;
    http.begin(String("http://") + ip + FRONIUS_API_PATH);
    int httpCode = http.GET();
    http.end();
    return httpCode == HTTP_CODE_OK;
}

void Fronius::queryInverterData(const char* ip) {
    HTTPClient http;
    String url = String("http://") + ip + FRONIUS_API_PATH;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Logger::getInstance().log(payload); // Replace with actual data parsing and handling
    }
    http.end();
}

void Fronius::addOrUpdateInverter(const char* ip, bool isAlive) {
    for (int i = 0; i < inverterCount; i++) {
        if (inverters[i].ip == ip) {
            inverters[i].isAlive = isAlive;
            return;
        }
    }
    if (inverterCount < 10) {
        inverters[inverterCount].ip = String(ip);
        inverters[inverterCount].isAlive = isAlive;
        inverterCount++;
    }
}

void Fronius::loadStoredInverterIPs() {
    inverterCount = preferences.getUInt("count", 0);
    for (int i = 0; i < inverterCount; i++) {
        char key[10];
        snprintf(key, sizeof(key), "ip%d", i);
        inverters[i].ip = preferences.getString(key, "");
        inverters[i].isAlive = true; // Assume alive on startup
    }
}

void Fronius::saveStoredInverterIPs() {
    preferences.putUInt("count", inverterCount);
    for (int i = 0; i < inverterCount; i++) {
        char key[10];
        snprintf(key, sizeof(key), "ip%d", i);
        preferences.putString(key, inverters[i].ip);
    }
}

void Fronius::clearStoredInverterIPs() {
    inverterCount = 0;
    preferences.clear();
}

Inverter* Fronius::getInverters() {
    return inverters;
}

int Fronius::getInverterCount() {
    return inverterCount;
}

void Fronius::startInverterScan() {
    scanning = true;
    currentScanIP = 1;
}

bool Fronius::isScanningInverters() {
    return scanning;
}
