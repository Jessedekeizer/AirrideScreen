#include "SolenoidManager.h"
SolenoidManager solenoidManager;

SolenoidManager::SolenoidManager() {
    solenoids = {
        new Solenoid(ESolenoid::FRONT_DOWN, 4),
        new Solenoid(ESolenoid::FRONT_UP, 5),
        new Solenoid(ESolenoid::BACK_UP, 6),
        new Solenoid(ESolenoid::BACK_DOWN, 7)
    };
}

SolenoidManager::~SolenoidManager() {
    for (auto solenoid: solenoids) {
        delete solenoid;
    }
    solenoids.clear();
}

void SolenoidManager::Begin() {
    for (auto solenoid: solenoids) {
        solenoid->Begin();
    }
}

Solenoid *SolenoidManager::GetSolenoid(ESolenoid requestedSolenoid) {
    for (auto solenoid: solenoids) {
        if (solenoid->GetESolenoid() == requestedSolenoid) {
            return solenoid;
        }
    }
    return nullptr;
}

void SolenoidManager::TurnOffAllSolenoids() {
    for (auto solenoid: solenoids) {
        solenoid->TurnOff();
    }
}


