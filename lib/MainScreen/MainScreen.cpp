#include <MainScreen.h>
#include "ScreenManager.h"

MainScreen::MainScreen() { name = "MainScreen",
                           path = "/MainScreen.png",
                           buttons = std::vector<Button>{
                               {0, 0, 70, 120, "Front Up", true},
                               {0, 120, 70, 120, "Front Down", true},
                               {263, 0, 70, 120, "Back Up", true},
                               {263, 120, 70, 120, "Back Down", true},
                               {132, 174, 48, 42, "Settings1"},
                               {84, 174, 48, 42, "Ride"},
                               {186, 174, 48, 42, "Park"}}; };

void MainScreen::HandleTouch(int touchX, int touchY)
{
    for (Button &button : buttons)
    {
        String buttonName = button.GetName();
        switch (button.CheckTouch(touchX, touchY))
        {
        case ButtonState::Push:
            if (buttonName == "Ride" || buttonName == "Park")
            {
                Serial2.println(buttonName);
            }
            if (buttonName == "Settings1")
            {
                std::shared_ptr<ScreenManager> screenManager = ScreenManager::instance();
                screenManager->RequestScreen(buttonName);
            }
            break;
        case ButtonState::ToggleOff:
            Serial2.println(buttonName + " Off");
            break;
        case ButtonState::ToggleOn:
            Serial2.println(buttonName + " On");
            break;
        case ButtonState::Idle:
            break;
        }
    }
}

void MainScreen::ReleaseButtons()
{
    for (Button &button : buttons)
    {
        if (button.GetToggleState())
        {
            Serial2.println(button.GetName() + " Off");
        }
    }
}

void MainScreen::OnLoop()
{
    getPressure();
    Serial.println(front + back);
    storageHandler->PrintPressure(front, back);
}

void MainScreen::getPressure()
{
    while (Serial2.available())
    {
        delay(2);
        char c = Serial2.read();
        incomingMessage += c;
    }
    if (incomingMessage != "")
    {
        Serial.println(incomingMessage);
        try
        {
            front = getValue(incomingMessage, '/', 0).toDouble();
            back = getValue(incomingMessage, '/', 1).toDouble();
            Serial.print(front);
            Serial.println(back);
        }
        catch (const std::exception &e)
        {
        }
        incomingMessage = "";
    }
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
