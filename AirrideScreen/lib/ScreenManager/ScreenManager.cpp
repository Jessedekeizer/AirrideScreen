#include "ScreenManager.h"
#include "MainScreen.h"
#include "Settings1Screen.h"
#include "Settings2Screen.h"
#include "Settings3Screen.h"
#include "Settings4Screen.h"
#include "TFTStorageHandler.h"
#include "SerialManager.h"
#include "CalibrationScreen.h"

ScreenManager &screenManager = ScreenManager::GetInstance();

ScreenManager::ScreenManager()
{
    screens = {
        new MainScreen(),
        new Settings1Screen(),
        new Settings2Screen(),
        new Settings3Screen(),
        new Settings4Screen(),
        new CalibrationScreen()};
}

bool ScreenManager::ChangeScreen(const String &screenName)
{
    serialManager.Debug("ScreenManager::ChangeScreen - Changing to: " + screenName);
    if (screenName.isEmpty())
        return false;

    IScreen *newScreen = FindScreen(screenName);
    if (!newScreen || newScreen == activeScreen)
        return false;
    serialManager.Debug("ScreenManager::ChangeScreen - Transitioning to screen: " + newScreen->GetName());
    TransitionToScreen(newScreen);
    return true;
}

IScreen *ScreenManager::FindScreen(const String &screenName)
{
    for (IScreen *screen : screens)
    {
        if (screen->GetName() == screenName)
        {
            return screen;
        }
    }
    return nullptr;
}

void ScreenManager::TransitionToScreen(IScreen *newScreen)
{
    if (activeScreen)
    {
        activeScreen->ReleaseButtons();
    }
    activeScreen = newScreen;
    storageHandler.PrintImage(activeScreen->GetPath());
    activeScreen->OnSetup();
}