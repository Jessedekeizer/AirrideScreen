#include <ScreenManager.h>
#include "MainScreen.h"
#include "Settings1Screen.h"
#include "Settings2Screen.h"
#include "TFTStorageHandler.h"

ScreenManager::ScreenManager()
{
    IScreen *mainScreen = new MainScreen();
    IScreen *settings1Screen = new Settings1Screen();
    IScreen *settings2Screen = new Settings2Screen();
    screens.push_back(mainScreen);
    screens.push_back(settings1Screen);
    screens.push_back(settings2Screen);
    activeScreen = screens.at(0);
    TFTStorageHandler &storageHandler = TFTStorageHandler::getInstance();
    storageHandler.PrintScreen(activeScreen->GetPath());
}

void ScreenManager::Change()
{
    if (!change)
    {
        return;
    }
    activeScreen->ReleaseButtons();
    activeScreen = newActiveScreen;
    TFTStorageHandler &storageHandler = TFTStorageHandler::getInstance();
    storageHandler.PrintScreen(activeScreen->GetPath());
    newActiveScreen = nullptr;
    change = false;
}

void ScreenManager::RequestScreen(String requestedScreen)
{
    if (requestedScreen != "")
    {
        for (IScreen *screen : screens)
        {
            if (screen->GetName() == requestedScreen)
            {
                newActiveScreen = screen;
                change = true;
                return;
            }
        }
    }
}