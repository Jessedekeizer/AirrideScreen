#ifndef SETTINGS3SCREEN_H
#define SETTINGS3SCREEN_H

#include "IScreen.h"
#include "ScreenManager.h"
#include "SettingsScreenCommunication.h"
#include "TFTStorageHandler.h"


class Settings3Screen : public IScreen
{
public:
    Settings3Screen(ScreenManager& screenManager, SettingsScreenCommunication& settingsScreenCommunication, SettingsDevice& settingsDevice);
    void OnLoop() override {}
    void OnSetup() override;

private:
    void HandleMainScreen();
    void HandleSave();
    void HandleSettings1();
    void HandleSettings3();
    void HandleAutoRideSecAdd();
    void HandleAutoRideSecSub();
    void HandleAutoRideOn();
    void HandleAutoRideOff();
    void HandleAutoParkSecAdd();
    void HandleAutoParkSecSub();
    void HandleAutoParkOn();
    void HandleAutoParkOff();
    ScreenManager& screenManager;
    SettingsScreenCommunication& settingsScreenCommunication;
    SettingsDevice& settings;
};

#endif