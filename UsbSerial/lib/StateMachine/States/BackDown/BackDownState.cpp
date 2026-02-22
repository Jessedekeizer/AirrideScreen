#include "BackDownState.h"

#include "LogHandler.h"
#include "SolenoidManager.h"

void BackDownState::Enter() {
    solenoid = solenoidManager.GetSolenoid(ESolenoid::BACK_DOWN);
    logHandler.StartBackLog();
    solenoid->TurnOn();
}

void BackDownState::Leave() {
    solenoid->TurnOff();
    logHandler.EndBackLog();
    solenoid = nullptr;
}
