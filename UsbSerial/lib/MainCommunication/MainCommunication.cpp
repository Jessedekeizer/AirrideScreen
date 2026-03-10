#include "MainCommunication.h"

MainCommunication::MainCommunication(Communication &communication, Settings &settings)
    : communication(communication), settings(settings), communicationId(-1) {
}

void MainCommunication::SendPressure(double front, double back) {
    communication.SendMessage("BAR/" + String(front) + "/" + String(back) + "/");
}

void MainCommunication::Init() {
    communicationId = communication.Subscribe([this](String &message) { ReceiveCallback(message); });
}

void MainCommunication::Leave() {
    communication.Unsubscribe(communicationId);
}

void MainCommunication::ReceiveCallback(String &message) {
    if (message.startsWith("settings")) {
        SaveSettings(message);
    }
}

void MainCommunication::SaveSettings(String &settingString) {
    settings.frontMax = GetValue(settingString, '/', 1).toDouble();
    settings.backMax = GetValue(settingString, '/', 2).toDouble();
    settings.rideFront = GetValue(settingString, '/', 3).toDouble();
    settings.rideBack = GetValue(settingString, '/', 4).toDouble();
    settings.frontUpX = GetValue(settingString, '/', 5).toDouble();
    settings.frontDownX = GetValue(settingString, '/', 6).toDouble();
    settings.backUpX = GetValue(settingString, '/', 7).toDouble();
    settings.backDownX = GetValue(settingString, '/', 8).toDouble();
    settings.parkDuration = GetValue(settingString, '/', 9).toDouble();
}

String MainCommunication::GetValue(String data, char separator, int index) {
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
