#ifndef MAINSCREENCOMMUNICATION_H
#define MAINSCREENCOMMUNICATION_H
#include "Communication.h"
#include "MainScreenData.h"

class MainScreenCommunication {
    public:
    MainScreenCommunication(Communication& communication, MainScreenData& mainScreenData);
    void Init() ;
    void SendMessage();
    void ReceiveCallback(String message);
    void Leave();

    private:
    Communication& communication;
    MainScreenData& mainScreenData;

};


#endif //MAINSCREENCOMMUNICATION_H