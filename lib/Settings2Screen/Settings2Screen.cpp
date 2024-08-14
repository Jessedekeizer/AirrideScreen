#include "Settings2Screen.h"

Settings2Screen::Settings2Screen()
{
    name = "Settings2",
    path = "/Settings2.png",
    buttons = std::vector<Button>{
        {20, 20, 50, 50, "mainScreen"},
        {245, 20, 50, 50, "save"},
        {120, 80, 30, 30, "FrontUD"},
        {205, 80, 30, 30, "FrontUU"},
        {120, 113, 30, 30, "FrontDD"},
        {205, 113, 30, 30, "FrontDU"},
        {120, 158, 30, 30, "BackUD"},
        {205, 158, 30, 30, "BackUU"},
        {120, 190, 30, 30, "BackDD"},
        {205, 190, 30, 30, "BackDU"},
        {20, 190, 30, 30, "Settings1"}};
}

void Settings2Screen::OnSetup()
{
    storageHandler.ReadAirSuspensionData();
}

void Settings2Screen::OnLoop()
{
    storageHandler.PrintSettings(false);
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
                storageHandler.sendSettings();
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

            if (buttonName.startsWith("Front"))
            {
                if (buttonName.endsWith("UU"))
                {
                    storageHandler.frontUpX += 0.1;
                }
                else if (buttonName.endsWith("UD"))
                {
                    storageHandler.frontUpX -= 0.1;
                }
                else if (buttonName.endsWith("DU"))
                {
                    storageHandler.frontDownX += 0.1;
                }
                else if (buttonName.endsWith("DD"))
                {
                    storageHandler.frontDownX -= 0.1;
                }
            }
            else if (buttonName.startsWith("Back"))
            {
                if (buttonName.endsWith("UU"))
                {
                    storageHandler.backUpX += 0.1;
                }
                else if (buttonName.endsWith("UD"))
                {
                    storageHandler.backUpX -= 0.1;
                }
                else if (buttonName.endsWith("DU"))
                {
                    storageHandler.backDownX += 0.1;
                }
                else if (buttonName.endsWith("DD"))
                {
                    storageHandler.backDownX -= 0.1;
                }
            }
        }
    }
}