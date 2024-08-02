#include "Logger.h"

void Logger::log(const String& message) {
    if (logs.size() >= maxLogSize) {
        logs.erase(logs.begin());
    }
    logs.push_back(message);
    Serial.println(message);
}

std::vector<String> Logger::getLogs() const {
    return logs;
}
