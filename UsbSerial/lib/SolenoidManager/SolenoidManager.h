#ifndef USBSERIAL_SOLENOIDMANAGER_H
#define USBSERIAL_SOLENOIDMANAGER_H
#include <vector>

#include "Solenoid.h"


class SolenoidManager {
public:
    SolenoidManager();

    ~SolenoidManager();

    void Begin();

    Solenoid *GetSolenoid(ESolenoid requestedSolenoid);

    void TurnOffAllSolenoids();

private:
    std::vector<Solenoid *> solenoids;
};

extern SolenoidManager solenoidManager;

#endif //USBSERIAL_SOLENOIDMANAGER_H
