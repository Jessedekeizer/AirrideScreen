#ifndef USBSERIAL_FRONTDOWNSTATE_H
#define USBSERIAL_FRONTDOWNSTATE_H
#include "IState.h"
#include "Solenoid.h"

class FrontDownState : public IState {
public:
    FrontDownState() : solenoid(nullptr) {
    };

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override { return state; }

private:
    const EState state = EState::FRONT_DOWN;
    Solenoid *solenoid;
};


#endif //USBSERIAL_FRONTDOWNSTATE_H
