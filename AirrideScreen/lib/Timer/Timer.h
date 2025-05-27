#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

typedef void (*TimerCallback)();

class Timer
{
public:
    Timer(unsigned long duration, TimerCallback callback);

    void update();
    bool isFinished() const;
    void reset();
    unsigned long getDuration() const;

private:
    unsigned long startTime;
    unsigned long duration;
    TimerCallback callback;
    bool finished;
};

#endif
// Timer.h