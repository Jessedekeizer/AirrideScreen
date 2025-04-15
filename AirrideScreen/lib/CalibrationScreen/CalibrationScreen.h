#ifndef CALIBRATIONSCREEN_H
#define CALIBRATIONSCREEN_H
#pragma once

#include "IScreen.h"
#include "TFTStorageHandler.h"
#include "SerialManager.h"
#include "XPT2046_Bitbang.h"
#define CALIBRATION_TIME 3000
XPT2046_Bitbang *getTouchScreen();

class CalibrationScreen : public IScreen
{
public:
    CalibrationScreen();
    void OnLoop() override;
    void OnSetup() override;

private:
    void TopLeftCalibration();
    void BottomRightCalibration();
    int xmin = 0;
    int xmax = 0;
    int ymin = 0;
    int ymax = 0;

    long TopLeftInterval = 0;
    long BottomRightInterval = 0;
    XPT2046_Bitbang *touchScreen;
};

#endif