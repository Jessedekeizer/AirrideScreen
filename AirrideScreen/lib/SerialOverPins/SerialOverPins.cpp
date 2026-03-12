#include "SerialOverPins.h"
#include <HardwareSerial.h>

#include "Logger.h"

void SerialOverPins::SendMessage(String &message)
{
    serial.println(message);
}

bool SerialOverPins::ReceiveAvailable()
{
    return serial.available();
}

void SerialOverPins::Receive()
{
    while (serial.available())
    {
        char c = serial.read();
        if (c == '\n')
        {
            if (!stringQueue.enqueue(message))
            {
                LOG_DEBUG("SerialOverPins::Receive: queue full");
            }
            message = "";
        }
        else if (c != '\r')
        {
            message += c;
        }
    }
}
