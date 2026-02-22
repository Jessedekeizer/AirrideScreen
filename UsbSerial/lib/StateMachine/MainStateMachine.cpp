#include "MainStateMachine.h"
#include "States/BackDown/BackDownState.h"
#include "States/BackUp/BackUpState.h"
#include "States/FrontDown/FrontDownState.h"
#include "States/FrontUp/FrontUpState.h"
#include "States/Idle/IdleState.h"
#include "States/Park/ParkState.h"
#include "States/Ride/RideState.h"

void MainStateMachine::Begin() {
    requestedState = EState::IDLE;
    currentState = new IdleState();
    currentState->Enter();
}

MainStateMachine::~MainStateMachine() {
    currentState->Leave();
    delete currentState;
    currentState = nullptr;
}

void MainStateMachine::RequestChangeState(String requestedStateMessage) {
    if (requestedStateMessage.equals("Front Up On")) {
        requestedState = EState::FRONT_UP;
    } else if (requestedStateMessage.equals("Front Down On")) {
        requestedState = EState::FRONT_DOWN;
    } else if (requestedStateMessage.equals("Back Up On")) {
        requestedState = EState::BACK_UP;
    } else if (requestedStateMessage.equals("Back Down On")) {
        requestedState = EState::BACK_DOWN;
    } else if (requestedStateMessage.endsWith("Off")) {
        requestedState = EState::IDLE;
    } else if (requestedStateMessage.equals("Ride")) {
        requestedState = EState::RIDE;
    } else if (requestedStateMessage.equals("Park")) {
        requestedState = EState::PARK;
    }
}

void MainStateMachine::Loop() {
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
    currentState->Leave();
    delete currentState;
    switch (newState) {
        case EState::IDLE:
            currentState = new IdleState();
            break;
        case EState::FRONT_UP:
            currentState = new FrontUpState();
            break;
        case EState::FRONT_DOWN:
            currentState = new FrontDownState();
            break;
        case EState::BACK_UP:
            currentState = new BackUpState();
            break;
        case EState::BACK_DOWN:
            currentState = new BackDownState();
            break;
        case EState::PARK:
            currentState = new ParkState();
            break;
        case EState::RIDE:
            currentState = new RideState();
            break;
    }
    requestedState = newState;
    currentState->Enter();
}
