#include "Settings3Screen.h"

#include "ESettingsScreenButtons.h"

Settings3Screen::Settings3Screen(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication, SettingsDevice &settingsDevice)
    : SettingsScreenBase(screenManager, settingsScreenCommunication, settingsDevice)
{
    name = EScreen::SETTINGS3;
    path = "/Settings3.png";
    buttons = std::vector<Button *>();
    buttons.push_back(new PushButton(SETTINGSBTN_MAIN_X, SETTINGSBTN_MAIN_Y, SETTINGSBTN_MAIN_W, SETTINGSBTN_MAIN_H, MAIN_SCREEN,
                                     [this](Button &button)
                                     { SaveSettings(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_SAVE_X, SETTINGSBTN_SAVE_Y, SETTINGSBTN_SAVE_W, SETTINGSBTN_SAVE_H, SAVE,
                                     [this](Button &button)
                                     { GoToMainScreen(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW1_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, AUTO_RIDE_SECONDS_SUBTRACT,
                                     [this](Button &button)
                                     { HandleAutoRideSecSub(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW1_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, AUTO_RIDE_SECONDS_ADD,
                                     [this](Button &button)
                                     { HandleAutoRideSecAdd(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW2_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, AUTO_RIDE_ON,
                                     [this](Button &button)
                                     { HandleAutoRideOn(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW2_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, AUTO_RIDE_OFF,
                                     [this](Button &button)
                                     { HandleAutoRideOff(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW3_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, AUTO_PARK_SECONDS_SUBTRACT,
                                     [this](Button &button)
                                     { HandleAutoParkSecSub(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW3_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, AUTO_PARK_SECONDS_ADD,
                                     [this](Button &button)
                                     { HandleAutoParkSecAdd(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW4_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, AUTO_PARK_ON,
                                     [this](Button &button)
                                     { HandleAutoParkOn(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW4_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, AUTO_PARK_OFF,
                                     [this](Button &button)
                                     { HandleAutoParkOff(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_NAV_LEFT_X, SETTINGSBTN_NAV_BOTTOM_Y, SETTINGSBTN_NAV_SIZE, SETTINGSBTN_NAV_SIZE, SETTINGS2,
                                     [this](Button &button)
                                     { HandleSettings2(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_NAV_RIGHT_X, SETTINGSBTN_NAV_BOTTOM_Y, SETTINGSBTN_NAV_SIZE, SETTINGSBTN_NAV_SIZE, SETTINGS4,
                                     [this](Button &button)
                                     { HandleSettings4(); }));
}

void Settings3Screen::OnSetup()
{
    storageHandler.DrawString(String(settings.autoRideSec, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
    storageHandler.PrintSettingBool(settings.autoRide, SETTINGS_BOOL_X, SETTINGS_BOOL_Y_ROW2);
    storageHandler.DrawString(String(settings.autoParkSec, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW3_Y);
    storageHandler.PrintSettingBool(settings.autoPark, SETTINGS_BOOL_X, SETTINGS_BOOL_Y_ROW4);
}

void Settings3Screen::HandleAutoRideSecAdd()
{
    settings.adjustValue(settings.autoRideSec, 0.1);
    storageHandler.DrawString(String(settings.autoRideSec, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
}

void Settings3Screen::HandleAutoRideSecSub()
{
    settings.adjustValue(settings.autoRideSec, -0.1);
    storageHandler.DrawString(String(settings.autoRideSec, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
}

void Settings3Screen::HandleAutoRideOn()
{
    settings.autoRide = true;
    storageHandler.PrintSettingBool(settings.autoRide, SETTINGS_BOOL_X, SETTINGS_BOOL_Y_ROW2);
}

void Settings3Screen::HandleAutoRideOff()
{
    settings.autoRide = false;
    storageHandler.PrintSettingBool(settings.autoRide, SETTINGS_BOOL_X, SETTINGS_BOOL_Y_ROW2);
}

void Settings3Screen::HandleAutoParkSecAdd()
{
    settings.adjustValue(settings.autoParkSec, 0.1);
    storageHandler.DrawString(String(settings.autoParkSec, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW3_Y);
}

void Settings3Screen::HandleAutoParkSecSub()
{
    settings.adjustValue(settings.autoParkSec, -0.1);
    storageHandler.DrawString(String(settings.autoParkSec, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW3_Y);
}

void Settings3Screen::HandleAutoParkOn()
{
    settings.autoPark = true;
    storageHandler.PrintSettingBool(settings.autoPark, SETTINGS_BOOL_X, SETTINGS_BOOL_Y_ROW4);
}

void Settings3Screen::HandleAutoParkOff()
{
    settings.autoPark = false;
    storageHandler.PrintSettingBool(settings.autoPark, SETTINGS_BOOL_X, SETTINGS_BOOL_Y_ROW4);
}

void Settings3Screen::HandleSettings2()
{
    screenManager.RequestScreen(EScreen::SETTINGS2);
}

void Settings3Screen::HandleSettings4()
{
    screenManager.RequestScreen(EScreen::SETTINGS4);
}
