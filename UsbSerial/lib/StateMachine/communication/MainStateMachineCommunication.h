#ifndef MAINSTATEMACHINECOMMUNICATION_H
#define MAINSTATEMACHINECOMMUNICATION_H
#include "Communication.h"
#include "MainStateMachineData.h"

class MainStateMachineCommunication {
public:
    MainStateMachineCommunication(Communication &communication, MainStateMachineData &mainStateMachineData);

    void Init();

    void Leave();

private:
    void RequestChangeState(String &requestedStateMessage);

    void ReceiveCallback(String &message);

    Communication &communication;
    MainStateMachineData &mainStateMachineData;
    int communicationId;
};

#endif //MAINSTATEMACHINECOMMUNICATION_H
