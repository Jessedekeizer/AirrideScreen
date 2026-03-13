#include "MainScreenCommunication.h"

#include "Logger.h"
#include "TFTStorageHandler.h"

MainScreenCommunication::MainScreenCommunication(Communication &communication, MainScreenData &mainScreenData)
    : communication(communication), mainScreenData(mainScreenData), communicationId(-1)
{
}

void MainScreenCommunication::Init()
{
    communicationId = communication.Subscribe([this](String message)
                                              { ReceiveCallback(message); });
}

void MainScreenCommunication::SendMessagePushButton(EMainScreenButtons button)
{
    String message = "";
    switch (button)
    {
    case RIDE:
    {
        message = "Ride";
        break;
    }
    case PARK:
    {
        message = "Park";
        break;
    }
    default:
    {
        return;
    }
    }
    communication.SendMessage(message);
}

void MainScreenCommunication::ReceiveCallback(String &message)
{
    LOG_DEBUG("Processing message:", message);
    if (message.startsWith("BAR"))
    {
        try
        {
            mainScreenData.front = GetValue(message, '/', 1).toDouble();
            mainScreenData.back = GetValue(message, '/', 2).toDouble();
        }
        catch (const std::exception &e)
        {
            LOG_DEBUG("Error parsing BAR message");
        }
    }
    if (message.startsWith("LOG"))
    {
        int semiColonIndex = message.indexOf(";");
        // TODO remove
        storageHandler.WriteLog(message.substring(0, semiColonIndex + 1));
    }
}

void MainScreenCommunication::Leave()
{
    communication.Unsubscribe(communicationId);
}

void MainScreenCommunication::SendToggleButtonPress(EMainScreenButtons button, bool state)
{
    String message = "";
    switch (button)
    {
    case FRONT_UP:
    {
        message = "Front Up";
        break;
    }
    case FRONT_DOWN:
    {
        message = "Front Down";
        break;
    }
    case BACK_UP:
    {
        message = "Back Up";
        break;
    }
    case BACK_DOWN:
    {
        message = "Back Down";
        break;
    }
    default:
    {
        return;
    }
    }
    if (state)
    {
        message += " On";
    }
    else
    {
        message += " Off";
    }
    communication.SendMessage(message);
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
