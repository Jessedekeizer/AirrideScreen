#ifndef MAINSCREENCOMMUNICATION_H
#define MAINSCREENCOMMUNICATION_H
#include "Communication.h"
#include "MainScreenData.h"

class MainScreenCommunication {
    public:
    MainScreenCommunication(Communication& communication, MainScreenData& mainScreenData);
    void Init() ;
    void SendMessage(String message);
    void Leave();

    private:
    void ReceiveCallback(String message);
    String GetValue(String data, char separator, int index);
    Communication& communication;
    MainScreenData& mainScreenData;
    int communicationId;

};


#endif //MAINSCREENCOMMUNICATION_H