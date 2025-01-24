#pragma once

#include <Arduino.h>
#include <memory>

class SerialManager
{
public:
    SerialManager(const SerialManager &) = delete;
    SerialManager &operator=(const SerialManager &) = delete;

    static SerialManager &GetInstance()
    {
        static SerialManager instance;
        return instance;
    }

    // void begin(long baud);
    void setDebugMode(bool debug) { debugMode = debug; }
    void sendMessage(const String &message);
    void Debug(const String &message);
    void handleIncoming();
    void setMessageCallback(std::function<void(String)> callback)
    {
        messageCallback = callback;
    }

    void clearMessageCallback()
    {
        messageCallback = nullptr;
    }

private:
    SerialManager();
    bool debugMode = false;
    String incomingMessage;
    std::function<void(String)> messageCallback;
};

extern SerialManager &serialManager;