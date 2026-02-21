#ifndef USBSERIAL_BACKDOWNSTATE_H
#define USBSERIAL_BACKDOWNSTATE_H
#include "IState.h"
#include "Solenoid.h"


class BackDownState : public IState {
public:
    BackDownState() {
    };

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override { return state; };

private:
    const EState state = EState::BACK_DOWN;
    Solenoid *solenoid;
};


#endif //USBSERIAL_BACKDOWNSTATE_H
