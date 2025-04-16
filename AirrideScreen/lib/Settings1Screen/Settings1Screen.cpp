#include "Settings1Screen.h"

Settings1Screen::Settings1Screen()
{
    name = "Settings1";
    path = "/Settings1.png";
    buttons = std::vector<Button *>();

    buttons.push_back(new PushButton(20, 20, 50, 50, "MainScreen",
                                     [this](Button &button)
                                     { HandleMainScreen(); }));

    buttons.push_back(new PushButton(245, 20, 50, 50, "save",
                                     [this](Button &button)
                                     { HandleSave(); }));

    buttons.push_back(new PushButton(120, 80, 30, 30, "RideFD",
                                     [this](Button &button)
                                     { HandleRideFD(); }));

    buttons.push_back(new PushButton(205, 80, 30, 30, "RideFU",
                                     [this](Button &button)
                                     { HandleRideFU(); }));

    buttons.push_back(new PushButton(120, 113, 30, 30, "RideBD",
                                     [this](Button &button)
                                     { HandleRideBD(); }));

    buttons.push_back(new PushButton(205, 113, 30, 30, "RideBU",
                                     [this](Button &button)
                                     { HandleRideBU(); }));
    buttons.push_back(new PushButton(120, 158, 30, 30, "MaxFD",
                                     [this](Button &button)
                                     { HandleMaxFD(); }));

    buttons.push_back(new PushButton(205, 158, 30, 30, "MaxFU",
                                     [this](Button &button)
                                     { HandleMaxFU(); }));

    buttons.push_back(new PushButton(120, 190, 30, 30, "MaxBD",
                                     [this](Button &button)
                                     { HandleMaxBD(); }));

    buttons.push_back(new PushButton(205, 190, 30, 30, "MaxBU",
                                     [this](Button &button)
                                     { HandleMaxBU(); }));

    buttons.push_back(new PushButton(260, 190, 30, 30, "Settings2",
                                     [this](Button &button)
                                     { HandleSettings2(); }));
}

void Settings1Screen::OnLoop()
{
    auto &settings = storageHandler.getSettings();
    storageHandler.DrawString(String(settings.rideFront, 1), 180, 82);
    storageHandler.DrawString(String(settings.rideBack, 1), 180, 122);
    storageHandler.DrawString(String(settings.frontMax, 1), 180, 167);
    storageHandler.DrawString(String(settings.backMax, 1), 180, 207);
}

void Settings1Screen::HandleMainScreen()
{
    screenManager.ChangeScreen("MainScreen");
}

void Settings1Screen::HandleSave()
{
    storageHandler.WriteSettings();
    storageHandler.sendSettings();
    storageHandler.ReadAirSuspensionData();
    screenManager.ChangeScreen("MainScreen");
}

void Settings1Screen::HandleRideFU()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.rideFront, 0.1);
}

void Settings1Screen::HandleRideFD()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.rideFront, -0.1);
}

void Settings1Screen::HandleRideBU()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.rideBack, 0.1);
}

void Settings1Screen::HandleRideBD()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.rideBack, -0.1);
}

void Settings1Screen::HandleMaxFU()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.frontMax, 0.1);
}

void Settings1Screen::HandleMaxFD()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.frontMax, -0.1);
}

void Settings1Screen::HandleMaxBU()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.backMax, 0.1);
}

void Settings1Screen::HandleMaxBD()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.backMax, -0.1);
}

void Settings1Screen::HandleSettings2()
{
    screenManager.ChangeScreen("Settings2");
}