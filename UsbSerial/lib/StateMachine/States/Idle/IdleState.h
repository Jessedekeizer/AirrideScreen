#ifndef USBSERIAL_IDLESTATE_H
#define USBSERIAL_IDLESTATE_H
#include "IState.h"


class IdleState : public IState {
public:
    IdleState() {
    };

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override {
    }

    EState Loop() override { return state; }

private:
    const EState state = EState::IDLE;
};


#endif //USBSERIAL_IDLESTATE_H
