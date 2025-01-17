#include "Settings2Screen.h"

Settings2Screen::Settings2Screen()
{
    name = "Settings2";
    path = "/Settings2.png";
    buttons = std::vector<Button>{
        {20, 20, 50, 50, "MainScreen", ButtonType::push, [this](Button &)
         { HandleMainScreen(); }},
        {245, 20, 50, 50, "save", ButtonType::push, [this](Button &)
         { HandleSave(); }},
        {120, 80, 30, 30, "FrontUS", ButtonType::push, [this](Button &)
         { HandleFrontUpSub(); }},
        {205, 80, 30, 30, "FrontUA", ButtonType::push, [this](Button &)
         { HandleFrontUpAdd(); }},
        {120, 113, 30, 30, "FrontDS", ButtonType::push, [this](Button &)
         { HandleFrontDownSub(); }},
        {205, 113, 30, 30, "FrontDA", ButtonType::push, [this](Button &)
         { HandleFrontDownAdd(); }},
        {120, 158, 30, 30, "BackUS", ButtonType::push, [this](Button &)
         { HandleBackUpSub(); }},
        {205, 158, 30, 30, "BackUA", ButtonType::push, [this](Button &)
         { HandleBackUpAdd(); }},
        {120, 190, 30, 30, "BackDS", ButtonType::push, [this](Button &)
         { HandleBackDownSub(); }},
        {205, 190, 30, 30, "BackDA", ButtonType::push, [this](Button &)
         { HandleBackDownAdd(); }},
        {20, 190, 30, 30, "Settings1", ButtonType::push, [this](Button &)
         { HandleSettings1(); }}};
}

void Settings2Screen::OnSetup()
{
    // storageHandler.ReadAirSuspensionData();
}

void Settings2Screen::OnLoop()
{
    storageHandler.PrintSettings(false);
}

void Settings2Screen::HandleTouch(int touchX, int touchY)
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

void Settings2Screen::HandleMainScreen()
{
    screenManager.RequestScreen("MainScreen");
}

void Settings2Screen::HandleSave()
{
    storageHandler.WriteSettings();
    storageHandler.sendSettings();
    storageHandler.ReadAirSuspensionData();
    screenManager.RequestScreen("MainScreen");
}

void Settings2Screen::HandleFrontUpAdd()
{
    storageHandler.frontUpX += 0.1;
}

void Settings2Screen::HandleFrontUpSub()
{
    storageHandler.frontUpX -= 0.1;
}

void Settings2Screen::HandleFrontDownAdd()
{
    storageHandler.frontDownX += 0.1;
}

void Settings2Screen::HandleFrontDownSub()
{
    storageHandler.frontDownX -= 0.1;
}

void Settings2Screen::HandleBackUpAdd()
{
    storageHandler.backUpX += 0.1;
}

void Settings2Screen::HandleBackUpSub()
{
    storageHandler.backUpX -= 0.1;
}

void Settings2Screen::HandleBackDownAdd()
{
    storageHandler.backDownX += 0.1;
}

void Settings2Screen::HandleBackDownSub()
{
    storageHandler.backDownX -= 0.1;
}

void Settings2Screen::HandleSettings1()
{
    screenManager.RequestScreen("Settings1");
}