#ifndef USBSERIAL_FRONTUPSTATE_H
#define USBSERIAL_FRONTUPSTATE_H
#include "IState.h"
#include "Solenoid.h"


class FrontUpState : public IState {
public:
    FrontUpState() {
    };

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override { return state; }

private:
    const EState state = EState::FRONT_UP;
    Solenoid *solenoid;
};


#endif //USBSERIAL_FRONTUPSTATE_H
