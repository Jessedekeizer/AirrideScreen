#include "SerialManager.h"

SerialManager& serialManager = SerialManager::GetInstance();

SerialManager::SerialManager() {
    long baud = 9600;
    Serial.begin(baud, SERIAL_8N1);
    Serial2.begin(baud, SERIAL_8N1, 27, 22);
}


void SerialManager::clearMessageCallback() {
    messageCallback = nullptr;
    Debug("Callback cleared");
}
void SerialManager::setMessageCallback(std::function<void(String)> callback) {
    Debug("Setting new callback");
    messageCallback = callback;
    Debug("Callback set successfully");
}

void SerialManager::handleIncoming() {
    while (Serial2.available()) {
        char c = Serial2.read();
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

void SerialManager::sendMessage(const String& message) {
    if (debugMode) {
        Serial.println("Serial2 send: " + message);
    }
    Serial2.print(message);
}

void SerialManager::Debug(const String& message) {
    if (debugMode) {
        Serial.println("Debug: " + message);
    }
}