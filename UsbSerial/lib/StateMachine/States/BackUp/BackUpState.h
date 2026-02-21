#ifndef USBSERIAL_BACKUPSTATE_H
#define USBSERIAL_BACKUPSTATE_H
#include "IState.h"
#include "Solenoid.h"


class BackUpState : public IState {
public:
    BackUpState() {
    };

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override { return state; };

private:
    const EState state = EState::BACK_UP;
    Solenoid *solenoid;
};


#endif //USBSERIAL_BACKUPSTATE_H
