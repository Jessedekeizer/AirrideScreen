#ifndef SETTINGS4SCREEN_H
#define SETTINGS4SCREEN_H
#pragma once

#include "IScreen.h"
#include "ScreenManager.h"
#include "TFTStorageHandler.h"

class Settings4Screen : public IScreen
{
public:
    Settings4Screen();
    void OnLoop() override {};
    void OnSetup() override;

private:
    void HandleMainScreen();
    void HandleSave();
    void HandleSettings3();
    void HandleParkDurationAdd();
    void HandleParkDurationSub();
    void HandleLoggingOn();
    void HandleLoggingOff();
    void HandleMachineLearningOn();
    void HandleMachineLearningOff();
    void HandleBluetoothOn();
    void HandleBluetoothOff();
};

#endif