#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "IScreen.h"
#include "MainScreenCommunication.h"
#include "MainScreenData.h"
#include "ScreenManager.h"
#include "TFTStorageHandler.h"
#include "SerialManager.h"
#include "Timer.h"

class MainScreen : public IScreen
{
public:
    explicit MainScreen(MainScreenData &mainScreenData, MainScreenCommunication &mainScreenCommunication, ScreenManager &screenManager, SettingsDevice &settings);
    void OnLoop() override;
    void OnSetup() override;

private:
    void GoToSettings1();
    void SendRideCommand();
    void SendParkCommand();
    void AutoStartRide();
    void HandleFrontUp(Button &sender);
    void HandleFrontDown(Button &sender);
    void HandleBackUp(Button &sender);
    void HandleBackDown(Button &sender);
    void AddRideTimer();
    MainScreenData &mainScreenData;
    MainScreenCommunication &mainScreenCommunication;
    ScreenManager &screenManager;
    SettingsDevice &settings;
    bool abortAutoRide = false;
    Timer *autoRideTimer = nullptr;
};

#endif