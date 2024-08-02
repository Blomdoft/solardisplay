#include "WiFiSetup.h"

WiFiSetup::WiFiSetup() : state(DISCONNECTED), connectStartTime(0) {}

void WiFiSetup::begin() {
    Serial.begin(115200);
    preferences.begin("wifi-creds", false);

    if (loadStoredCredentials()) {
        state = CONNECTING;
        connectStartTime = millis();
        Logger::getInstance().log("Managed to get stored credentials.");
    } else {
        state = DISCONNECTED;
    }
}

void WiFiSetup::loop() {
    switch (state) {
        case DISCONNECTED:
            // Optionally start a scan here if needed
            break;
        case CONNECTING:
            tryConnect();
            break;
        case CONNECTED:
            if (WiFi.status() != WL_CONNECTED) {
                Logger::getInstance().log("WiFi disconnected, attempting to reconnect...");
                state = DISCONNECTED;
            }
            break;
    }
}

bool WiFiSetup::isConnected() {
    return state == CONNECTED;
}

std::vector<String> WiFiSetup::getScannedSSIDs() {
    scanNetworks();
    return availableSSIDs;
}

void WiFiSetup::connectToNetwork(const char* ssid, const char* password) {
    WiFi.disconnect();
    storeCredentials(ssid, password);
    this->ssid = ssid;
    this->password = password;
    state = CONNECTING;
    connectStartTime = millis();
}

void WiFiSetup::scanNetworks() {
    Logger::getInstance().log("Scanning for networks...");
    int n = WiFi.scanNetworks();
    availableSSIDs.clear();
    for (int i = 0; i < n; ++i) {
        availableSSIDs.push_back(WiFi.SSID(i));
        Serial.print("Found network: ");
        Logger::getInstance().log(WiFi.SSID(i));
    }
}

void WiFiSetup::tryConnect() {
    if (millis() - connectStartTime > 10000) {
        // Timeout after 10 seconds
        Logger::getInstance().log("Failed to connect to WiFi network (timeout).");
        state = DISCONNECTED;
        return;
    }

    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid.c_str(), password.c_str());
        delay(100); // Brief delay to allow connection attempt
    } else {
        Logger::getInstance().log("Connected to WiFi network.");
        state = CONNECTED;
    }
}

void WiFiSetup::reconnect() {
    if (millis() - connectStartTime > 10000) {
        // Timeout after 10 seconds
        Logger::getInstance().log("Failed to reconnect to WiFi network (timeout).");
        state = DISCONNECTED;
        return;
    }

    if (WiFi.status() != WL_CONNECTED) {
        WiFi.reconnect();
        delay(100); // Brief delay to allow reconnection attempt
    } else {
        Logger::getInstance().log("Reconnected to WiFi network.");
        state = CONNECTED;
    }
}

void WiFiSetup::storeCredentials(const char* ssid, const char* password) {
    Logger::getInstance().log("Storing WiFi credentials...");
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    Logger::getInstance().log("WiFi credentials stored.");
}

bool WiFiSetup::loadStoredCredentials() {
    ssid = preferences.getString("ssid", "");
    password = preferences.getString("password", "");

    Serial.print("Stored SSID: ");
    Logger::getInstance().log(ssid);
    Serial.print("Stored Password: ");
    Logger::getInstance().log(password);

    return !ssid.isEmpty() && !password.isEmpty();
}
