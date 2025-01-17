#include "Settings1Screen.h"

Settings1Screen::Settings1Screen()
{
    name = "Settings1";
    path = "/Settings1.png";
    buttons = std::vector<Button>{
        {20, 20, 50, 50, "MainScreen", ButtonType::push, [this](Button &button)
         { HandleMainScreen(); }},
        {245, 20, 50, 50, "save", ButtonType::push, [this](Button &button)
         { HandleSave(); }},
        {120, 80, 30, 30, "RideFD", ButtonType::push, [this](Button &button)
         { HandleRideFD(); }},
        {205, 80, 30, 30, "RideFU", ButtonType::push, [this](Button &button)
         { HandleRideFU(); }},
        {120, 113, 30, 30, "RideBD", ButtonType::push, [this](Button &button)
         { HandleRideBD(); }},
        {205, 113, 30, 30, "RideBU", ButtonType::push, [this](Button &button)
         { HandleRideBU(); }},
        {120, 158, 30, 30, "MaxFD", ButtonType::push, [this](Button &button)
         { HandleMaxFD(); }},
        {205, 158, 30, 30, "MaxFU", ButtonType::push, [this](Button &button)
         { HandleMaxFU(); }},
        {120, 190, 30, 30, "MaxBD", ButtonType::push, [this](Button &button)
         { HandleMaxBD(); }},
        {205, 190, 30, 30, "MaxBU", ButtonType::push, [this](Button &button)
         { HandleMaxBU(); }},
        {260, 190, 30, 30, "Settings2", ButtonType::push, [this](Button &button)
         { HandleSettings2(); }}};
}

void Settings1Screen::OnLoop()
{
    storageHandler.PrintSettings(true);
}

void Settings1Screen::HandleTouch(int touchX, int touchY)
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

void Settings1Screen::HandleMainScreen()
{
    screenManager.RequestScreen("MainScreen");
}

void Settings1Screen::HandleSave()
{
    storageHandler.WriteSettings();
    storageHandler.sendSettings();
    storageHandler.ReadAirSuspensionData();
    screenManager.RequestScreen("MainScreen");
}

void Settings1Screen::HandleRideFU()
{
    storageHandler.rideFront += 0.1;
}

void Settings1Screen::HandleRideFD()
{
    storageHandler.rideFront -= 0.1;
}

void Settings1Screen::HandleRideBU()
{
    storageHandler.rideBack += 0.1;
}

void Settings1Screen::HandleRideBD()
{
    storageHandler.rideBack -= 0.1;
}

void Settings1Screen::HandleMaxFU()
{
    storageHandler.frontMax += 0.1;
}

void Settings1Screen::HandleMaxFD()
{
    storageHandler.frontMax -= 0.1;
}

void Settings1Screen::HandleMaxBU()
{
    storageHandler.backMax += 0.1;
}

void Settings1Screen::HandleMaxBD()
{
    storageHandler.backMax -= 0.1;
}

void Settings1Screen::HandleSettings2()
{
    screenManager.RequestScreen("Settings2");
}