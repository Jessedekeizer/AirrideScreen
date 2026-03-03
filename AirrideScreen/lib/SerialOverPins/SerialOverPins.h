#ifndef SERIALOVERPINS_H
#define SERIALOVERPINS_H
#include "ISerial.h"

class SerialOverPins : public ISerial{
    void Init(int baudrate) override;
    void SendMessage(String message) override;
    bool ReceiveAvailable() override;
    String Receive() override;
};

#endif //SERIALOVERPINS_H
