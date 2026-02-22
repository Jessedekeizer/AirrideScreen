#ifndef USBSERIAL_PARKSTATE_H
#define USBSERIAL_PARKSTATE_H
#include "IState.h"
#include "Solenoid.h"


class ParkState : public IState {
public:
    ParkState() : frontSolenoid(nullptr), backSolenoid(nullptr) {
    };

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override;

private:
    const EState state = EState::PARK;
    Solenoid *frontSolenoid;
    Solenoid *backSolenoid;
    long timePrevious = 0;
    long timeNow = 0;

    long timeInterval = 4000;
};


#endif //USBSERIAL_PARKSTATE_H
