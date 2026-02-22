#include "LogHandler.h"
#include "Arduino.h"
#include "PressureSensorManager.h"
#include "SerialManager.h"

LogHandler logHandler;

LogHandler::~LogHandler() {
    frontPressureSensor = nullptr;
    backPressureSensor = nullptr;
    tankPressureSensor = nullptr;
}

void LogHandler::Begin() {
    frontPressureSensor = pressureSensorManager.GetPressureSensor(EPressureSensor::FRONT);
    backPressureSensor = pressureSensorManager.GetPressureSensor(EPressureSensor::BACK);
    tankPressureSensor = pressureSensorManager.GetPressureSensor(EPressureSensor::TANK);
}

void LogHandler::StartFrontLog(bool together) {
    sendLogFront = false;
    startPressureFront = frontPressureSensor->GetRawPressure();
    startTankPressureFront = tankPressureSensor->GetRawPressure();
    startTimeFront = millis();
    togetherMoveFront = together;
}

void LogHandler::EndFrontLog() {
    sendLogFront = true;
    frontLogPreviousTime = millis();
}

void LogHandler::StartBackLog(bool together) {
    sendLogBack = false;
    startPressureBack = backPressureSensor->GetRawPressure();
    startTankPressureBack = tankPressureSensor->GetRawPressure();
    startTimeBack = millis();
    togetherMoveBack = together;
}

void LogHandler::EndBackLog() {
    sendLogBack = true;
    backLogPreviousTime = millis();
}

void LogHandler::SendLog() {
    if (sendLogFront && millis() - frontLogPreviousTime > timeInterval) {
        double endPressure = frontPressureSensor->GetRawPressure();
        bool directionFront = startPressureFront - endPressure < 0;
        String message = CreateLogMessage("LOGF/", startPressureFront, endPressure, startTankPressureFront,(millis() - startTimeFront - timeInterval), directionFront, togetherMoveFront);
        serialManager.SendMessage(message);
        sendLogFront = false;
    }
    if (sendLogBack && millis() - backLogPreviousTime > timeInterval) {
        double endPressure = backPressureSensor->GetRawPressure();
        bool directionBack = startPressureFront - endPressure < 0;
        String message = CreateLogMessage("LOGB/", startPressureBack,endPressure,startTankPressureBack,(millis() - startTimeBack - timeInterval), directionBack,togetherMoveBack);
        serialManager.SendMessage(message);
        sendLogBack = false;
    }
}

String LogHandler::CreateLogMessage(String message, double startPressure, double endPressure, double startTankPressure,
    long time, bool direction, bool togetherMove) {
    return message + startPressure + "/" + endPressure + "/" + startTankPressure + "/" + time + "/" + direction + "/" + togetherMove + ";";
}