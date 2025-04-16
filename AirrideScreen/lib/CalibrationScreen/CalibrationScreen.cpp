#include "CalibrationScreen.h"

CalibrationScreen::CalibrationScreen()
{
    name = "CalibrationScreen";
    path = "/CalibrationScreen.png";

    buttons = std::vector<Button *>();

    buttons.push_back(new PushButton(0, 0, 50, 50, "TopLeft",
                                     [this](Button &button)
                                     { TopLeftCalibration(); }));

    buttons.push_back(new PushButton(270, 190, 50, 50, "BottomRight",
                                     [this](Button &button)
                                     { BottomRightCalibration(); }));
}

void CalibrationScreen::OnLoop()
{
    if (preAndPostCalibrationDone)
    {
        if (startInterval && !TopLeftCalibrationDone && (millis() - TopLeftStartTime > CALIBRATION_TIME))
        {
            startInterval = false;
            storageHandler.DrawRect(0, 0, 320, 240);
            storageHandler.DrawString("Press the bottom right corner", 80, 110);
            storageHandler.PrintImage("/Cross.png", 300, 220);
            TopLeftCalibrationDone = true;
        }

        if (startInterval && TopLeftCalibrationDone && (millis() - BottomRightStartTime > CALIBRATION_TIME))
        {
            touchScreen->setCalibration(xmin, xmax, ymin, ymax);
            preAndPostCalibrationTime = millis();
            secondsTime = millis();
            preAndPostCalibrationDone = false;
            storageHandler.DrawRect(0, 0, 320, 240);
            seconds = 3;
            storageHandler.DrawString("Ending in:", 120, 90);
            storageHandler.DrawString(String(seconds), 150, 110);
        }
        return;
    }

    if (!preAndPostCalibrationDone && (millis() - preAndPostCalibrationTime < CALIBRATION_TIME))
    {
        if (millis() - secondsTime > 1000)
        {
            secondsTime = millis();
            storageHandler.DrawRect(150, 110, 10, 10);
            storageHandler.DrawString(String(--seconds), 150, 110);
        }
    }
    else
    {
        preAndPostCalibrationDone = true;
        if (TopLeftCalibrationDone)
        {
            screenManager.ChangeScreen("MainScreen");
            return;
        }
        TopLeftCalibrationDone = false;
        startInterval = false;
        storageHandler.DrawRect(0, 0, 320, 240);
        storageHandler.PrintImage("/Cross.png", 0, 0);
        storageHandler.DrawString("Press the top left corner", 80, 110);
        TopLeftStartTime = 0;
        BottomRightStartTime = 0;
    }
}

void CalibrationScreen::OnSetup()
{
    touchScreen = getTouchScreen();
    secondsTime = millis();
    seconds = 3;
    preAndPostCalibrationDone = false;
    preAndPostCalibrationTime = millis();
    xmin = 0;
    ymin = 0;
    xmax = 320;
    ymax = 240;
    storageHandler.DrawString("Starting in:", 120, 90);
    storageHandler.DrawString(String(seconds), 150, 110);

    // todo create struct for calibration data
    // todo check on startup for calibration data
}

void CalibrationScreen::TopLeftCalibration()
{
    if (TopLeftStartTime == 0)
    {
        startInterval = true;
        TopLeftStartTime = millis();
    }
    if (millis() - TopLeftStartTime < CALIBRATION_TIME)
    {
        TouchPoint touch = touchScreen->getTouch();

        xmin = touch.xRaw;
        ymin = touch.yRaw;
    }
}
void CalibrationScreen::BottomRightCalibration()
{
    if (TopLeftCalibrationDone && BottomRightStartTime == 0)
    {
        startInterval = true;
        BottomRightStartTime = millis();
    }
    if (millis() - BottomRightStartTime < CALIBRATION_TIME)
    {
        TouchPoint touch = touchScreen->getTouch();

        xmax = touch.xRaw;
        ymax = touch.yRaw;
    }
}