#pragma once

#include <Arduino.h>
#include <vector>
#include <string>

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const String& message);
    std::vector<String> getLogs() const;

private:
    Logger() : maxLogSize(1000) {}

    std::vector<String> logs;
    size_t maxLogSize;
};

