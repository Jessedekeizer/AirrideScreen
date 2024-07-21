#include "Settings2Screen.h"

Settings2Screen::Settings2Screen()
{
    name = "Settings2",
    path = "/Settings2.png",
    buttons = std::vector<Button>{
        {20, 20, 50, 50, "mainScreen"},
        {245, 20, 50, 50, "save"},
        {120, 80, 30, 30, "MaxFD"},
        {205, 80, 30, 30, "MaxFU"},
        {120, 113, 30, 30, "MaxBD"},
        {205, 113, 30, 30, "MaxBU"},
        {120, 158, 30, 30, "MinFD"},
        {205, 158, 30, 30, "MinFU"},
        {120, 190, 30, 30, "MinBD"},
        {205, 190, 30, 30, "MinBU"},
        {20, 190, 30, 30, "Settings1"}};
}

void Settings2Screen::OnSetup()
{
    storageHandler.ReadAirSuspensionData();
}

void Settings2Screen::OnLoop()
{
    storageHandler.PrintSettings(true);
}

void Settings2Screen::HandleTouch(int touchX, int touchY)
{
    for (Button &button : buttons)
    {
        if (button.CheckTouch(touchX, touchY) == ButtonState::Push)
        {
            String buttonName = button.GetName();

            if (buttonName == "save")
            {
                storageHandler.WriteSettings();
                screenManager.RequestScreen("MainScreen");
                return;
            }
            if (buttonName == "MainScreen")
            {
                screenManager.RequestScreen("MainScreen");
                return;
            }
            if (buttonName == "Settings1")
            {
                screenManager.RequestScreen("Settings1");
                return;
            }

            if (buttonName.startsWith("Max"))
            {
                if (buttonName.endsWith("FU"))
                {
                    storageHandler.frontMax += 0.1;
                }
                else if (buttonName.endsWith("FD"))
                {
                    storageHandler.frontMax -= 0.1;
                }
                else if (buttonName.endsWith("BU"))
                {
                    storageHandler.backMax += 0.1;
                }
                else if (buttonName.endsWith("BD"))
                {
                    storageHandler.backMax -= 0.1;
                }
            }
            else if (buttonName.startsWith("Min"))
            {
                if (buttonName.endsWith("FU"))
                {
                    storageHandler.frontMin += 0.1;
                }
                else if (buttonName.endsWith("FD"))
                {
                    storageHandler.frontMin -= 0.1;
                }
                else if (buttonName.endsWith("BU"))
                {
                    storageHandler.backMin += 0.1;
                }
                else if (buttonName.endsWith("BD"))
                {
                    storageHandler.backMin -= 0.1;
                }
            }
        }
    }
}