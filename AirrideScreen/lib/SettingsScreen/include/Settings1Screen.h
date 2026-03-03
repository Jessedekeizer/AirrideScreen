#ifndef SETTINGS1SCREEN_H
#define SETTINGS1SCREEN_H

#include "IScreen.h"
#include "TFTStorageHandler.h"
#include "ScreenManager.h"
#include "SettingsScreenCommunication.h"

class Settings1Screen : public IScreen
{
public:
    Settings1Screen(ScreenManager& screenManager, SettingsScreenCommunication& settingsScreenCommunication, SettingsDevice& settingsDevice);
    void OnLoop() override {};
    void OnSetup() override;

private:
    void HandleMainScreen();
    void HandleSave();
    void HandleRideFU();
    void HandleRideFD();
    void HandleRideBU();
    void HandleRideBD();
    void HandleMaxFU();
    void HandleMaxFD();
    void HandleMaxBU();
    void HandleMaxBD();
    void HandleSettings2();
    ScreenManager& screenManager;
    SettingsScreenCommunication& settingsScreenCommunication;
    SettingsDevice& settings;
};

#endif