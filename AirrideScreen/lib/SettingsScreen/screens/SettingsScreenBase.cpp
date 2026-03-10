#include "SettingsScreenBase.h"
#include "TFTStorageHandler.h"

SettingsScreenBase::SettingsScreenBase(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication, SettingsDevice &settingsDevice, EScreen screen, const char* path)
    : screenManager(screenManager), settingsScreenCommunication(settingsScreenCommunication), settings(settingsDevice) , BaseScreen(screen, path) {
}

void SettingsScreenBase::SaveSettings() {
    storageHandler.WriteSettings(settings);
    settingsScreenCommunication.SendSettings(settings);
    storageHandler.ReadSettings(settings);
    screenManager.RequestScreen(EScreen::MAIN);
}

void SettingsScreenBase::GoToMainScreen() {
    screenManager.RequestScreen(EScreen::MAIN);
}
