#include "MainStateMachineCommunication.h"

#include "EState.h"

MainStateMachineCommunication::MainStateMachineCommunication(Communication &communication,
                                                             MainStateMachineData &
                                                             mainStateMachineData)
    : communication(communication), mainStateMachineData(mainStateMachineData), communicationId(-1) {
}

void MainStateMachineCommunication::Init() {
    communicationId = communication.Subscribe([this](String &message) { ReceiveCallback(message); });
}

void MainStateMachineCommunication::Leave() {
    communication.Unsubscribe(communicationId);
}

void MainStateMachineCommunication::ReceiveCallback(String &message) {
    RequestChangeState(message);
}

void MainStateMachineCommunication::RequestChangeState(String &requestedStateMessage) {
    if (requestedStateMessage.equals("Front Up On")) {
        mainStateMachineData.newRequestedState = EState::FRONT_UP;
    }
    else if (requestedStateMessage.equals("Front Down On")) {
        mainStateMachineData.newRequestedState = EState::FRONT_DOWN;
    }
    else if (requestedStateMessage.equals("Back Up On")) {
        mainStateMachineData.newRequestedState = EState::BACK_UP;
    }
    else if (requestedStateMessage.equals("Back Down On")) {
        mainStateMachineData.newRequestedState = EState::BACK_DOWN;
    }
    else if (requestedStateMessage.endsWith("Off")) {
        mainStateMachineData.newRequestedState = EState::IDLE;
    }
    else if (requestedStateMessage.equals("Ride")) {
        mainStateMachineData.newRequestedState = EState::RIDE;
    }
    else if (requestedStateMessage.equals("Park")) {
        mainStateMachineData.newRequestedState = EState::PARK;
    }
}
