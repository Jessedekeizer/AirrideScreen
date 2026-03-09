#ifndef MAINSTATEMACHINE_H
#define MAINSTATEMACHINE_H
#include <Arduino.h>
#include "SolenoidManager.h"
#include "IState.h"
#include "LogHandler.h"
#include "MainStateMachineCommunication.h"
#include "MainStateMachineData.h"
#include "PressureSensorManager.h"

class MainStateMachine {
public:
    MainStateMachine(MainStateMachineData &mainStateMachineData,
                     MainStateMachineCommunication &mainStateMachineCommunication, SolenoidManager &solenoidManager,
                     LogHandler &logHandler,
                     PressureSensorManager &pressureSensorManager);

    ~MainStateMachine();

    void Begin();

    void Loop();

private:
    void ChangeState(EState newState);

    EState requestedState;
    IState *currentState;
    SolenoidManager &solenoidManager;
    LogHandler &logHandler;
    PressureSensorManager &pressureSensorManager;
    MainStateMachineCommunication &mainStateMachineCommunication;
    MainStateMachineData &mainStateMachineData;
};

#endif //MAINSTATEMACHINE_H
