#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <vector>
#include "Screen.h"

class ScreenManager {
public:
    ScreenManager(Screen* screen ): activeScreen(screen){}
    bool getChange() {return change;};
    void Change(bool c) {change = c;}; 

    void addScreen(Screen* screen);
    void changeActiveScreen(String screenName);
    Screen*& getActiveScreen() {return activeScreen;};


private:

    std::vector<Screen*> screens;
    Screen* activeScreen;
    bool change;
};

#endif // SCREENMANAGER_H