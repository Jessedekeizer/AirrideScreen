#include "CalibrationScreen.h"
#include "SerialManager.h"

// Constants
constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;
constexpr int TEXT_X_POS = 80;
constexpr int TEXT_Y_POS = 90;
constexpr int PRE_POST_X_POS = 100;
constexpr int NUMBER_X_POS = 160;
constexpr int NUMBER_Y_POS = 110;
constexpr int SQUARE_SIZE = 10;
constexpr int SCREEN_DEFAULT = 4095;
constexpr int COUNTDOWN_START = 3;
constexpr unsigned long COUNTDOWN_INTERVAL = 1000; // 1 second

CalibrationScreen::CalibrationScreen(ScreenManager &screenManager, SettingsDevice &settings) : screenManager(screenManager), settings(settings)
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

void CalibrationScreen::OnSetup()
{
    touchScreen = GetTouchScreen();
    secondsTime = millis();
    preCalibrationTime = millis();
    xmin = 0;
    ymin = 0;
    xmax = SCREEN_DEFAULT;
    ymax = SCREEN_DEFAULT;
    TopLeftStartTime = 0;
    BottomRightStartTime = 0;
    seconds = COUNTDOWN_START;
    ClearScreen();
    storageHandler.DrawString("Starting calibration in:", PRE_POST_X_POS, TEXT_Y_POS);
    storageHandler.DrawString(String(seconds), NUMBER_X_POS, NUMBER_Y_POS);
    ChangeCalibrationState(ECalibrationState::START_CALIBRATION);
}

void CalibrationScreen::OnLoop()
{
    switch (calibrationState) {
        case ECalibrationState::START_CALIBRATION:
            HandlePreCalibrationCountdown();
            break;
        case ECalibrationState::TOUCH_TOP_LEFT:
        case ECalibrationState::TOUCH_BOTTOM_RIGHT:
            break;
        case ECalibrationState::TOP_LEFT_COUNTDOWN:
            HandleTopLeftCalibration();
            break;
        case ECalibrationState::BOTTOM_RIGHT_COUNTDOWN:
            HandleBottomRightCalibration();
            break;
        case ECalibrationState::EXIT_CALIBRATION:
            HandlePostCalibrationCountdown();
            break;
    }
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
        storageHandler.DrawRect(150, NUMBER_Y_POS, SQUARE_SIZE, SQUARE_SIZE);
        storageHandler.DrawString(String(--seconds), NUMBER_X_POS, NUMBER_Y_POS);
    }
}

void CalibrationScreen::StartTopLeftCalibration()
{
    ClearScreen();
    storageHandler.PrintImage("/Cross.png", 0, 0);
    storageHandler.DrawString("Press the top left corner", TEXT_X_POS, TEXT_Y_POS);
    TopLeftStartTime = 0;
    BottomRightStartTime = 0;
    ChangeCalibrationState(ECalibrationState::TOUCH_TOP_LEFT);
}

void CalibrationScreen::HandleTopLeftCalibration()
{
    if (millis() - TopLeftStartTime > CALIBRATION_TIME)
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
    if (millis() - BottomRightStartTime > CALIBRATION_TIME)
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
    ClearScreen();
    storageHandler.DrawString("Press the bottom right corner", TEXT_X_POS, TEXT_Y_POS);
    storageHandler.PrintImage("/Cross.png", 300, 220);
    ChangeCalibrationState(ECalibrationState::TOUCH_BOTTOM_RIGHT);
}

void CalibrationScreen::SaveCalibrationAndExit()
{
    exitCountdownStart = millis();
    secondsTime = millis();
    ClearScreen();
    seconds = COUNTDOWN_START;
    storageHandler.DrawString("Exit calibration in:", PRE_POST_X_POS, TEXT_Y_POS);
    storageHandler.DrawString(String(seconds), NUMBER_X_POS, NUMBER_Y_POS);
    // adjust for middle of cross
    xmin -= (SCREEN_DEFAULT / SCREEN_WIDTH) * SQUARE_SIZE;
    xmax += (SCREEN_DEFAULT / SCREEN_WIDTH) * SQUARE_SIZE;
    ymin -= (SCREEN_DEFAULT / SCREEN_HEIGHT) * SQUARE_SIZE;
    ymax += (SCREEN_DEFAULT / SCREEN_HEIGHT) * SQUARE_SIZE;
    serialManager.Debug("Saving calibration values: " + String(xmin) + " " + String(xmax) + " " + String(ymin) + " " + String(ymax));
    touchScreen->setCalibration(xmin, xmax, ymin, ymax);

    settings.xmin = xmin;
    settings.xmax = xmax;
    settings.ymin = ymin;
    settings.ymax = ymax;
    settings.calibrationSet = true;
    storageHandler.WriteSettings(settings);
    ChangeCalibrationState(ECalibrationState::EXIT_CALIBRATION);
}

void CalibrationScreen::ChangeCalibrationState(ECalibrationState newState) {
    calibrationState = newState;
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
    if (calibrationState == ECalibrationState::TOUCH_TOP_LEFT)
    {
        serialManager.Debug("TopLeft starting");
        TopLeftStartTime = millis();
        ChangeCalibrationState(ECalibrationState::TOP_LEFT_COUNTDOWN);
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
    if (calibrationState == ECalibrationState::TOUCH_BOTTOM_RIGHT)
    {
        serialManager.Debug("BottomRight starting");
        BottomRightStartTime = millis();
        ChangeCalibrationState(ECalibrationState::BOTTOM_RIGHT_COUNTDOWN);
    }
}
