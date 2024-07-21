#ifndef MAINSCREEN_H
#define MAINSCREEN_H
#pragma once

#include "IScreen.h"
#include "TFTStorageHandler.h"

class MainScreen : public IScreen
{
public:
    MainScreen();
    void HandleTouch(int touchX, int touchY) override;
    void OnLoop() override;
    void ReleaseButtons() override;
    void OnSetup() override {};

private:
    TFTStorageHandler &storageHandler = TFTStorageHandler::getInstance();
    void getPressure();
    String getValue(String data, char separator, int index);
    String incomingMessage;
    double front = 0;
    double back = 0;
};

#endif