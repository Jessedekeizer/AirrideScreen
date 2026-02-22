#include "BackUpState.h"

#include "LogHandler.h"
#include "SolenoidManager.h"

void BackUpState::Enter() {
    solenoid = solenoidManager.GetSolenoid(ESolenoid::BACK_UP);
    logHandler.StartBackLog();
    solenoid->TurnOn();
}

void BackUpState::Leave() {
    solenoid->TurnOff();
    logHandler.EndBackLog();
    solenoid = nullptr;
}
