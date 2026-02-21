#include "FrontUpState.h"

#include "LogHandler.h"
#include "SolenoidManager.h"

void FrontUpState::Enter() {
    solenoid = solenoidManager.GetSolenoid(ESolenoid::FRONT_UP);
    logHandler.StartFrontLog();
    solenoid->TurnOn();
}

void FrontUpState::Leave() {
    solenoid->TurnOff();
    logHandler.EndFrontLog();
    solenoid = nullptr;
}
