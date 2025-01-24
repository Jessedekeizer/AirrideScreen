#include "SerialManager.h"

SerialManager &serialManager = serialManager.GetInstance();

SerialManager::SerialManager()
{
    long baud = 9600;
    Serial.begin(baud, SERIAL_8N1);

    Serial2.begin(baud, SERIAL_8N1, 27, 22);
}

void SerialManager::sendMessage(const String &message)
{
    if (debugMode)
    {
        Serial.println("Debug: " + message);
    }
    Serial2.print(message);
}

void SerialManager::Debug(const String &message)
{
    if (debugMode)
    {
        Serial.println("Debug: " + message);
    }
}

void SerialManager::handleIncoming()
{
    while (Serial2.available())
    {
        char c = Serial2.read();
        if (c == '\n' || c == '\r')
        {
            if (incomingMessage.length() > 0 && messageCallback)
            {
                messageCallback(incomingMessage);
                incomingMessage = "";
            }
        }
        else
        {
            incomingMessage += c;
        }
    }
}