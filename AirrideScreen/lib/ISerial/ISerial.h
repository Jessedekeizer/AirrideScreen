#ifndef ICOMMUNICATION_H
#define ICOMMUNICATION_H
#include <WString.h>


class ISerial {
    public:
    virtual ~ISerial() = default;

    virtual void Init(int baudrate) = 0;
    virtual void SendMessage(String message) = 0;
    virtual bool ReceiveAvailable() = 0;
    virtual String Receive() = 0;
};

#endif //ICOMMUNICATION_H