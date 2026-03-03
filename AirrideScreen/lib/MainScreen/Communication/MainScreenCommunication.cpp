#include "MainScreenCommunication.h"
#include "SerialManager.h"
#include "TFTStorageHandler.h"

MainScreenCommunication::MainScreenCommunication(Communication &communication, MainScreenData &mainScreenData)
    : communication(communication) , mainScreenData(mainScreenData), communicationId(-1){
}

void MainScreenCommunication::Init() {
    communicationId = communication.Subscribe([this](String message){ ReceiveCallback(message);});
}

void MainScreenCommunication::SendMessage(String message) {
    communication.SendMessage(message);

}

void MainScreenCommunication::ReceiveCallback(String message) {
    serialManager.Debug("Processing message: " + message);
    if (message.startsWith("BAR"))
    {
        try
        {
            mainScreenData.front = GetValue(message, '/', 1).toDouble();
            mainScreenData.back = GetValue(message, '/', 2).toDouble();
        }
        catch (const std::exception &e)
        {
            serialManager.Debug("Error parsing BAR message");
        }
    }
    if (message.startsWith("LOG"))
    {
        int semiColonIndex = message.indexOf(";");
        //TODO remove
        storageHandler.WriteLog(message.substring(0, semiColonIndex + 1));
    }
}

void MainScreenCommunication::Leave() {
    communication.Unsubscribe(communicationId);
}

String MainScreenCommunication::GetValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
