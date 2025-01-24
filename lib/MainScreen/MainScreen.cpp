#include <MainScreen.h>
#include "ScreenManager.h"

MainScreen::MainScreen()
{
    name = "MainScreen",
    path = "/MainScreen.png",
    buttons = std::vector<Button>{
        {0, 0, 70, 120, "Front Up", ButtonType::Toggle, [this](Button &button)
         { HandleFrontUp(button); }},
        {0, 120, 70, 120, "Front Down", ButtonType::Toggle, [this](Button &button)
         { HandleFrontDown(button); }},
        {263, 0, 70, 120, "Back Up", ButtonType::Toggle, [this](Button &button)
         { HandleBackUp(button); }},
        {263, 120, 70, 120, "Back Down", ButtonType::Toggle, [this](Button &button)
         { HandleBackDown(button); }},
        {132, 174, 48, 42, "Settings1", ButtonType::push, [this](Button &button)
         { GoToSettings1(); }},
        {84, 174, 48, 42, "Ride", ButtonType::push, [this](Button &button)
         { SendRideCommand(); }},
        {186, 174, 48, 42, "Park", ButtonType::push, [this](Button &button)
         { SendParkCommand(); }}};
};

void MainScreen::HandleTouch(int touchX, int touchY)
{
    for (Button &button : buttons)
    {
        if (button.CheckTouch(touchX, touchY))
        {
            button.OnPress(true);
        }
        else
        {
            button.OnPress(false);
        }
    }
}

void MainScreen::ReleaseButtons()
{
    for (Button &button : buttons)
    {
        button.OnPress(false);
    }
}

void MainScreen::OnSetup()
{
    serialManager.setMessageCallback([this](String message)
                                     {
        if (message.startsWith("BAR")) {
            try {
                front = getValue(message, '/', 1).toDouble();
                back = getValue(message, '/', 2).toDouble();
            } catch (const std::exception &e) {}
            storageHandler.PrintPressure(front, back);
        }
        if (message.startsWith("LOG")) {
            int semiColonIndex = message.indexOf(";");
            storageHandler.WriteLog(message.substring(0, semiColonIndex + 1));
        } });
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
    if (sender.GetToggle())
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
    if (sender.GetToggle())
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
    if (sender.GetToggle())
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
    if (sender.GetToggle())
    {
        serialManager.sendMessage("Back Down On");
    }
    else
    {
        serialManager.sendMessage("Back Down Off");
    }
}