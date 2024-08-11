#ifndef SETTINGS1SCREEN_H
#define SETTINGS1SCREEN_H
#pragma once

#include "IScreen.h"
#include "TFTStorageHandler.h"
#include "ScreenManager.h"

class Settings1Screen : public IScreen
{
public:
    Settings1Screen();
    void HandleTouch(int touchX, int touchY) override;
    void OnLoop() override;
    void ReleaseButtons() override {};
    void OnSetup() override;

};

#endif