#ifndef MAINSCREENCOMMUNICATION_H
#define MAINSCREENCOMMUNICATION_H
#include "Communication.h"
#include "EMainScreenButtons.h"
#include "MainScreenData.h"

class MainScreenCommunication {
public:
    MainScreenCommunication(Communication &communication, MainScreenData &mainScreenData);
    void Init();
    void SendMessagePushButton(EMainScreenButtons button);
    void Leave();
    void SendToggleButtonPress(EMainScreenButtons button, bool state);

private:
    void ReceiveCallback(String& message);
    String GetValue(String data, char separator, int index);
    Communication &communication;
    MainScreenData &mainScreenData;
    int communicationId;
};

#endif //MAINSCREENCOMMUNICATION_H
