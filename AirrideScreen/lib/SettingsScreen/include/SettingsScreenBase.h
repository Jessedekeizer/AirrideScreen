#ifndef SETTINGSSCREENBASE_H
#define SETTINGSSCREENBASE_H
#include "BaseScreen.h"
#include "ScreenManager.h"
#include "SettingsScreenCommunication.h"
#include "SettingsScreenGeometry.h"

class SettingsScreenBase : public BaseScreen
{
public:
    SettingsScreenBase(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication, SettingsDevice &settingsDevice, EScreen screen, const char* path);

protected:
    void SaveSettings();
    void GoToMainScreen();
    ScreenManager &screenManager;
    SettingsDevice &settings;

private:
    SettingsScreenCommunication &settingsScreenCommunication;
};

#endif // SETTINGSSCREENBASE_H
