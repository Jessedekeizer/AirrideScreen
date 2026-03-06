#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H
#include <Arduino.h>

class SerialManager {
private:
    SerialManager();
    bool debugMode = false;
    
public:
    static SerialManager& GetInstance() {
        static SerialManager instance;
        return instance;
    }
    
    SerialManager(const SerialManager&) = delete;
    SerialManager& operator=(const SerialManager&) = delete;

    void setDebugMode(bool debug) { debugMode = debug; }
    void Debug(const String& message);
};

extern SerialManager& serialManager;
#endif