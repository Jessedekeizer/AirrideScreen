
#include <MainScreen.h>
#include "MainScreenGeometry.h"
#include "MainScreenCommunication.h"
#include "ScreenManager.h"
#include "TimerManager.h"
#include "Logger.h"

MainScreen::MainScreen(MainScreenData &mainScreenData,
                       MainScreenCommunication &mainScreenCommunication,
                       ScreenManager &screenManager,
                       SettingsDevice &settings)
    : mainScreenData(mainScreenData),
      mainScreenCommunication(mainScreenCommunication),
      screenManager(screenManager),
      settings(settings),
      BaseScreen(EScreen::MAIN, "/MainScreen.png")
{
    buttons.push_back(new ToggleButton(FRONT_UP_X, FRONT_UP_Y, FRONT_UP_W, FRONT_UP_H, FRONT_UP,
                                       [this](Button &button)
                                       { HandleToggleButton(button); }));
    buttons.push_back(new ToggleButton(FRONT_DOWN_X, FRONT_DOWN_Y, FRONT_DOWN_W, FRONT_DOWN_H, FRONT_DOWN,
                                       [this](Button &button)
                                       { HandleToggleButton(button); }));

    buttons.push_back(new ToggleButton(BACK_UP_X, BACK_UP_Y, BACK_UP_W, BACK_UP_H, BACK_UP,
                                       [this](Button &button)
                                       { HandleToggleButton(button); }));
    buttons.push_back(new ToggleButton(BACK_DOWN_X, BACK_DOWN_Y, BACK_DOWN_W, BACK_DOWN_H, BACK_DOWN,
                                       [this](Button &button)
                                       { HandleToggleButton(button); }));

    buttons.push_back(new PushButton(SETTINGS1_X, SETTINGS1_Y, SETTINGS1_W, SETTINGS1_H, SETTINGS1,
                                     [this](Button &button)
                                     { GoToSettings1(); }));
    buttons.push_back(new PushButton(RIDE_X, RIDE_Y, RIDE_W, RIDE_H, RIDE,
                                     [this](Button &button)
                                     { HandlePushButton(button); }));
    buttons.push_back(new PushButton(PARK_X, PARK_Y, PARK_W, PARK_H, PARK,
                                     [this](Button &button)
                                     { HandlePushButton(button); }));
};

void MainScreen::OnSetup()
{
    mainScreenCommunication.Init();
    AddRideTimer();
}

MainScreen::~MainScreen()
{
    timerManager.removeTimer(autoRideTimer);
    delete autoRideTimer;
}

void MainScreen::AddRideTimer()
{
    if (!settings.autoRide)
    {
        LOG_DEBUG("MainScreen::addRideTimer - Auto start ride is disabled");
        return;
    }
    if (autoRideTimer != nullptr)
    {
        LOG_DEBUG("MainScreen::addRideTimer - Timer already exists, not adding again");
        return;
    }
    autoRideTimer = new Timer(settings.autoRideSec, [this]()
                              { AutoStartRide(); });
    timerManager.addTimer(autoRideTimer);
    LOG_DEBUG("MainScreen::addRideTimer - Timer added");
}

void MainScreen::OnLoop()
{
    storageHandler.PrintPressure(mainScreenData.front, mainScreenData.back);
}

void MainScreen::AutoStartRide()
{
    if (abortAutoRide)
    {
        return;
    }
    if (mainScreenData.front < 1.5 || mainScreenData.back < 1.5)
    {
        LOG_DEBUG("MainScreen::Timer - Sending ride command");
        mainScreenCommunication.SendMessagePushButton(RIDE);
    }
    else
    {
        LOG_DEBUG("MainScreen::Timer - Not sending ride command");
    }
    abortAutoRide = true;
}

void MainScreen::GoToSettings1()
{
    mainScreenCommunication.Leave();
    screenManager.RequestScreen(EScreen::SETTINGS1);
}

void MainScreen::HandlePushButton(Button &sender)
{
    abortAutoRide = true;
    mainScreenCommunication.SendMessagePushButton(static_cast<EMainScreenButtons>(sender.GetName()));
}

void MainScreen::HandleToggleButton(Button &sender)
{
    abortAutoRide = true;
    auto &toggle = static_cast<ToggleButton &>(sender);
    mainScreenCommunication.SendToggleButtonPress(static_cast<EMainScreenButtons>(toggle.GetName()), toggle.GetState());
}
