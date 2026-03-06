#include "SettingsScreenCommunication.h"

#include "SerialManager.h"

void SettingsScreenCommunication::SendSettings(SettingsDevice &settings) {
    serialManager.Debug("try settings send");
    communication.SendMessage("settings/" + String(settings.frontMax) +
                              "/" + settings.backMax +
                              "/" + settings.rideFront +
                              "/" + settings.rideBack +
                              "/" + settings.frontUpX +
                              "/" + settings.frontDownX +
                              "/" + settings.backUpX +
                              "/" + settings.backDownX +
                              "/" + settings.parkDuration + "/");
    serialManager.Debug("settings send");
}
