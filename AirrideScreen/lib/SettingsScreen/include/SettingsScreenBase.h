#ifndef SETTINGSSCREENBASE_H
#define SETTINGSSCREENBASE_H
#include "IScreen.h"
#include "ScreenManager.h"
#include "SettingsScreenCommunication.h"
#include "SettingsScreenGeometry.h" // layout macros moved here

class SettingsScreenBase : public IScreen
{
public:
    SettingsScreenBase(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication, SettingsDevice &settingsDevice);

protected:
    void SaveSettings();
    void GoToMainScreen();
    ScreenManager &screenManager;
    SettingsDevice &settings;

private:
    SettingsScreenCommunication &settingsScreenCommunication;
};

#endif // SETTINGSSCREENBASE_H
