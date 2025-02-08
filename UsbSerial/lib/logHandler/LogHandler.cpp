#include "LogHandler.h"
#include "Arduino.h"

void LogHandler::startFrontLog(bool together)
{
    startPressureFront = fmap(analogRead(frontSensor), AnalogMin, AnalogMax, 0, BarMax);
    startTankPressureFront = fmap(analogRead(tankSensor), AnalogMin, AnalogMax, 0, BarTankMax);
    startTimeFront = millis();
    togetherMoveFront = together;
    logFrontRunning = true;
}

void LogHandler::endFrontLog()
{
    if (!logFrontRunning)
    {
        return;
    }
    delay(100);
    double endPresssure = fmap(analogRead(frontSensor), AnalogMin, AnalogMax, 0, BarMax);
    bool directionFront = startPressureFront - endPresssure < 0;
    String message = "LOGF/";
    message = message + startPressureFront + "/" + endPresssure + "/" + startTankPressureFront + "/" + (millis() - startTimeFront) + "/" + directionFront + "/" + togetherMoveFront + ";";
    Serial1.println(message);
}

void LogHandler::startBackLog(bool together)
{
    startPressureBack = fmap(analogRead(backSensor), AnalogMin, AnalogMax, 0, BarMax);
    startTankPressureBack = fmap(analogRead(tankSensor), AnalogMin, AnalogMax, 0, BarTankMax);
    startTimeBack = millis();
    togetherMoveBack = together;
    logBackRunning = true;
}

void LogHandler::endBackLog()
{
    if (!logBackRunning)
    {
        return;
    }
    delay(100);
    double endPresssure = fmap(analogRead(backSensor), AnalogMin, AnalogMax, 0, BarMax);
    bool directionBack = startPressureFront - endPresssure < 0;
    String message = "LOGB/";
    message = message + startPressureBack + "/" + endPresssure + "/" + startTankPressureBack + "/" + (millis() - startTimeBack) + "/" + directionBack + "/" + togetherMoveBack + ";";
    Serial1.println(message);
}