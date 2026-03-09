#ifndef MAINCOMMUNICATION_H
#define MAINCOMMUNICATION_H
#include "Communication.h"
#include "Settings.h"
#include "WString.h"

class MainCommunication {
public:
    MainCommunication(Communication &communication, Settings &settings);

    void SendPressure(double front, double back);

    void SendMessage(String &message);

    void Init();

    void Leave();

private:
    void SaveSettings(String &settings);

    String GetValue(String data, char separator, int index);

    void ReceiveCallback(String &message);

    Communication &communication;
    Settings &settings;
    int communicationId;
};

#endif //MAINCOMMUNICATION_H
