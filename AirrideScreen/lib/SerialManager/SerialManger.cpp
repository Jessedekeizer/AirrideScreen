#include "SerialManager.h"

SerialManager& serialManager = SerialManager::GetInstance();

SerialManager::SerialManager() {
    long baud = 9600;
    Serial.begin(baud, SERIAL_8N1);
}

void SerialManager::Debug(const String& message) {
    if (debugMode) {
        Serial.println("Debug: " + message);
    }
}