#include "SerialManager.h"

SerialManager serialManager;

SerialManager::SerialManager() {
}

SerialManager::~SerialManager() {
    ClearMessageCallback();
}

void SerialManager::Begin(bool debug) {
    debugMode = debug;
    Serial.begin(9600, SERIAL_8N1);
    Serial1.begin(9600, SERIAL_8N1);
    Serial1.setTimeout(20);
}


void SerialManager::ClearMessageCallback() {
    messageCallback = nullptr;
    Debug("Callback cleared");
}

void SerialManager::SetMessageCallback(std::function<void(String)> callback) {
    Debug("Setting new callback");
    messageCallback = callback;
    Debug("Callback set successfully");
}

void SerialManager::HandleIncoming() {
    while (Serial1.available()) {
        char c = Serial1.read();
        if (c == '\n' || c == '\r') {
            if (incomingMessage.length() > 0) {
                Debug("Received: " + incomingMessage);
                if (messageCallback) {
                    Debug("Executing callback");
                    messageCallback(incomingMessage);
                } else {
                    Debug("No callback set (ptr: null)");
                }
                incomingMessage = "";
            }
        } else {
            incomingMessage += c;
        }
    }
}

void SerialManager::SendMessage(const String &message) {
    if (debugMode) {
        Serial.println("Serial1 send: " + message);
    }
    Serial1.println(message);
}

void SerialManager::Debug(const String &message) {
    if (debugMode) {
        Serial.println("Debug: " + message);
    }
}
