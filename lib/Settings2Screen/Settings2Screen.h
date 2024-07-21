#ifndef SETTINGS2SCREEN_H
#define SETTINGS2SCREEN_H
#pragma once

#include "IScreen.h"
#include "ScreenManager.h"
#include "TFTStorageHandler.h"

class Settings2Screen : public IScreen
{
public:
    Settings2Screen();
    void HandleTouch(int touchX, int touchY) override;
    void OnLoop() override;
    void ReleaseButtons() override {};
    void OnSetup() override;

private:
    TFTStorageHandler &storageHandler = TFTStorageHandler::getInstance();
    ScreenManager &screenManager = ScreenManager::getInstance();
};

#endif