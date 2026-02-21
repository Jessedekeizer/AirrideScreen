#include "LogHandler.h"
#include "Arduino.h"
#include "PressureSensorManager.h"

LogHandler logHandler;

void LogHandler::Begin() {
    frontPressureSensor = pressureSensorManager.GetPressureSensor(EPressureSensor::FRONT);
    backPressureSensor = pressureSensorManager.GetPressureSensor(EPressureSensor::BACK);
    tankPressureSensor = pressureSensorManager.GetPressureSensor(EPressureSensor::TANK);
}

void LogHandler::StartFrontLog(bool together) {
    startPressureFront = frontPressureSensor->GetRawPressure();
    startTankPressureFront = tankPressureSensor->GetRawPressure();
    startTimeFront = millis();
    togetherMoveFront = together;
    logFrontRunning = true;
}

void LogHandler::EndFrontLog() {
    if (!logFrontRunning) {
        return;
    }
    delay(100); //TODO remove
    double endPresssure = frontPressureSensor->GetRawPressure();
    bool directionFront = startPressureFront - endPresssure < 0;
    String message = "LOGF/";
    message = message + startPressureFront + "/" + endPresssure + "/" + startTankPressureFront + "/" + (
                  millis() - startTimeFront) + "/" + directionFront + "/" + togetherMoveFront + ";";
    Serial1.println(message);
}

void LogHandler::StartBackLog(bool together) {
    startPressureBack = backPressureSensor->GetRawPressure();
    startTankPressureBack = tankPressureSensor->GetRawPressure();
    startTimeBack = millis();
    togetherMoveBack = together;
    logBackRunning = true;
}

void LogHandler::EndBackLog() {
    if (!logBackRunning) {
        return;
    }
    delay(100); //TODO remove
    double endPresssure = backPressureSensor->GetRawPressure();
    bool directionBack = startPressureFront - endPresssure < 0;
    String message = "LOGB/";
    message = message + startPressureBack + "/" + endPresssure + "/" + startTankPressureBack + "/" + (
                  millis() - startTimeBack) + "/" + directionBack + "/" + togetherMoveBack + ";";
    Serial1.println(message);
}
