#pragma once
#include <Arduino.h>

class SerialManager {
public:
    SerialManager();
    ~SerialManager();
    void SetDebugMode(bool debug) { debugMode = debug; }
    void Debug(const String& message);
    void Begin(bool debug);

private:

    bool debugMode = false;
};

extern SerialManager serialManager;