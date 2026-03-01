#ifndef MAINSCREENCOMMUNICATION_H
#define MAINSCREENCOMMUNICATION_H
#include "ICommunication.h"

class MainScreenCommunication : public ICommunication {
    public:
    MainScreenCommunication();
    void Init() override;
    void SendMessage() override;
    void ReceiveCallback() override;

};


#endif //MAINSCREENCOMMUNICATION_H