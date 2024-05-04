#ifndef ISCREEN_H
#define ISCREEN_H

#include <Arduino.h>
#include <Button.h>
#include "vector"

class IScreen {
public:
       
    virtual void HandleTouch(int touchX, int touchY);

    virtual void ConnectButton(Button *button);

    String GetName(){return name;};

    const char * GetPath(){return path;};

protected:
    String name;
    const char * path;
    bool isActive;
    #define MAX_IMAGE_WIDTH 320
    std::vector<Button> buttons;
};

#endif