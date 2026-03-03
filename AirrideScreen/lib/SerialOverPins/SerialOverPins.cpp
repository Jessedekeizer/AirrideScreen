#include "SerialOverPins.h"

#include <HardwareSerial.h>

void SerialOverPins::Init(int baudrate) {
    Serial2.begin(baudrate, SERIAL_8N1, 27, 22);
}

void SerialOverPins::SendMessage(String message) {
    Serial2.println(message);
}

bool SerialOverPins::ReceiveAvailable() {
    return Serial2.available();
}

String SerialOverPins::Receive() {
    String message = "";
    while (Serial2.available()) {
        char c = Serial2.read();
        if (c == '\n' || c == '\r') {
            if (message.length() > 0) {
                return message;
            }
        } else {
            message += c;
        }
    }
}

