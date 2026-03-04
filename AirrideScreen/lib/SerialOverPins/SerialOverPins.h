#ifndef SERIALOVERPINS_H
#define SERIALOVERPINS_H
#include <HardwareSerial.h>

#include "ISerial.h"
#include "StringQueue.h"

class SerialOverPins : public ISerial{
    public:
    SerialOverPins(HardwareSerial &serial, StringQueue &stringQueue) : serial(serial), stringQueue(stringQueue) {};
    void SendMessage(String message) override;
    bool ReceiveAvailable() override;
    void Receive() override;
    private:
    HardwareSerial &serial;
    StringQueue &stringQueue;
    String message = "";
};

#endif //SERIALOVERPINS_H
