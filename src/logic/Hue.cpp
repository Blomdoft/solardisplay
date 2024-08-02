#include "Hue.h"
#include <ArduinoJson.h>
#include "../framework/Logger.h"

#define HUE_API_PATH "/api"

Hue::Hue() : bridgeRegistered(false) {}

void Hue::begin() {
    preferences.begin("hue", false);
    loadStoredBridgeIP();
}

void Hue::loop() {
    // Placeholder for any periodic tasks, if needed
}

void Hue::scanHueBridges() {
    for (int i = 1; i < 255; i++) {
        String ip = "192.168.1." + String(i);
        if (isHueBridge(ip.c_str())) {
            registerWithHueBridge(ip.c_str());
            if (bridgeRegistered) break;
        }
    }
    if (bridgeRegistered) {
        saveStoredBridgeIP(bridgeIP.c_str(), username.c_str());
    }
}

bool Hue::isHueBridge(const char* ip) {
    HTTPClient http;
    http.begin(String("http://") + ip + HUE_API_PATH + "/config");
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        const char* name = doc["name"];
        if (name != nullptr && String(name).startsWith("Philips hue")) {
            http.end();
            return true;
        }
    }
    http.end();
    return false;
}

void Hue::registerWithHueBridge(const char* bridgeIP) {
    HTTPClient http;
    String url = String("http://") + bridgeIP + HUE_API_PATH;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    String body = "{\"devicetype\":\"my_hue_app#esp32\"}";
    int httpCode = http.POST(body);

    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        if (doc[0]["success"].containsKey("username")) {
            const char* usernameChar = doc[0]["success"]["username"];
            username = String(usernameChar);
            this->bridgeIP = String(bridgeIP);
            bridgeRegistered = true;
        }
    }
    http.end();
}

void Hue::listSmartOutlets() {
    if (!bridgeRegistered) return;

    HTTPClient http;
    String url = String("http://") + bridgeIP + HUE_API_PATH + "/" + username + "/lights";
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Logger::getInstance().log(payload); // Replace with actual data parsing and handling
    }
    http.end();
}

void Hue::switchOutlet(const char* outletId, bool state) {
    if (!bridgeRegistered) return;

    HTTPClient http;
    String url = String("http://") + bridgeIP + HUE_API_PATH + "/" + username + "/lights/" + outletId + "/state";
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    String body = String("{\"on\":") + (state ? "true" : "false") + "}";
    int httpCode = http.PUT(body);

    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Logger::getInstance().log(payload); // Replace with actual response handling
    }
    http.end();
}

void Hue::loadStoredBridgeIP() {
    bridgeIP = preferences.getString("bridge_ip", "");
    username = preferences.getString("username", "");
    bridgeRegistered = !bridgeIP.isEmpty() && !username.isEmpty();
}

void Hue::saveStoredBridgeIP(const char* ip, const char* username) {
    preferences.putString("bridge_ip", ip);
    preferences.putString("username", username);
    this->bridgeIP = String(ip);
    this->username = String(username);
    bridgeRegistered = true;
}

void Hue::clearStoredBridgeIP() {
    preferences.remove("bridge_ip");
    preferences.remove("username");
    bridgeIP = "";
    username = "";
    bridgeRegistered = false;
}

bool Hue::isBridgeRegistered() {
    return bridgeRegistered;
}

const char* Hue::getStoredBridgeIP() {
    return bridgeIP.c_str();
}

const char* Hue::getStoredUsername() {
    return username.c_str();
}
