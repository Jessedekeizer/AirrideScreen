#include "PressureSensorManager.h"
#include <Arduino.h>

#include "Settings.h"
#include "SolenoidManager.h"

PressureSensorManager pressureSensorManager;


PressureSensorManager::PressureSensorManager() {
    double analogMin = 1638.4;
    double analogMax = 14745.6;
    double barMax = 14.82;
    double barTankMax = 15.2;
    int frontFilterSize = 16;
    int backFilterSize = 10;

    pressureSensors = {
        new PressureSensor(EPressureSensor::FRONT, A0, frontFilterSize, analogMin, analogMax, barMax),
        new PressureSensor(EPressureSensor::BACK, A1, backFilterSize, analogMin, analogMax, barMax),
        new PressureSensor(EPressureSensor::TANK, A2, backFilterSize, analogMin, analogMax, barTankMax)
    };
    frontSolenoid = solenoidManager.GetSolenoid(ESolenoid::FRONT_UP);
    backSolenoid = solenoidManager.GetSolenoid(ESolenoid::BACK_UP);
}

void PressureSensorManager::Begin() {
    for (auto pressureSensor: pressureSensors) {
        pressureSensor->Begin();
    }
}

void PressureSensorManager::Update() {
    for (auto pressureSensor: pressureSensors) {
        pressureSensor->UpdateBuffer();
        CheckIfPressureIsWithinTolerance(pressureSensor);
    }
}

PressureSensor *PressureSensorManager::GetPressureSensor(EPressureSensor requestedPressureSensor) {
    for (auto pressureSensor: pressureSensors) {
        if (pressureSensor->GetPressureSensor() == requestedPressureSensor) {
            return pressureSensor;
        }
    }
}

void PressureSensorManager::CheckIfPressureIsWithinTolerance(PressureSensor *pressureSensor) {
    //Turn off solenoid when max pressure is reached.
    if (pressureSensor->GetPressureSensor() == EPressureSensor::FRONT) {
        if (pressureSensor->GetAveragePressure() >= settings.frontMax) {
            frontSolenoid->Activate(false);
        } else {
            frontSolenoid->Activate(true);
        }
    } else if (pressureSensor->GetPressureSensor() == EPressureSensor::BACK) {
        if (pressureSensor->GetAveragePressure() >= settings.backMax) {
            backSolenoid->Activate(false);
        } else {
            backSolenoid->Activate(true);
        }
    }
}
