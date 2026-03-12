#include "SerialManager.h"

SerialManager serialManager;

SerialManager::SerialManager() {
}

SerialManager::~SerialManager() {
}

void SerialManager::Begin(bool debug) {
    debugMode = debug;
    Serial.begin(9600, SERIAL_8N1);
}


void SerialManager::Debug(const String &message) {
    if (debugMode) {
        Serial.println("Debug: " + message);
    }
}
