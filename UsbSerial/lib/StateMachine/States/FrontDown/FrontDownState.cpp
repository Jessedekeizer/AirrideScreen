#include "FrontDownState.h"

#include "LogHandler.h"
#include "SolenoidManager.h"

void FrontDownState::Enter() {
    solenoid = solenoidManager.GetSolenoid(ESolenoid::FRONT_DOWN);
    logHandler.StartFrontLog();
    solenoid->TurnOn();
}

void FrontDownState::Leave() {
    solenoid->TurnOff();
    logHandler.EndFrontLog();
    solenoid = nullptr;
}
