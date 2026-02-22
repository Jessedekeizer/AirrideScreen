#ifndef USBSERIAL_RIDESTATE_H
#define USBSERIAL_RIDESTATE_H
#include "IState.h"
#include "PressureSensor.h"
#include "Solenoid.h"


class RideState : public IState {
public:
    RideState() : frontSolenoid(nullptr), backSolenoid(nullptr), frontPressureSensor(nullptr), backPressureSensor(nullptr) {
    };

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override;

private:
    const EState state = EState::RIDE;
    Solenoid *frontSolenoid;
    Solenoid *backSolenoid;
    PressureSensor *frontPressureSensor;
    PressureSensor *backPressureSensor;

    bool rideFrontDone = false;
    bool rideBackDone = false;

    long frontTimeInterval;
    long frontTimePrevious;
    long backTimeInterval;
    long backTimePrevious;

    long CalculateTime(double current, double desired, double x);

    void SetupRide();
};


#endif //USBSERIAL_RIDESTATE_H
