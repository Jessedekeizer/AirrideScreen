#include "IdleState.h"

#include "SolenoidManager.h"

void IdleState::Enter() {
    solenoidManager.TurnOffAllSolenoids();
}
