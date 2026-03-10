#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "BaseScreen.h"
#include "MainScreenCommunication.h"
#include "MainScreenData.h"
#include "ScreenManager.h"
#include "TFTStorageHandler.h"
#include "Timer.h"

#include "MainScreenGeometry.h"

class MainScreen : public BaseScreen
{
public:
    MainScreen(MainScreenData &mainScreenData, MainScreenCommunication &mainScreenCommunication, ScreenManager &screenManager, SettingsDevice &settings);
    void OnLoop() override;
    void OnSetup() override;

private:
    void GoToSettings1();
    void HandlePushButton(Button &sender);
    void AutoStartRide();
    void HandleToggleButton(Button &sender);
    void AddRideTimer();
    MainScreenData &mainScreenData;
    MainScreenCommunication &mainScreenCommunication;
    ScreenManager &screenManager;
    SettingsDevice &settings;
    bool abortAutoRide = false;
    Timer *autoRideTimer = nullptr;
};

#endif
