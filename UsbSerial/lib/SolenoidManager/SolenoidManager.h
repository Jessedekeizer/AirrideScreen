#ifndef USBSERIAL_SOLENOIDMANAGER_H
#define USBSERIAL_SOLENOIDMANAGER_H
#include <vector>

#include "Solenoid.h"

class SolenoidManager {
public:
    SolenoidManager();

    ~SolenoidManager();

    void Begin();

    void AddSolenoid(Solenoid &solenoid);

    Solenoid &GetSolenoid(ESolenoid requestedSolenoid);

    void TurnOffAllSolenoids();

private:
    std::vector<Solenoid *> solenoids;
};

#endif //USBSERIAL_SOLENOIDMANAGER_H
