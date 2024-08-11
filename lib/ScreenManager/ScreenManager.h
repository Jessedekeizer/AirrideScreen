#pragma once

#include <vector>
#include "IScreen.h"
#include <memory>

class ScreenManager
{
public:
    ScreenManager(const ScreenManager &) = delete;
    ScreenManager &operator=(const ScreenManager &) = delete;

    static ScreenManager &GetInstance()
    {
        static ScreenManager instance;
        return instance;
    }

    
    void Change();
    void RequestScreen(String);
    IScreen *&GetActiveScreen() { return activeScreen; };

private:
    ScreenManager();
    std::vector<IScreen *> screens;
    IScreen *activeScreen;
    IScreen *newActiveScreen;
    bool change;
};

extern ScreenManager &screenManager;