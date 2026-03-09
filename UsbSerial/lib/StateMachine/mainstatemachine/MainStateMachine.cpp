#include "../include/MainStateMachine.h"
#include "../States/BackDown/BackDownState.h"
#include "../States/BackUp/BackUpState.h"
#include "../States/FrontDown/FrontDownState.h"
#include "../States/FrontUp/FrontUpState.h"
#include "../States/Idle/IdleState.h"
#include "../States/Park/ParkState.h"
#include "../States/Ride/RideState.h"

MainStateMachine::MainStateMachine(MainStateMachineData &mainStateMachineData,
                                   MainStateMachineCommunication &mainStateMachineCommunication,
                                   SolenoidManager &solenoidManager, LogHandler &logHandler,
                                   PressureSensorManager &pressureSensorManager)
    : mainStateMachineData(mainStateMachineData), mainStateMachineCommunication(mainStateMachineCommunication),
      solenoidManager(solenoidManager), logHandler(logHandler), pressureSensorManager(pressureSensorManager) {
}

MainStateMachine::~MainStateMachine() {
    currentState->Leave();
    delete currentState;
    currentState = nullptr;
}

void MainStateMachine::Begin() {
    requestedState = EState::IDLE;
    mainStateMachineData.newRequestedState = requestedState;
    ChangeState(requestedState);
}

void MainStateMachine::Loop() {
    if (mainStateMachineData.newRequestedState != requestedState) {
        requestedState = mainStateMachineData.newRequestedState;
    }
    if (currentState) {
        EState stateRequestedByState = currentState->Loop();
        if (currentState->GetEState() != requestedState) {
            ChangeState(requestedState);
            return;
        }
        if (currentState->GetEState() != stateRequestedByState) {
            ChangeState(stateRequestedByState);
        }
    }
}

void MainStateMachine::ChangeState(EState newState) {
    if (currentState) {
        currentState->Leave();
        delete currentState;
    }
    switch (newState) {
        case EState::IDLE:
            currentState = new IdleState(solenoidManager);
            break;
        case EState::FRONT_UP:
            currentState = new FrontUpState(solenoidManager.GetSolenoid(ESolenoid::FRONT_UP), logHandler);
            break;
        case EState::FRONT_DOWN:
            currentState = new FrontDownState(solenoidManager.GetSolenoid(ESolenoid::FRONT_DOWN), logHandler);
            break;
        case EState::BACK_UP:
            currentState = new BackUpState(solenoidManager.GetSolenoid(ESolenoid::BACK_UP), logHandler);
            break;
        case EState::BACK_DOWN:
            currentState = new BackDownState(solenoidManager.GetSolenoid(ESolenoid::BACK_DOWN), logHandler);
            break;
        case EState::PARK:
            currentState = new ParkState(solenoidManager.GetSolenoid(ESolenoid::FRONT_DOWN),
                                         solenoidManager.GetSolenoid(ESolenoid::FRONT_DOWN), logHandler, settings);
            break;
        case EState::RIDE:
            currentState = new RideState(solenoidManager, logHandler, settings,
                                         pressureSensorManager.GetPressureSensor(EPressureSensor::FRONT),
                                         pressureSensorManager.GetPressureSensor(EPressureSensor::BACK));
            break;
    }
    requestedState = newState;
    currentState->Enter();
}
