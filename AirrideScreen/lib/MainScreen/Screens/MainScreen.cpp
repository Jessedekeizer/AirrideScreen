#include <MainScreen.h>

#include "MainScreenCommunication.h"
#include "ScreenManager.h"
#include "TimerManager.h"

MainScreen::MainScreen(MainScreenData &mainScreenData, MainScreenCommunication &mainScreenCommunication, ScreenManager &screenManager, SettingsDevice &settings)
                        : mainScreenData(mainScreenData), mainScreenCommunication(mainScreenCommunication), screenManager(screenManager), settings(settings)
{
    name = "MainScreen";
    path = "/MainScreen.png";

    buttons = std::vector<Button *>();

    // Front buttons
    buttons.push_back(new ToggleButton(0, 0, 70, 120, "Front Up",
                                       [this](Button &button)
                                       { HandleFrontUp(button); }));
    buttons.push_back(new ToggleButton(0, 120, 70, 120, "Front Down",
                                       [this](Button &button)
                                       { HandleFrontDown(button); }));

    // Back buttons
    buttons.push_back(new ToggleButton(263, 0, 70, 120, "Back Up",
                                       [this](Button &button)
                                       { HandleBackUp(button); }));
    buttons.push_back(new ToggleButton(263, 120, 70, 120, "Back Down",
                                       [this](Button &button)
                                       { HandleBackDown(button); }));

    // Control buttons
    buttons.push_back(new PushButton(132, 174, 48, 42, "Settings1",
                                     [this](Button &button)
                                     { GoToSettings1(); }));
    buttons.push_back(new PushButton(84, 174, 48, 42, "Ride",
                                     [this](Button &button)
                                     { SendRideCommand(); }));
    buttons.push_back(new PushButton(186, 174, 48, 42, "Park",
                                     [this](Button &button)
                                     { SendParkCommand(); }));
};

void MainScreen::OnSetup()
{
    mainScreenCommunication.Init();
    AddRideTimer();
}

void MainScreen::AddRideTimer()
{
    if (!settings.autoRide)
    {
        serialManager.Debug("MainScreen::addRideTimer - Auto start ride is disabled");
        return;
    }
    if (autoRideTimer != nullptr)
    {
        serialManager.Debug("MainScreen::addRideTimer - Timer already exists, not adding again");
        return;
    }
    autoRideTimer = new Timer(settings.autoRideSec, [this]()
                              { AutoStartRide(); });
    timerManager.addTimer(autoRideTimer);
    serialManager.Debug("MainScreen::addRideTimer - Timer added");
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
        serialManager.Debug("MainScreen::Timer - Sending ride command");
        mainScreenCommunication.SendMessage("Ride");
    }
    else
    {
        serialManager.Debug("MainScreen::Timer - Not sending ride command");
    }
    abortAutoRide = true;
}

void MainScreen::GoToSettings1()
{
    mainScreenCommunication.Leave();
    serialManager.clearMessageCallback();
    screenManager.ChangeScreen("Settings1");
}

void MainScreen::SendRideCommand()
{
    abortAutoRide = true;
    mainScreenCommunication.SendMessage("Ride");
}

void MainScreen::SendParkCommand()
{
    abortAutoRide = true;
    mainScreenCommunication.SendMessage("Park");
}

void MainScreen::HandleFrontUp(Button &sender)
{
    abortAutoRide = true;
    auto &toggle = static_cast<ToggleButton &>(sender);
    if (toggle.GetState())
    {
        mainScreenCommunication.SendMessage("Front Up On");
    }
    else
    {
        mainScreenCommunication.SendMessage("Front Up Off");
    }
}

void MainScreen::HandleFrontDown(Button &sender)
{
    abortAutoRide = true;
    auto &toggle = static_cast<ToggleButton &>(sender);
    if (toggle.GetState())
    {
        mainScreenCommunication.SendMessage("Front Down On");
    }
    else
    {
        mainScreenCommunication.SendMessage("Front Down Off");
    }
}

void MainScreen::HandleBackUp(Button &sender)
{
    abortAutoRide = true;
    auto &toggle = static_cast<ToggleButton &>(sender);
    if (toggle.GetState())
    {
        mainScreenCommunication.SendMessage("Back Up On");
    }
    else
    {
        mainScreenCommunication.SendMessage("Back Up Off");
    }
}

void MainScreen::HandleBackDown(Button &sender)
{
    abortAutoRide = true;
    auto &toggle = static_cast<ToggleButton &>(sender);
    if (toggle.GetState())
    {
        mainScreenCommunication.SendMessage("Back Down On");
    }
    else
    {
        mainScreenCommunication.SendMessage("Back Down Off");
    }
}