#include "Settings2Screen.h"

Settings2Screen::Settings2Screen()
{
    name = "Settings2";
    path = "/Settings2.png";
    buttons = std::vector<Button *>();
    buttons.push_back(new PushButton(20, 20, 50, 50, "MainScreen",
                                     [this](Button &button)
                                     { HandleMainScreen(); }));

    buttons.push_back(new PushButton(245, 20, 50, 50, "save",
                                     [this](Button &button)
                                     { HandleSave(); }));

    buttons.push_back(new PushButton(120, 80, 30, 30, "FrontUS",
                                     [this](Button &button)
                                     { HandleFrontUpSub(); }));

    buttons.push_back(new PushButton(205, 80, 30, 30, "FrontUA",
                                     [this](Button &button)
                                     { HandleFrontUpAdd(); }));

    buttons.push_back(new PushButton(120, 113, 30, 30, "FrontDS",
                                     [this](Button &button)
                                     { HandleFrontDownSub(); }));

    buttons.push_back(new PushButton(205, 113, 30, 30, "FrontDA",
                                     [this](Button &button)
                                     { HandleFrontDownAdd(); }));

    buttons.push_back(new PushButton(120, 158, 30, 30, "BackUS",
                                     [this](Button &button)
                                     { HandleBackUpSub(); }));

    buttons.push_back(new PushButton(205, 158, 30, 30, "BackUA",
                                     [this](Button &button)
                                     { HandleBackUpAdd(); }));

    buttons.push_back(new PushButton(120, 190, 30, 30, "BackDS",
                                     [this](Button &button)
                                     { HandleBackDownSub(); }));

    buttons.push_back(new PushButton(205, 190, 30, 30, "BackDA",
                                     [this](Button &button)
                                     { HandleBackDownAdd(); }));
}

void Settings2Screen::OnLoop()
{
    auto &settings = storageHandler.getSettings();
    storageHandler.DrawString(String(settings.frontUpX, 1), 180, 82);
    storageHandler.DrawString(String(settings.frontDownX, 1), 180, 122);
    storageHandler.DrawString(String(settings.backUpX, 1), 180, 167);
    storageHandler.DrawString(String(settings.backDownX, 1), 180, 207);
}

void Settings2Screen::HandleMainScreen()
{
    screenManager.ChangeScreen("MainScreen");
}

void Settings2Screen::HandleSave()
{
    storageHandler.WriteSettings();
    storageHandler.SendSettings();
    storageHandler.ReadSettings();
    screenManager.ChangeScreen("MainScreen");
}

void Settings2Screen::HandleFrontUpAdd()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.frontUpX, 0.1);
}

void Settings2Screen::HandleFrontUpSub()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.frontUpX, -0.1);
}

void Settings2Screen::HandleFrontDownAdd()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.frontDownX, 0.1);
}

void Settings2Screen::HandleFrontDownSub()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.frontDownX, -0.1);
}

void Settings2Screen::HandleBackUpAdd()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.backUpX, 0.1);
}

void Settings2Screen::HandleBackUpSub()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.backUpX, -0.1);
}

void Settings2Screen::HandleBackDownAdd()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.backDownX, 0.1);
}

void Settings2Screen::HandleBackDownSub()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.backDownX, -0.1);
}

void Settings2Screen::HandleSettings1()
{
    screenManager.ChangeScreen("Settings1");
}