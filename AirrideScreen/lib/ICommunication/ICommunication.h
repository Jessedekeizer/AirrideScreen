#ifndef ICOMMUNICATION_H
#define ICOMMUNICATION_H

class ICommunication {
    public:
    virtual ~ICommunication() = default;

    virtual void Init() = 0;
    virtual void SendMessage() = 0;
    virtual void ReceiveCallback() = 0;
};

#endif //ICOMMUNICATION_H