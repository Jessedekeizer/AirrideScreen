#include <MainScreen.h>
#include "ScreenManager.h"

MainScreen::MainScreen()
{
    name = "MainScreen";
    path = "/MainScreen.png";

    buttons = std::vector<Button *>();

    // Front buttons
    buttons.push_back(new ToggleButton(0, 0, 70, 120, "Front Up",
                                       [this](Button &button)
                                       { HandleFrontUp(button); }));
    buttons.push_back(new ToggleButton(0, 120, 70, 120, "Front Down",
                                       [this](Button &button)
                                       { HandleFrontDown(button); }));

    // Back buttons
    buttons.push_back(new ToggleButton(263, 0, 70, 120, "Back Up",
                                       [this](Button &button)
                                       { HandleBackUp(button); }));
    buttons.push_back(new ToggleButton(263, 120, 70, 120, "Back Down",
                                       [this](Button &button)
                                       { HandleBackDown(button); }));

    // Control buttons
    buttons.push_back(new PushButton(132, 174, 48, 42, "Settings1",
                                     [this](Button &button)
                                     { GoToSettings1(); }));
    buttons.push_back(new PushButton(84, 174, 48, 42, "Ride",
                                     [this](Button &button)
                                     { SendRideCommand(); }));
    buttons.push_back(new PushButton(186, 174, 48, 42, "Park",
                                     [this](Button &button)
                                     { SendParkCommand(); }));
};

void MainScreen::OnSetup()
{
    serialManager.Debug("MainScreen::OnSetup - Setting callback");
    serialManager.setMessageCallback([this](String message)
                                     {
                                         serialManager.Debug("Processing message: " + message);
                                         if (message.startsWith("BAR"))
                                         {
                                             try
                                             {
                                                 front = getValue(message, '/', 1).toDouble();
                                                 back = getValue(message, '/', 2).toDouble();
                                                 storageHandler.PrintPressure(front, back);
                                             }
                                             catch (const std::exception &e)
                                             {
                                                 serialManager.Debug("Error parsing BAR message");
                                             }
                                         }
                                         // ...existing code...
                                     });
    serialManager.Debug("MainScreen::OnSetup - Callback set complete");
}

String MainScreen::getValue(String data, char separator, int index)
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

void MainScreen::GoToSettings1()
{
    serialManager.clearMessageCallback();
    screenManager.ChangeScreen("Settings1");
}

void MainScreen::SendRideCommand()
{
    serialManager.sendMessage("Ride");
}

void MainScreen::SendParkCommand()
{
    serialManager.sendMessage("Park");
}

void MainScreen::HandleFrontUp(Button &sender)
{
    auto &toggle = static_cast<ToggleButton &>(sender);
    if (toggle.GetState())
    {
        serialManager.sendMessage("Front Up On");
    }
    else
    {
        serialManager.sendMessage("Front Up Off");
    }
}

void MainScreen::HandleFrontDown(Button &sender)
{
    auto &toggle = static_cast<ToggleButton &>(sender);
    if (toggle.GetState())
    {
        serialManager.sendMessage("Front Down On");
    }
    else
    {
        serialManager.sendMessage("Front Down Off");
    }
}

void MainScreen::HandleBackUp(Button &sender)
{
    auto &toggle = static_cast<ToggleButton &>(sender);
    if (toggle.GetState())
    {
        serialManager.sendMessage("Back Up On");
    }
    else
    {
        serialManager.sendMessage("Back Up Off");
    }
}

void MainScreen::HandleBackDown(Button &sender)
{
    auto &toggle = static_cast<ToggleButton &>(sender);
    if (toggle.GetState())
    {
        serialManager.sendMessage("Back Down On");
    }
    else
    {
        serialManager.sendMessage("Back Down Off");
    }
}