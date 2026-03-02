#include "MainScreenCommunication.h"

MainScreenCommunication::MainScreenCommunication(Communication &communication, MainScreenData &mainScreenData)
    : communication(communication) , mainScreenData(mainScreenData){
}

void MainScreenCommunication::Init() {
    communication.Subscribe([this](String message){ ReceiveCallback(message);});
}

void MainScreenCommunication::SendMessage() {

}

void MainScreenCommunication::ReceiveCallback(String message) {

}

void MainScreenCommunication::Leave() {
}
