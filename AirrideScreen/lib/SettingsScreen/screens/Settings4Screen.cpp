#include "Settings4Screen.h"
#include "ESettingsScreenButtons.h"

Settings4Screen::Settings4Screen(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication, SettingsDevice &settingsDevice)
    : SettingsScreenBase(screenManager, settingsScreenCommunication, settingsDevice)
{
    name = EScreen::SETTINGS4;
    path = "/Settings4.png";
    buttons = std::vector<Button *>();
    buttons.push_back(new PushButton(SETTINGSBTN_MAIN_X, SETTINGSBTN_MAIN_Y, SETTINGSBTN_MAIN_W, SETTINGSBTN_MAIN_H, MAIN_SCREEN,
                                     [this](Button &button)
                                     { GoToMainScreen(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_SAVE_X, SETTINGSBTN_SAVE_Y, SETTINGSBTN_SAVE_W, SETTINGSBTN_SAVE_H, SAVE,
                                     [this](Button &button)
                                     { SaveSettings(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW1_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, PARK_DURATION_SECONDS_SUBTRACT,
                                     [this](Button &button)
                                     { HandleParkDurationSub(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW1_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, PARK_DURATION_SECONDS_SUBTRACT,
                                     [this](Button &button)
                                     { HandleParkDurationAdd(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW2_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, LOGGING_ON,
                                     [this](Button &button)
                                     { HandleLoggingOn(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW2_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, LOGGING_OFF,
                                     [this](Button &button)
                                     { HandleLoggingOff(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW3_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, MACHINE_LEARNING_ON,
                                     [this](Button &button)
                                     { HandleMachineLearningOn(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW3_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, MACHINE_LEARNING_OFF,
                                     [this](Button &button)
                                     { HandleMachineLearningOff(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW4_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, BLUETOOTH_ON,
                                     [this](Button &button)
                                     { HandleBluetoothOn(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW4_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, BLUETOOTH_OFF,
                                     [this](Button &button)
                                     { HandleBluetoothOff(); }));

    // calibration button doesn't follow grid, use defined X while sharing size
    buttons.push_back(new PushButton(SETTINGSBTN_CALIB_X, 112, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, CALIBRATION,
                                     [this](Button &button)
                                     { HandleCalibration(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_NAV_LEFT_X, SETTINGSBTN_NAV_BOTTOM_Y, SETTINGSBTN_NAV_SIZE, SETTINGSBTN_NAV_SIZE, SETTINGS3,
                                     [this](Button &button)
                                     { HandleSettings3(); }));
}

void Settings4Screen::OnSetup()
{
    storageHandler.DrawString(String(settings.parkDuration, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
    storageHandler.PrintSettingBool(settings.logging, SETTINGS_BOOL_X, SETTINGS_TEXT_ROW2_Y);
    storageHandler.PrintSettingBool(settings.machineLearning, SETTINGS_BOOL_X, SETTINGS_TEXT_ROW3_Y);
    storageHandler.PrintSettingBool(settings.bluetooth, SETTINGS_BOOL_X, SETTINGS_TEXT_ROW4_Y);
}

void Settings4Screen::HandleParkDurationAdd()
{
    settings.adjustValue(settings.parkDuration, 0.1);
    storageHandler.DrawString(String(settings.parkDuration, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
}

void Settings4Screen::HandleParkDurationSub()
{
    settings.adjustValue(settings.parkDuration, -0.1);
    storageHandler.DrawString(String(settings.parkDuration, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
}

void Settings4Screen::HandleLoggingOn()
{
    settings.logging = true;
    storageHandler.PrintSettingBool(settings.logging, SETTINGS_BOOL_X, SETTINGS_TEXT_ROW2_Y);
}

void Settings4Screen::HandleLoggingOff()
{
    settings.logging = false;
    storageHandler.PrintSettingBool(settings.logging, SETTINGS_BOOL_X, SETTINGS_TEXT_ROW2_Y);
}

void Settings4Screen::HandleMachineLearningOn()
{
    settings.machineLearning = true;
    storageHandler.PrintSettingBool(settings.machineLearning, SETTINGS_BOOL_X, SETTINGS_TEXT_ROW3_Y);
}

void Settings4Screen::HandleMachineLearningOff()
{
    settings.machineLearning = false;
    storageHandler.PrintSettingBool(settings.machineLearning, SETTINGS_BOOL_X, SETTINGS_TEXT_ROW3_Y);
}

void Settings4Screen::HandleBluetoothOn()
{
    settings.bluetooth = true;
    storageHandler.PrintSettingBool(settings.bluetooth, SETTINGS_BOOL_X, SETTINGS_TEXT_ROW4_Y);
}

void Settings4Screen::HandleBluetoothOff()
{
    settings.bluetooth = false;
    storageHandler.PrintSettingBool(settings.bluetooth, SETTINGS_BOOL_X, SETTINGS_TEXT_ROW4_Y);
}

void Settings4Screen::HandleSettings3()
{
    screenManager.RequestScreen(EScreen::SETTINGS3);
}

void Settings4Screen::HandleCalibration()
{
    screenManager.RequestScreen(EScreen::CALIBRATION);
}
