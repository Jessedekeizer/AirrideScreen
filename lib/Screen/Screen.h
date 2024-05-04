#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Box.h>
#include <TFT_eSPI.h>
#include <PNGdec.h>
#include "FS.h"
#include "SD.h"
#include "vector"

class Screen {
public:
    Screen(String name, const char * path)
        : name(name), path(path), isActive(false) {};

    void addButtons(const std::vector<Button>& boxesToAdd);
    
    String handleTouch(int touchX, int touchY);
    void releaseButton();

    void activate(bool state){isActive = state;};

    bool getIsActive(){return isActive;};

    String getName(){return name;};

    const char * getPath(){return path;};

private:
    String name;
    const char * path;
    bool isActive;
    #define MAX_IMAGE_WIDTH 320
    std::vector<Button> buttons;
};

#endif