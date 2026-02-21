#ifndef USBSERIAL_PRESSURESENSORMANAGER_H
#define USBSERIAL_PRESSURESENSORMANAGER_H
#include <vector>

#include "PressureSensor.h"
#include "Solenoid.h"


class PressureSensorManager {
public:
    PressureSensorManager();

    PressureSensor *GetPressureSensor(EPressureSensor requestedPressureSensor);

    void Update();

    void Begin();

private:
    void CheckIfPressureIsWithinTolerance(PressureSensor *pressureSensor);

    std::vector<PressureSensor *> pressureSensors;
    Solenoid *frontSolenoid;
    Solenoid *backSolenoid;
};

extern PressureSensorManager pressureSensorManager;


#endif //USBSERIAL_PRESSURESENSORMANAGER_H
