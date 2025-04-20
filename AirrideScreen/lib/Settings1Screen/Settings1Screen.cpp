#include "Settings1Screen.h"

Settings1Screen::Settings1Screen()
{
    name = "Settings1";
    path = "/Settings1.png";
    buttons = std::vector<Button *>();

    buttons.push_back(new PushButton(10, 10, 50, 50, "MainScreen",
                                     [this](Button &button)
                                     { HandleMainScreen(); }));

    buttons.push_back(new PushButton(260, 10, 50, 50, "save",
                                     [this](Button &button)
                                     { HandleSave(); }));

    buttons.push_back(new PushButton(126, 73, 35, 35, "RideFD",
                                     [this](Button &button)
                                     { HandleRideFD(); }));

    buttons.push_back(new PushButton(218, 73, 35, 35, "RideFU",
                                     [this](Button &button)
                                     { HandleRideFU(); }));

    buttons.push_back(new PushButton(126, 112, 35, 35, "RideBD",
                                     [this](Button &button)
                                     { HandleRideBD(); }));

    buttons.push_back(new PushButton(218, 112, 35, 35, "RideBU",
                                     [this](Button &button)
                                     { HandleRideBU(); }));
    buttons.push_back(new PushButton(126, 159, 35, 35, "MaxFD",
                                     [this](Button &button)
                                     { HandleMaxFD(); }));

    buttons.push_back(new PushButton(218, 159, 35, 35, "MaxFU",
                                     [this](Button &button)
                                     { HandleMaxFU(); }));

    buttons.push_back(new PushButton(126, 198, 35, 35, "MaxBD",
                                     [this](Button &button)
                                     { HandleMaxBD(); }));

    buttons.push_back(new PushButton(218, 198, 35, 35, "MaxBU",
                                     [this](Button &button)
                                     { HandleMaxBU(); }));

    buttons.push_back(new PushButton(275, 198, 30, 30, "Settings2",
                                     [this](Button &button)
                                     { HandleSettings2(); }));
}

void Settings1Screen::OnSetup()
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
    storageHandler.SendSettings();
    storageHandler.ReadSettings();
    screenManager.ChangeScreen("MainScreen");
}

void Settings1Screen::HandleRideFU()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.rideFront, 0.1);
    storageHandler.DrawString(String(settings.rideFront, 1), 180, 82);
}

void Settings1Screen::HandleRideFD()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.rideFront, -0.1);
    storageHandler.DrawString(String(settings.rideFront, 1), 180, 82);
}

void Settings1Screen::HandleRideBU()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.rideBack, 0.1);
    storageHandler.DrawString(String(settings.rideBack, 1), 180, 122);
}

void Settings1Screen::HandleRideBD()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.rideBack, -0.1);
    storageHandler.DrawString(String(settings.rideBack, 1), 180, 122);
}

void Settings1Screen::HandleMaxFU()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.frontMax, 0.1);
    storageHandler.DrawString(String(settings.frontMax, 1), 180, 167);
}

void Settings1Screen::HandleMaxFD()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.frontMax, -0.1);
    storageHandler.DrawString(String(settings.frontMax, 1), 180, 167);
}

void Settings1Screen::HandleMaxBU()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.backMax, 0.1);
    storageHandler.DrawString(String(settings.backMax, 1), 180, 207);
}

void Settings1Screen::HandleMaxBD()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.backMax, -0.1);
    storageHandler.DrawString(String(settings.backMax, 1), 180, 207);
}

void Settings1Screen::HandleSettings2()
{
    screenManager.ChangeScreen("Settings2");
}