#include "Settings1Screen.h"

Settings1Screen::Settings1Screen()
{
    name = "Settings1",
    path = "/Settings1.png",
    buttons = std::vector<Button>{
        {20, 20, 50, 50, "MainScreen"},
        {245, 20, 50, 50, "save"},
        {120, 80, 30, 30, "RideFD"},
        {205, 80, 30, 30, "RideFU"},
        {120, 113, 30, 30, "RideBD"},
        {205, 113, 30, 30, "RideBU"},
        {120, 158, 30, 30, "ParkFD"},
        {205, 158, 30, 30, "ParkFU"},
        {120, 190, 30, 30, "ParkBD"},
        {205, 190, 30, 30, "ParkBU"},
        {260, 190, 30, 30, "Settings2"}};
}

void Settings1Screen::OnSetup()
{
    storageHandler.ReadAirSuspensionData();
}

void Settings1Screen::OnLoop()
{
    storageHandler.PrintSettings(true);
}

void Settings1Screen::HandleTouch(int touchX, int touchY)
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
            if (buttonName == "Settings2")
            {
                screenManager.RequestScreen("Settings2");
                return;
            }

            if (buttonName.startsWith("Ride"))
            {
                if (buttonName.endsWith("FU"))
                {
                    storageHandler.rideFront += 0.1;
                }
                else if (buttonName.endsWith("FD"))
                {
                    storageHandler.rideFront -= 0.1;
                }
                else if (buttonName.endsWith("BU"))
                {
                    storageHandler.rideBack += 0.1;
                }
                else if (buttonName.endsWith("BD"))
                {
                    storageHandler.rideBack -= 0.1;
                }
            }
            else if (buttonName.startsWith("Park"))
            {
                if (buttonName.endsWith("FU"))
                {
                    storageHandler.parkFront += 0.1;
                }
                else if (buttonName.endsWith("FD"))
                {
                    storageHandler.parkFront -= 0.1;
                }
                else if (buttonName.endsWith("BU"))
                {
                    storageHandler.parkBack += 0.1;
                }
                else if (buttonName.endsWith("BD"))
                {
                    storageHandler.parkBack -= 0.1;
                }
            }
        }
    }
}