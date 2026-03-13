#include "SettingsScreenCommunication.h"

#include "Logger.h"

void SettingsScreenCommunication::SendSettings(SettingsDevice &settings)
{
    LOG_DEBUG("try settings send");
    communication.SendMessage("settings/" + String(settings.frontMax) +
                              "/" + settings.backMax +
                              "/" + settings.rideFront +
                              "/" + settings.rideBack +
                              "/" + settings.frontUpX +
                              "/" + settings.frontDownX +
                              "/" + settings.backUpX +
                              "/" + settings.backDownX +
                              "/" + settings.parkDuration + "/");
    LOG_DEBUG("settings send");
}
