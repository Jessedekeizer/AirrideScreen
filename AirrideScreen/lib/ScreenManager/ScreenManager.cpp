#include "ScreenManager.h"
#include "TFTStorageHandler.h"
#include "SerialManager.h"

ScreenManager::ScreenManager() {
}

ScreenManager::~ScreenManager() {
    activeScreen = nullptr;
    screens.clear();
}

void ScreenManager::AddScreen(IScreen *screen) {
    screens.push_back(screen);
}

bool ScreenManager::RequestScreen(EScreen requestedScreen) {
    serialManager.Debug("ScreenManager::ChangeScreen - Changing to: " + String(static_cast<int>(requestedScreen)));

    IScreen *newScreen = FindScreen(requestedScreen);
    if (!newScreen || newScreen == activeScreen)
        return false;
    serialManager.Debug("ScreenManager::ChangeScreen - Transitioning to screen: " + String(static_cast<int>(newScreen->GetName())));
    TransitionToScreen(newScreen);
    return true;
}

IScreen *ScreenManager::FindScreen(EScreen newScreen) {
    for (IScreen *screen: screens) {
        if (screen->GetName() == newScreen) {
            return screen;
        }
    }
    return nullptr;
}

void ScreenManager::TransitionToScreen(IScreen *newScreen) {
    if (activeScreen) {
        activeScreen->ReleaseButtons();
    }
    activeScreen = newScreen;
    storageHandler.PrintImage(activeScreen->GetPath());
    activeScreen->OnSetup();
}
