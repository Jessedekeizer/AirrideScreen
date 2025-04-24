#ifndef CALIBRATIONSCREEN_H
#define CALIBRATIONSCREEN_H
#pragma once

#include "IScreen.h"
#include "TFTStorageHandler.h"
#include "SerialManager.h"
#include "XPT2046_Bitbang.h"
#include "ScreenManager.h"
#define CALIBRATION_TIME 3000
XPT2046_Bitbang *GetTouchScreen();

class CalibrationScreen : public IScreen
{
public:
    CalibrationScreen();
    void OnLoop() override;
    void OnSetup() override;

private:
    void TopLeftCalibration();
    void BottomRightCalibration();
    void HandlePreCalibrationCountdown();
    void UpdateCountdown();
    void StartTopLeftCalibration();
    void HandleTopLeftCalibration();
    void HandleBottomRightCalibration();
    void StartBottomRightCalibration();
    void SaveCalibrationAndExit();
    void ClearScreen();
    void HandlePostCalibrationCountdown();

    void HandleTopLeftStart();
    void HandleBottomRightStart();
    int xmin = 0;
    int xmax = 0;
    int ymin = 0;
    int ymax = 0;

    int seconds = 3;

    bool TopLeftCalibrationDone = false;
    bool BottomRightCalibrationDone = false;
    bool startInterval = false;
    bool preCalibrationDone = false;

    long TopLeftStartTime = 0;
    long BottomRightStartTime = 0;
    long preCalibrationTime = 0;
    long secondsTime = 0;

    long exitCountdownStart = 0;

    XPT2046_Bitbang *touchScreen;
};

#endif