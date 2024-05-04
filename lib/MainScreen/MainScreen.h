#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <IScreen.h>

class MainScreen: public IScreen{
public:
    MainScreen();
    void HandleTouch(int touchX, int touchY);
    void ConnectButton(Button *button);
    void AdjustHeight(Button *button);
};

#endif