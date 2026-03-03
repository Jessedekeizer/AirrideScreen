#include "../include/SettingsScreenCommunication.h"

void SettingsScreenCommunication::SendSettings() {
    communication.SendMessage("settings/" + String(settings.frontMax) +
                              "/" + settings.backMax +
                              "/" + settings.rideFront +
                              "/" + settings.rideBack +
                              "/" + settings.frontUpX +
                              "/" + settings.frontDownX +
                              "/" + settings.backUpX +
                              "/" + settings.backDownX +
                              "/" + settings.parkDuration + "/");
}
