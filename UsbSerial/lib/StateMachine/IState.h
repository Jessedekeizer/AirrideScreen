#ifndef USBSERIAL_STATE_H
#define USBSERIAL_STATE_H
#include "EState.h"

class IState {
public:
    virtual ~IState() = default;

    IState() {
    }

    virtual EState GetEState() = 0;

    virtual void Enter() = 0;

    virtual EState Loop() = 0;

    virtual void Leave() = 0;
};

#endif //USBSERIAL_STATE_H
