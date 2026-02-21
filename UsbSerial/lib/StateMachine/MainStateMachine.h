#ifndef USBSERIAL_MAINSTATEMACHINE_H
#define USBSERIAL_MAINSTATEMACHINE_H
#include <Arduino.h>

#include "IState.h"


class MainStateMachine {
public:
    MainStateMachine();

    ~MainStateMachine();

    void RequestChangeState(String requestedStateMessage);

    void Begin();

    void Loop();

private:
    void ChangeState(EState newState);

    IState *currentState;
    EState requestedState;
};


#endif //USBSERIAL_MAINSTATEMACHINE_H
