#ifndef ISCREEN_H
#define ISCREEN_H
#pragma once

#include <Arduino.h>
#include <Button.h>
#include <vector>

class IScreen
{
public:
    void HandleTouch(int touchX, int touchY)
    {
        for (Button *button : buttons)
        {
            button->CheckTouch(touchX, touchY);
        };
    }

    virtual void OnLoop() = 0;

    void ReleaseButtons()
    {
        for (Button *button : buttons)
        {
            button->ReleaseButton();
        }
    };

    virtual void OnSetup() = 0;

    String GetName() { return name; };

    const char *GetPath() { return path; };

protected:
    String name;
    const char *path;
    bool isActive;
    std::vector<Button *> buttons;
};

#endif