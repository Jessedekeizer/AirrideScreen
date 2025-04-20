#include "Settings4Screen.h"

Settings4Screen::Settings4Screen()
{
    name = "Settings4";
    path = "/Settings4.png";
    buttons = std::vector<Button *>();
    buttons.push_back(new PushButton(10, 10, 50, 50, "MainScreen",
                                     [this](Button &button)
                                     { HandleMainScreen(); }));

    buttons.push_back(new PushButton(260, 10, 50, 50, "save",
                                     [this](Button &button)
                                     { HandleSave(); }));

    buttons.push_back(new PushButton(126, 73, 35, 35, "ParkDurS",
                                     [this](Button &button)
                                     { HandleParkDurationSub(); }));

    buttons.push_back(new PushButton(218, 73, 35, 35, "ParkDurA",
                                     [this](Button &button)
                                     { HandleParkDurationAdd(); }));

    buttons.push_back(new PushButton(126, 112, 35, 35, "LoggingOn",
                                     [this](Button &button)
                                     { HandleLoggingOn(); }));

    buttons.push_back(new PushButton(218, 112, 35, 35, "LoggingOff",
                                     [this](Button &button)
                                     { HandleLoggingOff(); }));

    buttons.push_back(new PushButton(126, 159, 35, 35, "MachineLearningOn",
                                     [this](Button &button)
                                     { HandleMachineLearningOn(); }));

    buttons.push_back(new PushButton(218, 159, 35, 35, "MachineLearningOff",
                                     [this](Button &button)
                                     { HandleMachineLearningOff(); }));

    buttons.push_back(new PushButton(126, 198, 35, 35, "BluetoothOn",
                                     [this](Button &button)
                                     { HandleBluetoothOn(); }));

    buttons.push_back(new PushButton(218, 198, 35, 35, "BluetoothOff",
                                     [this](Button &button)
                                     { HandleBluetoothOff(); }));

    buttons.push_back(new PushButton(10, 198, 30, 30, "Settings3",
                                     [this](Button &button)
                                     { HandleSettings3(); }));
}

void Settings4Screen::OnSetup()
{
    auto &settings = storageHandler.getSettings();
    storageHandler.DrawString(String(settings.parkDuration, 1), 180, 82);
    storageHandler.PrintSettingBool(settings.logging, 172, 112);
    storageHandler.PrintSettingBool(settings.machineLearning, 172, 159);
    storageHandler.PrintSettingBool(settings.bluetooth, 172, 198);
}

void Settings4Screen::HandleMainScreen()
{
    screenManager.ChangeScreen("MainScreen");
}

void Settings4Screen::HandleSave()
{
    storageHandler.WriteSettings();
    storageHandler.SendSettings();
    storageHandler.ReadSettings();
    screenManager.ChangeScreen("MainScreen");
}

void Settings4Screen::HandleParkDurationAdd()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.parkDuration, 0.1);
    storageHandler.DrawString(String(settings.parkDuration, 1), 180, 82);
}

void Settings4Screen::HandleParkDurationSub()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.parkDuration, -0.1);
    storageHandler.DrawString(String(settings.parkDuration, 1), 180, 82);
}

void Settings4Screen::HandleLoggingOn()
{
    auto &settings = storageHandler.getSettings();
    settings.logging = true;
    storageHandler.PrintSettingBool(settings.logging, 172, 112);
}

void Settings4Screen::HandleLoggingOff()
{
    auto &settings = storageHandler.getSettings();
    settings.logging = false;
    storageHandler.PrintSettingBool(settings.logging, 172, 112);
}

void Settings4Screen::HandleMachineLearningOn()
{
    auto &settings = storageHandler.getSettings();
    settings.machineLearning = true;
    storageHandler.PrintSettingBool(settings.machineLearning, 172, 159);
}

void Settings4Screen::HandleMachineLearningOff()
{
    auto &settings = storageHandler.getSettings();
    settings.machineLearning = false;
    storageHandler.PrintSettingBool(settings.machineLearning, 172, 159);
}

void Settings4Screen::HandleBluetoothOn()
{
    auto &settings = storageHandler.getSettings();
    settings.bluetooth = true;
    storageHandler.PrintSettingBool(settings.bluetooth, 172, 198);
}

void Settings4Screen::HandleBluetoothOff()
{
    auto &settings = storageHandler.getSettings();
    settings.bluetooth = false;
    storageHandler.PrintSettingBool(settings.bluetooth, 172, 198);
}

void Settings4Screen::HandleSettings3()
{
    screenManager.ChangeScreen("Settings3");
}
