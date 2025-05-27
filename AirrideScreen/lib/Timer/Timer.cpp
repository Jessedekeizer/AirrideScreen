#include "Timer.h"

Timer::Timer(unsigned long durationInSeconds, TimerCallback callbackFunc)
    : duration(durationInSeconds * 1000), callback(callbackFunc), finished(false)
{
    startTime = millis();
}

void Timer::update()
{
    if (!finished && millis() - startTime >= duration)
    {
        finished = true;
        if (callback)
        {
            callback();
        }
    }
}

bool Timer::isFinished() const
{
    return finished;
}

void Timer::reset()
{
    startTime = millis();
    finished = false;
}

unsigned long Timer::getDuration() const
{
    return duration;
}
// Timer.cpp