#ifndef SETTINGS2SCREEN_H
#define SETTINGS2SCREEN_H

#include "IScreen.h"
#include "ScreenManager.h"
#include "SettingsScreenCommunication.h"
#include "TFTStorageHandler.h"

class Settings2Screen : public IScreen
{
public:
    Settings2Screen(ScreenManager& screenManager, SettingsScreenCommunication& settingsScreenCommunication, SettingsDevice& settingsDevice);
    void OnLoop() override {}
    void OnSetup() override;

private:
    void HandleMainScreen();
    void HandleSave();
    void HandleSettings1();
    void HandleSettings3();
    void HandleFrontUpAdd();
    void HandleFrontUpSub();
    void HandleFrontDownAdd();
    void HandleFrontDownSub();
    void HandleBackUpAdd();
    void HandleBackUpSub();
    void HandleBackDownAdd();
    void HandleBackDownSub();
    ScreenManager& screenManager;
    SettingsScreenCommunication& settingsScreenCommunication;
    SettingsDevice& settings;
};

#endif