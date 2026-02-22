#include "ParkState.h"

#include <api/Common.h>

#include "LogHandler.h"
#include "Settings.h"
#include "SolenoidManager.h"

void ParkState::Enter() {
    frontSolenoid = solenoidManager.GetSolenoid(ESolenoid::FRONT_DOWN);
    backSolenoid = solenoidManager.GetSolenoid(ESolenoid::BACK_DOWN);
    logHandler.StartFrontLog();
    logHandler.StartBackLog();
    frontSolenoid->TurnOn();
    backSolenoid->TurnOn();
    timeInterval = settings.parkDuration * 1000;
    timePrevious = millis();
}

void ParkState::Leave() {
    frontSolenoid->TurnOff();
    backSolenoid->TurnOff();
    logHandler.EndFrontLog();
    logHandler.EndBackLog();
    frontSolenoid = nullptr;
    backSolenoid = nullptr;
}

EState ParkState::Loop() {
    timeNow = millis();
    if (timeNow - timePrevious > timeInterval) {
        return EState::IDLE;
    }
    return EState::PARK;
}
