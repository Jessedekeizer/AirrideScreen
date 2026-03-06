#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "IScreen.h"
#include "MainScreenCommunication.h"
#include "MainScreenData.h"
#include "ScreenManager.h"
#include "TFTStorageHandler.h"
#include "SerialManager.h"
#include "Timer.h"

#include "MainScreenGeometryDefinitions.h"

// geometry defines for main screen buttons moved to separate header

class MainScreen : public IScreen
{
public:
    explicit MainScreen(MainScreenData &mainScreenData, MainScreenCommunication &mainScreenCommunication, ScreenManager &screenManager, SettingsDevice &settings);
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
