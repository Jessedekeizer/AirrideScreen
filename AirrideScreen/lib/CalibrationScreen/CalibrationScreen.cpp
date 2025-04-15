#include "CalibrationScreen.h"

CalibrationScreen::CalibrationScreen()
{
    name = "CalibrationScreen";
    path = "/CalibrationScreen.png";

    buttons = std::vector<Button *>();

    buttons.push_back(new PushButton(20, 20, 50, 50, "MainScreen",
                                     [this](Button &button)
                                     { TopLeftCalibration(); }));

    buttons.push_back(new PushButton(245, 20, 50, 50, "save",
                                     [this](Button &button)
                                     { BottomRightCalibration(); }));
}

void CalibrationScreen::OnLoop()
{
    if (!TopLeftCalibrationDone && (millis() - TopLeftInterval > CALIBRATION_TIME))
    {
        BottomRightInterval = millis();
        TopLeftCalibrationDone = true;
    }

    if (TopLeftCalibrationDone && (millis() - BottomRightInterval > CALIBRATION_TIME))
    {
        // todo: done
    }
}

void CalibrationScreen::OnSetup()
{
    touchScreen = getTouchScreen();
    TopLeftInterval = millis();
    TopLeftCalibrationDone = false;
    // todo draw a circle on the screen
}

void CalibrationScreen::TopLeftCalibration()
{
    if (millis() - TopLeftInterval < CALIBRATION_TIME)
    {
        TouchPoint touch = touchScreen->getTouch();

        xmin = touch.x;
        ymin = touch.y;
    }
}
void CalibrationScreen::BottomRightCalibration()
{
    if (millis() - BottomRightInterval < CALIBRATION_TIME)
    {
        TouchPoint touch = touchScreen->getTouch();

        xmax = touch.x;
        ymax = touch.y;
    }
}