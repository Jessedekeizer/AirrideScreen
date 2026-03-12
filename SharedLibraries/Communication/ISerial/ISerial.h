#ifndef ISERIAL_H
#define ISERIAL_H
#include <WString.h>


class ISerial {
    public:
    virtual ~ISerial() = default;

    virtual void SendMessage(String& message) = 0;
    virtual bool ReceiveAvailable() = 0;
    virtual void Receive() = 0;
};

#endif //ISERIAL_H