#ifndef LOGHANDLER_H
#define LOGHANDLER_H
#pragma once
#include "PressureSensor.h"

class LogHandler {
public:
    LogHandler() {
    }

    void Begin();

    void StartFrontLog(bool togetherMove = false);

    void EndFrontLog();

    void StartBackLog(bool togetherMove = false);

    void EndBackLog();

private:
    long startTimeFront;
    double startPressureFront;
    double startTankPressureFront;
    bool togetherMoveFront = false;
    bool logFrontRunning = false;

    long startTimeBack;
    double startPressureBack;
    double startTankPressureBack;
    bool togetherMoveBack = false;
    bool logBackRunning = false;

    PressureSensor *frontPressureSensor;
    PressureSensor *backPressureSensor;
    PressureSensor *tankPressureSensor;
};

extern LogHandler logHandler;
#endif
