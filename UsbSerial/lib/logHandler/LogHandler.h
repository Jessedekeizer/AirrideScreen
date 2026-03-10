#ifndef LOGHANDLER_H
#define LOGHANDLER_H
#include "PressureSensor.h"
#include "MainCommunication.h"
#include "LogHandlerCommunication.h"

class LogHandler {
public:
    LogHandler(LogHandlerCommunication &communication, PressureSensor &frontPressureSensor, PressureSensor &backPressureSensor, PressureSensor &tankPressureSensor);
    ~LogHandler();

    void StartFrontLog(bool togetherMove = false);

    void EndFrontLog();

    void StartBackLog(bool togetherMove = false);

    void EndBackLog();

    void SendLog();

private:
    String CreateLogMessage(String message, double startPressure, double endPressure, double startTankPressure, long time, bool direction, bool togetherMove);
    unsigned long startTimeFront = 0;
    double startPressureFront = 0;
    double startTankPressureFront = 0;
    bool togetherMoveFront = false;
    bool sendLogFront = false;

    unsigned long startTimeBack = 0;
    double startPressureBack = 0;
    double startTankPressureBack = 0;
    bool togetherMoveBack = false;
    bool sendLogBack = false;

    int timeInterval = 100;
    unsigned long frontLogPreviousTime = 0;
    unsigned long backLogPreviousTime = 0;

    LogHandlerCommunication &communication;

    PressureSensor &frontPressureSensor;
    PressureSensor &backPressureSensor;
    PressureSensor &tankPressureSensor;
};
#endif
