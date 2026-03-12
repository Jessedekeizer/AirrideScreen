#include "CalibrationScreen.h"
#include "CalibrationGeometry.h"
#include "ECalibrationScreenButtons.h"

#include "Logger.h"

// Constants for calibration process
#define CALIBRATION_TIME 3000
constexpr int SCREEN_DEFAULT = 4095;
constexpr int COUNTDOWN_START = 3;
constexpr unsigned long COUNTDOWN_INTERVAL = 1000; // 1 second

CalibrationScreen::CalibrationScreen(ScreenManager &screenManager, SettingsDevice &settings, XPT2046_Bitbang &touchScreen)
    : screenManager(screenManager), settings(settings), touchScreen(touchScreen), BaseScreen(EScreen::CALIBRATION, "/CalibrationScreen.png")
{
    buttons.push_back(new PushButton(CAL_BTN_TOPLEFT_X, CAL_BTN_TOPLEFT_Y, CAL_BTN_SIZE, CAL_BTN_SIZE, TOP_LEFT,
                                     [this](Button &button)
                                     { HandleTopLeftStart(); }));

    buttons.push_back(new PushButton(CAL_BTN_BOTTOMRIGHT_X, CAL_BTN_BOTTOMRIGHT_Y, CAL_BTN_SIZE, CAL_BTN_SIZE, BOTTOM_RIGHT,
                                     [this](Button &button)
                                     { HandleBottomRightStart(); }));
}

void CalibrationScreen::OnSetup()
{
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
    storageHandler.DrawString("Starting calibration in:", CAL_TEXT_CENTER_X, CAL_TEXT_COUNT_Y);
    storageHandler.DrawString(String(seconds), CAL_TEXT_NUM_X, CAL_TEXT_Y);
    ChangeCalibrationState(ECalibrationState::START_CALIBRATION);
}

void CalibrationScreen::OnLoop()
{
    switch (calibrationState)
    {
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
        screenManager.RequestScreen(EScreen::MAIN);
    }
}

void CalibrationScreen::UpdateCountdown()
{
    if (millis() - secondsTime > COUNTDOWN_INTERVAL)
    {
        secondsTime = millis();
        storageHandler.DrawRect(150, CAL_TEXT_Y, CAL_SQUARE_SIZE, CAL_SQUARE_SIZE);
        storageHandler.DrawString(String(--seconds), CAL_TEXT_NUM_X, CAL_TEXT_Y);
    }
}

void CalibrationScreen::StartTopLeftCalibration()
{
    ClearScreen();
    storageHandler.PrintImage("/Cross.png", 0, 0);
    storageHandler.DrawString("Press the top left corner", CAL_TEXT_X, CAL_TEXT_Y);
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
    storageHandler.DrawString("Press the bottom right corner", CAL_TEXT_X, CAL_TEXT_Y);
    storageHandler.PrintImage("/Cross.png", 300, 220);
    ChangeCalibrationState(ECalibrationState::TOUCH_BOTTOM_RIGHT);
}

void CalibrationScreen::SaveCalibrationAndExit()
{
    exitCountdownStart = millis();
    secondsTime = millis();
    ClearScreen();
    seconds = COUNTDOWN_START;
    storageHandler.DrawString("Exit calibration in:", CAL_TEXT_CENTER_X, CAL_TEXT_COUNT_Y);
    storageHandler.DrawString(String(seconds), CAL_TEXT_NUM_X, CAL_TEXT_Y);
    // adjust for middle of cross
    xmin -= (SCREEN_DEFAULT / CAL_SCR_WIDTH) * CAL_SQUARE_SIZE;
    xmax += (SCREEN_DEFAULT / CAL_SCR_WIDTH) * CAL_SQUARE_SIZE;
    ymin -= (SCREEN_DEFAULT / CAL_SCR_HEIGHT) * CAL_SQUARE_SIZE;
    ymax += (SCREEN_DEFAULT / CAL_SCR_HEIGHT) * CAL_SQUARE_SIZE;
    LOG_DEBUG("Saving calibration values:", xmin, xmax, ymin, ymax);
    touchScreen.setCalibration(xmin, xmax, ymin, ymax);

    settings.xmin = xmin;
    settings.xmax = xmax;
    settings.ymin = ymin;
    settings.ymax = ymax;
    settings.calibrationSet = true;
    storageHandler.WriteSettings(settings);
    ChangeCalibrationState(ECalibrationState::EXIT_CALIBRATION);
}

void CalibrationScreen::ChangeCalibrationState(ECalibrationState newState)
{
    calibrationState = newState;
}

void CalibrationScreen::ClearScreen()
{
    storageHandler.DrawRect(0, 0, CAL_SCR_WIDTH, CAL_SCR_HEIGHT);
}

void CalibrationScreen::TopLeftCalibration()
{
    TouchPoint touch = touchScreen.getTouch();
    if (touch.zRaw != 0)
    {
        xmin = touch.xRaw;
        ymin = touch.yRaw;
        LOG_DEBUG("Xmin:", xmin, "Ymin:", ymin);
    }
}

void CalibrationScreen::HandleTopLeftStart()
{
    if (calibrationState == ECalibrationState::TOUCH_TOP_LEFT)
    {
        LOG_DEBUG("TopLeft starting");
        TopLeftStartTime = millis();
        ChangeCalibrationState(ECalibrationState::TOP_LEFT_COUNTDOWN);
    }
}

void CalibrationScreen::BottomRightCalibration()
{
    TouchPoint touch = touchScreen.getTouch();
    if (touch.zRaw != 0)
    {
        xmax = touch.xRaw;
        ymax = touch.yRaw;
        LOG_DEBUG("Xmax:", xmax, "Ymax:", ymax);
    }
}

void CalibrationScreen::HandleBottomRightStart()
{
    if (calibrationState == ECalibrationState::TOUCH_BOTTOM_RIGHT)
    {
        LOG_DEBUG("BottomRight starting");
        BottomRightStartTime = millis();
        ChangeCalibrationState(ECalibrationState::BOTTOM_RIGHT_COUNTDOWN);
    }
}
