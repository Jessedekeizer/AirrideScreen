#include "CalibrationScreen.h"
#include "SerialManager.h"

// Constants
constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;
constexpr int TEXT_X_POS = 80;
constexpr int TEXT_Y_POS = 110;
constexpr int SCREEN_DEFAULT = 4095;
constexpr int COUNTDOWN_START = 3;
constexpr unsigned long COUNTDOWN_INTERVAL = 1000; // 1 second

CalibrationScreen::CalibrationScreen()
{
    name = "CalibrationScreen";
    path = "/CalibrationScreen.png";
    buttons = std::vector<Button *>();
    buttons.push_back(new PushButton(0, 0, 50, 50, "TopLeft",
                                     [this](Button &button)
                                     { HandleTopLeftStart(); }));

    buttons.push_back(new PushButton(270, 190, 50, 50, "BottomRight",
                                     [this](Button &button)
                                     { HandleBottomRightStart(); }));
}

void CalibrationScreen::OnLoop()
{
    if (!preCalibrationDone)
    {
        HandlePreCalibrationCountdown();
        return;
    }

    if (!TopLeftCalibrationDone)
    {
        HandleTopLeftCalibration();
        return;
    }

    if (!BottomRightCalibrationDone)
    {
        HandleBottomRightCalibration();
        return;
    }

    HandlePostCalibrationCountdown();
}

void CalibrationScreen::HandlePreCalibrationCountdown()
{
    if (millis() - preCalibrationTime < CALIBRATION_TIME)
    {
        UpdateCountdown();
    }
    else
    {
        StartTopLeftCalibration();
    }
}

void CalibrationScreen::HandlePostCalibrationCountdown()
{
    if (millis() - exitCountdownStart < CALIBRATION_TIME)
    {
        UpdateCountdown();
    }
    else
    {
        screenManager.ChangeScreen("MainScreen");
    }
}

void CalibrationScreen::UpdateCountdown()
{
    if (millis() - secondsTime > COUNTDOWN_INTERVAL)
    {
        secondsTime = millis();
        storageHandler.DrawRect(150, TEXT_Y_POS, 10, 10);
        storageHandler.DrawString(String(--seconds), 160, TEXT_Y_POS);
    }
}

void CalibrationScreen::StartTopLeftCalibration()
{
    preCalibrationDone = true;
    TopLeftCalibrationDone = false;
    startInterval = false;
    ClearScreen();
    storageHandler.PrintImage("/Cross.png", 0, 0);
    storageHandler.DrawString("Press the top left corner", TEXT_X_POS, TEXT_Y_POS);
    TopLeftStartTime = 0;
    BottomRightStartTime = 0;
}

void CalibrationScreen::HandleTopLeftCalibration()
{
    if (startInterval && (millis() - TopLeftStartTime > CALIBRATION_TIME))
    {
        StartBottomRightCalibration();
    }
    else
    {
        TopLeftCalibration();
    }
}

void CalibrationScreen::HandleBottomRightCalibration()
{
    if (startInterval && (millis() - BottomRightStartTime > CALIBRATION_TIME))
    {
        SaveCalibrationAndExit();
    }
    else
    {
        BottomRightCalibration();
    }
}

void CalibrationScreen::StartBottomRightCalibration()
{
    startInterval = false;
    ClearScreen();
    storageHandler.DrawString("Press the bottom right corner", TEXT_X_POS, TEXT_Y_POS);
    storageHandler.PrintImage("/Cross.png", 300, 220);
    TopLeftCalibrationDone = true;
}

void CalibrationScreen::SaveCalibrationAndExit()
{
    BottomRightCalibrationDone = true;
    exitCountdownStart = millis();
    secondsTime = millis();
    ClearScreen();
    seconds = COUNTDOWN_START;
    storageHandler.DrawString("Exit calibration in:", 100, 90);
    storageHandler.DrawString(String(seconds), 160, TEXT_Y_POS);
    // adjust for middle of cross
    xmin -= (SCREEN_DEFAULT / SCREEN_WIDTH) * 10;
    xmax += (SCREEN_DEFAULT / SCREEN_WIDTH) * 10;
    ymin -= (SCREEN_DEFAULT / SCREEN_HEIGHT) * 10;
    ymax += (SCREEN_DEFAULT / SCREEN_HEIGHT) * 10;
    serialManager.Debug("Saving calibration values: " + String(xmin) + " " + String(xmax) + " " + String(ymin) + " " + String(ymax));
    touchScreen->setCalibration(xmin, xmax, ymin, ymax);
    SettingsDevice &settings = storageHandler.getSettings();
    settings.xmin = xmin;
    settings.xmax = xmax;
    settings.ymin = ymin;
    settings.ymax = ymax;
    settings.calibrationSet = true;
    storageHandler.WriteSettings();
}

void CalibrationScreen::OnSetup()
{
    touchScreen = GetTouchScreen();
    secondsTime = millis();
    preCalibrationTime = millis();
    preCalibrationDone = false;
    TopLeftCalibrationDone = false;
    BottomRightCalibrationDone = false;
    startInterval = false;
    xmin = 0;
    ymin = 0;
    xmax = SCREEN_DEFAULT;
    ymax = SCREEN_DEFAULT;
    TopLeftStartTime = 0;
    BottomRightStartTime = 0;
    seconds = COUNTDOWN_START;
    ClearScreen();
    storageHandler.DrawString("Starting calibration in:", 100, 90);
    storageHandler.DrawString(String(seconds), 160, TEXT_Y_POS);
}

void CalibrationScreen::ClearScreen()
{
    storageHandler.DrawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void CalibrationScreen::TopLeftCalibration()
{

    TouchPoint touch = touchScreen->getTouch();
    if (touch.zRaw != 0)
    {
        xmin = touch.xRaw;
        ymin = touch.yRaw;
        serialManager.Debug("Xmin: " + String(xmin) + " Ymin: " + String(ymin));
    }
}

void CalibrationScreen::HandleTopLeftStart()
{
    if (TopLeftStartTime == 0)
    {
        serialManager.Debug("TopLeft starting");
        startInterval = true;
        TopLeftStartTime = millis();
        return;
    }
}
void CalibrationScreen::BottomRightCalibration()
{

    TouchPoint touch = touchScreen->getTouch();
    if (touch.zRaw != 0)
    {
        xmax = touch.xRaw;
        ymax = touch.yRaw;
        serialManager.Debug("Xmax: " + String(xmax) + " Ymax: " + String(ymax));
    }
}

void CalibrationScreen::HandleBottomRightStart()
{
    if (TopLeftCalibrationDone && BottomRightStartTime == 0)
    {
        serialManager.Debug("BottomRight starting");
        startInterval = true;
        BottomRightStartTime = millis();
        return;
    }
}
