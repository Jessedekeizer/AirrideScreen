#pragma once
#include "IScreen.h"
#include <memory>
#include <vector>

class ScreenManager
{
public:
    static ScreenManager &GetInstance()
    {
        static ScreenManager instance;
        return instance;
    }

    bool ChangeScreen(const String &screenName);
    IScreen *GetActiveScreen() const { return activeScreen; }

    ScreenManager(const ScreenManager &) = delete;
    ScreenManager &operator=(const ScreenManager &) = delete;

private:
    ScreenManager();
    std::vector<IScreen *> screens;
    IScreen *activeScreen;

    IScreen *FindScreen(const String &screenName);
    void TransitionToScreen(IScreen *newScreen);
};

extern ScreenManager &screenManager;