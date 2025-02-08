#ifndef LOGHANDLER_H
#define LOGHANDLER_H
#pragma once

extern double AnalogMin;
extern double AnalogMax;
extern double BarMax;
extern double BarTankMax;
#define frontSensor A0
#define backSensor A1
#define tankSensor A2
extern double fmap(double, double, double, double, double);
class LogHandler
{
public:
    LogHandler() {};
    void startFrontLog(bool togetherMove = false);
    void endFrontLog();
    void startBackLog(bool togetherMove = false);
    void endBackLog();

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
};

#endif