#include "Settings2Screen.h"

Settings2Screen::Settings2Screen()
{
    name = "Settings2";
    path = "/Settings2.png";
    buttons = std::vector<Button *>();
    buttons.push_back(new PushButton(10, 10, 50, 50, "MainScreen",
                                     [this](Button &button)
                                     { HandleMainScreen(); }));

    buttons.push_back(new PushButton(260, 10, 50, 50, "save",
                                     [this](Button &button)
                                     { HandleSave(); }));

    buttons.push_back(new PushButton(126, 73, 35, 35, "FrontUS",
                                     [this](Button &button)
                                     { HandleFrontUpSub(); }));

    buttons.push_back(new PushButton(218, 73, 35, 35, "FrontUA",
                                     [this](Button &button)
                                     { HandleFrontUpAdd(); }));

    buttons.push_back(new PushButton(126, 112, 35, 35, "FrontDS",
                                     [this](Button &button)
                                     { HandleFrontDownSub(); }));

    buttons.push_back(new PushButton(218, 112, 35, 35, "FrontDA",
                                     [this](Button &button)
                                     { HandleFrontDownAdd(); }));

    buttons.push_back(new PushButton(126, 159, 35, 35, "BackUS",
                                     [this](Button &button)
                                     { HandleBackUpSub(); }));

    buttons.push_back(new PushButton(218, 159, 35, 35, "BackUA",
                                     [this](Button &button)
                                     { HandleBackUpAdd(); }));

    buttons.push_back(new PushButton(126, 198, 35, 35, "BackDS",
                                     [this](Button &button)
                                     { HandleBackDownSub(); }));

    buttons.push_back(new PushButton(218, 198, 35, 35, "BackDA",
                                     [this](Button &button)
                                     { HandleBackDownAdd(); }));
    buttons.push_back(new PushButton(10, 198, 30, 30, "Settings1",
                                     [this](Button &button)
                                     { HandleSettings1(); }));

    buttons.push_back(new PushButton(275, 198, 30, 30, "Settings2",
                                     [this](Button &button)
                                     { HandleSettings3(); }));
}

void Settings2Screen::OnSetup()
{
    auto &settings = storageHandler.getSettings();
    storageHandler.DrawString(String(settings.backUpX, 1), 180, 82);
    storageHandler.DrawString(String(settings.backDownX, 1), 180, 122);
    storageHandler.DrawString(String(settings.frontUpX, 1), 180, 167);
    storageHandler.DrawString(String(settings.frontDownX, 1), 180, 207);
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
    storageHandler.DrawString(String(settings.frontUpX, 1), 180, 82);
}

void Settings2Screen::HandleFrontUpSub()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.frontUpX, -0.1);
    storageHandler.DrawString(String(settings.frontUpX, 1), 180, 82);
}

void Settings2Screen::HandleFrontDownAdd()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.frontDownX, 0.1);
    storageHandler.DrawString(String(settings.frontDownX, 1), 180, 122);
}

void Settings2Screen::HandleFrontDownSub()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.frontDownX, -0.1);
    storageHandler.DrawString(String(settings.frontDownX, 1), 180, 122);
}

void Settings2Screen::HandleBackUpAdd()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.backUpX, 0.1);
    storageHandler.DrawString(String(settings.backUpX, 1), 180, 167);
}

void Settings2Screen::HandleBackUpSub()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.backUpX, -0.1);
    storageHandler.DrawString(String(settings.backUpX, 1), 180, 167);
}

void Settings2Screen::HandleBackDownAdd()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.backDownX, 0.1);
    storageHandler.DrawString(String(settings.backDownX, 1), 180, 207);
}

void Settings2Screen::HandleBackDownSub()
{
    auto &settings = storageHandler.getSettings();
    settings.adjustValue(settings.backDownX, -0.1);
    storageHandler.DrawString(String(settings.backDownX, 1), 180, 207);
}

void Settings2Screen::HandleSettings1()
{
    screenManager.ChangeScreen("Settings1");
}

void Settings2Screen::HandleSettings3()
{
    screenManager.ChangeScreen("Settings3");
}