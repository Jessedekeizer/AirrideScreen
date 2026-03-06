#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H
#include "IScreen.h"
#include <memory>
#include <vector>

class ScreenManager {
public:
    ScreenManager();
    ~ScreenManager();
    void AddScreen(IScreen *screen);
    bool RequestScreen(EScreen requestedScreen);
    IScreen *GetActiveScreen() const { return activeScreen; }

private:
    std::vector<IScreen *> screens;
    IScreen *activeScreen;

    IScreen *FindScreen(EScreen newScreen);
    void TransitionToScreen(IScreen *newScreen);
};

#endif
