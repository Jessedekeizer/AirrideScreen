#ifndef SETTINGSSCREENCOMMUNICATION_H
#define SETTINGSSCREENCOMMUNICATION_H
#include <WString.h>

#include "../../../../SharedLib/Communication/Communication/Communication.h"
#include "SettingsDevice.h"

class SettingsScreenCommunication {
public:
    SettingsScreenCommunication(Communication &communication, SettingsDevice &settings) : communication(communication) {
    };

    ~SettingsScreenCommunication() {
    };
    void SendSettings(SettingsDevice &settings);

private:
    Communication &communication;
};

#endif //SETTINGSSCREENCOMMUNICATION_H
