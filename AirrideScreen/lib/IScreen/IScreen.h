#ifndef ISCREEN_H
#define ISCREEN_H

#include <Arduino.h>
#include <Button.h>
#include <vector>

#include "EScreens.h"

class IScreen {
public:
    virtual ~IScreen() = default;

    void HandleTouch(int touchX, int touchY) {
        for (Button *button: buttons) {
            button->CheckTouch(touchX, touchY);
        };
    }

    virtual void OnLoop() = 0;

    void ReleaseButtons() {
        for (Button *button: buttons) {
            button->ReleaseButton();
        }
    };

    virtual void OnSetup() = 0;

    EScreen GetName() { return name; };

    const char *GetPath() { return path; };

protected:
    EScreen name;
    const char *path;
    bool isActive;
    std::vector<Button *> buttons;
};

#endif
