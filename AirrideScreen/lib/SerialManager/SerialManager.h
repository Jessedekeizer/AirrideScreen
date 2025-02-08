#pragma once
#include <Arduino.h>

class SerialManager {
private:
    SerialManager();
    bool debugMode = false;
    String incomingMessage;
    std::function<void(String)> messageCallback = nullptr;
    
public:
    static SerialManager& GetInstance() {
        static SerialManager instance;
        return instance;
    }
    
    SerialManager(const SerialManager&) = delete;
    SerialManager& operator=(const SerialManager&) = delete;

    void setDebugMode(bool debug) { debugMode = debug; }
    void sendMessage(const String& message);
    void Debug(const String& message);
    void handleIncoming();
    void setMessageCallback(std::function<void(String)> callback);
    void clearMessageCallback();
};

extern SerialManager& serialManager;