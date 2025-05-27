#pragma once
#include <Arduino.h>
#include "Timer.h"

#define MAX_TIMERS 10

class TimerManager
{
private:
    TimerManager();

public:
    static TimerManager &GetInstance()
    {
        static TimerManager instance;
        return instance;
    }

    TimerManager(const TimerManager &) = delete;
    TimerManager &operator=(const TimerManager &) = delete;

    bool addTimer(Timer *timer);
    void update();
    bool removeTimer(Timer *timer);

private:
    Timer *timers[MAX_TIMERS];
    int count;
};

extern TimerManager &timerManager;