#include "../include/Settings3Screen.h"

Settings3Screen::Settings3Screen(ScreenManager& screenManager, SettingsScreenCommunication& , SettingsDevice& settingsDevice)
    : screenManager(screenManager),settingsScreenCommunication(settingsScreenCommunication), settings(settingsDevice)
{
    name = "Settings3";
    path = "/Settings3.png";
    buttons = std::vector<Button *>();
    buttons.push_back(new PushButton(10, 10, 50, 50, "MainScreen",
                                     [this](Button &button)
                                     { HandleMainScreen(); }));

    buttons.push_back(new PushButton(260, 10, 50, 50, "save",
                                     [this](Button &button)
                                     { HandleSave(); }));

    buttons.push_back(new PushButton(126, 73, 35, 35, "AutoRideSecS",
                                     [this](Button &button)
                                     { HandleAutoRideSecSub(); }));

    buttons.push_back(new PushButton(218, 73, 35, 35, "AutoRideSecA",
                                     [this](Button &button)
                                     { HandleAutoRideSecAdd(); }));

    buttons.push_back(new PushButton(126, 112, 35, 35, "AutoRideOn",
                                     [this](Button &button)
                                     { HandleAutoRideOn(); }));

    buttons.push_back(new PushButton(218, 112, 35, 35, "AutoRideOff",
                                     [this](Button &button)
                                     { HandleAutoRideOff(); }));

    buttons.push_back(new PushButton(126, 159, 35, 35, "AutoParkSecS",
                                     [this](Button &button)
                                     { HandleAutoParkSecSub(); }));

    buttons.push_back(new PushButton(218, 159, 35, 35, "AutoParkSecA",
                                     [this](Button &button)
                                     { HandleAutoParkSecAdd(); }));

    buttons.push_back(new PushButton(126, 198, 35, 35, "AutoParkOn",
                                     [this](Button &button)
                                     { HandleAutoParkOn(); }));

    buttons.push_back(new PushButton(218, 198, 35, 35, "AutoParkOff",
                                     [this](Button &button)
                                     { HandleAutoParkOff(); }));

    buttons.push_back(new PushButton(10, 198, 30, 30, "Settings2",
                                     [this](Button &button)
                                     { HandleSettings1(); }));

    buttons.push_back(new PushButton(275, 198, 30, 30, "Settings4",
                                     [this](Button &button)
                                     { HandleSettings3(); }));
}

void Settings3Screen::OnSetup()
{
    storageHandler.DrawString(String(settings.autoRideSec, 1), 180, 82);
    storageHandler.PrintSettingBool(settings.autoRide, 172, 112);
    storageHandler.DrawString(String(settings.autoParkSec, 1), 180, 167);
    storageHandler.PrintSettingBool(settings.autoPark, 172, 198);
}
void Settings3Screen::HandleMainScreen()
{
    screenManager.ChangeScreen("MainScreen");
}

void Settings3Screen::HandleSave()
{
    storageHandler.WriteSettings(settings);
    settingsScreenCommunication.SendSettings();
    storageHandler.ReadSettings(settings);
    screenManager.ChangeScreen("MainScreen");
}

void Settings3Screen::HandleAutoRideSecAdd()
{
    settings.adjustValue(settings.autoRideSec, 0.1);
    storageHandler.DrawString(String(settings.autoRideSec, 1), 180, 82);
}

void Settings3Screen::HandleAutoRideSecSub()
{
    settings.adjustValue(settings.autoRideSec, -0.1);
    storageHandler.DrawString(String(settings.autoRideSec, 1), 180, 82);
}

void Settings3Screen::HandleAutoRideOn()
{
    settings.autoRide = true;
    storageHandler.PrintSettingBool(settings.autoRide, 172, 112);
}

void Settings3Screen::HandleAutoRideOff()
{
    settings.autoRide = false;
    storageHandler.PrintSettingBool(settings.autoRide, 172, 112);
}

void Settings3Screen::HandleAutoParkSecAdd()
{
    settings.adjustValue(settings.autoParkSec, 0.1);
    storageHandler.DrawString(String(settings.autoParkSec, 1), 180, 167);
}

void Settings3Screen::HandleAutoParkSecSub()
{
    settings.adjustValue(settings.autoParkSec, -0.1);
    storageHandler.DrawString(String(settings.autoParkSec, 1), 180, 167);
}

void Settings3Screen::HandleAutoParkOn()
{
    settings.autoPark = true;
    storageHandler.PrintSettingBool(settings.autoPark, 172, 198);
}

void Settings3Screen::HandleAutoParkOff()
{
    settings.autoPark = false;
    storageHandler.PrintSettingBool(settings.autoPark, 172, 198);
}

void Settings3Screen::HandleSettings1()
{
    screenManager.ChangeScreen("Settings2");
}

void Settings3Screen::HandleSettings3()
{
    screenManager.ChangeScreen("Settings4");
}