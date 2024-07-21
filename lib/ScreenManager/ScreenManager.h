#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H
#pragma once

#include <vector>
#include "IScreen.h"


class ScreenManager
{
public:
    static ScreenManager &getInstance()
    {
        static ScreenManager instance;
        return instance;
    }

    ~ScreenManager(){};
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

#endif // SCREENMANAGER_H