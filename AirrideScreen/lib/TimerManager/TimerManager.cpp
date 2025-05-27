#include "TimerManager.h"

TimerManager &timerManager = TimerManager::GetInstance();

TimerManager::TimerManager() : count(0)
{
    for (int i = 0; i < MAX_TIMERS; ++i)
    {
        timers[i] = nullptr;
    }
}

bool TimerManager::addTimer(Timer *timer)
{
    if (count >= MAX_TIMERS)
        return false;
    timers[count++] = timer;
    return true;
}

void TimerManager::update()
{
    for (int i = 0; i < count; ++i)
    {
        if (timers[i])
        {
            timers[i]->update();
        }
    }
}

bool TimerManager::removeTimer(Timer *timer)
{
    for (int i = 0; i < count; ++i)
    {
        if (timers[i] == timer)
        {
            for (int j = i; j < count - 1; ++j)
            {
                timers[j] = timers[j + 1];
            }
            timers[--count] = nullptr;
            return true;
        }
    }
    return false;
}
