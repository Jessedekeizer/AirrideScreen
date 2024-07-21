#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H
#pragma once

#include <vector>
#include "IScreen.h"
#include <memory>

class ScreenManager
{
public:
    ScreenManager(ScreenManager const &) = delete;
    ScreenManager &operator=(ScreenManager const &) = delete;

    static std::shared_ptr<ScreenManager> instance()
    {
        static std::shared_ptr<ScreenManager> s{new ScreenManager};
        return s;
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