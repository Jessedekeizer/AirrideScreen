#ifndef ISCREEN_H
#define ISCREEN_H
#pragma once

#include <Arduino.h>
#include <Button.h>
#include "vector"

class IScreen
{
public:
    virtual void HandleTouch(int touchX, int touchY) = 0;

    virtual void OnLoop() = 0;

    virtual void ReleaseButtons() = 0;

    virtual void OnSetup() = 0;

    String GetName() { return name; };

    const char *GetPath() { return path; };

protected:
    String name;
    const char *path;
    bool isActive;
    std::vector<Button> buttons;
};

#endif