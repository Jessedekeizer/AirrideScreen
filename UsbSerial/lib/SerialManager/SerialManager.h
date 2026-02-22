#pragma once
#include <Arduino.h>
#include <functional>

class SerialManager {
public:
    SerialManager();
    ~SerialManager();
    void SetDebugMode(bool debug) { debugMode = debug; }
    void SendMessage(const String& message);
    void Debug(const String& message);
    void HandleIncoming();
    void SetMessageCallback(std::function<void(String)> callback);
    void ClearMessageCallback();
    void Begin(bool debug);

private:

    bool debugMode = false;
    String incomingMessage;
    std::function<void(String)> messageCallback = nullptr;
};

extern SerialManager serialManager;