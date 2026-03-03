#ifndef SETTINGSSCREENCOMMUNICATION_H
#define SETTINGSSCREENCOMMUNICATION_H
#include <WString.h>

#include "Communication.h"
#include "SettingsDevice.h"

class SettingsScreenCommunication {
    public:
    SettingsScreenCommunication(Communication& communication, SettingsDevice& settings) : communication(communication), settings(settings) {};
    ~SettingsScreenCommunication(){};
    void SendSettings();

private:
    Communication& communication;
    SettingsDevice& settings;
};

#endif //SETTINGSSCREENCOMMUNICATION_H