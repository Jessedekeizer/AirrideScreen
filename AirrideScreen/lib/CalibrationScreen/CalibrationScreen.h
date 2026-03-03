#ifndef CALIBRATIONSCREEN_H
#define CALIBRATIONSCREEN_H

#include "IScreen.h"
#include "TFTStorageHandler.h"
#include "XPT2046_Bitbang.h"
#include "ECalibrationState.h"
#include "ScreenManager.h"

#define CALIBRATION_TIME 3000
XPT2046_Bitbang *GetTouchScreen();

class CalibrationScreen : public IScreen
{
public:
    CalibrationScreen(ScreenManager &screenManager, SettingsDevice &settings);
    void OnLoop() override;
    void OnSetup() override;

private:
    void ChangeCalibrationState(ECalibrationState newState);
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

    ECalibrationState calibrationState;

    bool TopLeftCalibrationDone = false;
    bool BottomRightCalibrationDone = false;
    bool preCalibrationDone = false;

    long TopLeftStartTime = 0;
    long BottomRightStartTime = 0;
    long preCalibrationTime = 0;
    long secondsTime = 0;

    long exitCountdownStart = 0;

    XPT2046_Bitbang *touchScreen;
    SettingsDevice &settings;
    ScreenManager &screenManager;
};

#endif