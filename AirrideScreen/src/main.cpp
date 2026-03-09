#include <Arduino.h>
#include <XPT2046_Bitbang.h>

#include "SerialOverPins.h"
#include "Communication.h"
#include "SerialManager.h"
#include "ScreenManager.h"

#include "MainScreenData.h"
#include "MainScreenCommunication.h"
#include "MainScreen.h"

#include "SettingsScreenCommunication.h"
#include "Settings1Screen.h"
#include "Settings2Screen.h"
#include "Settings3Screen.h"
#include "Settings4Screen.h"
#include "CalibrationScreen.h"

#include "TFTStorageHandler.h"
#include "TimerManager.h"

#define MOSI_PIN 32
#define MISO_PIN 39
#define CLK_PIN 25
#define CS_PIN 33

#define SERIAL_BAUD_RATE 9600
#define SERIAL2_RX_PIN 27
#define SERIAL2_TX_PIN 22

XPT2046_Bitbang touchScreen(MOSI_PIN, MISO_PIN, CLK_PIN, CS_PIN);

void printTouchToSerial(TouchPoint);
void UpdateTouchScreen();

Timer *tickTimer = nullptr;

ScreenManager screenManager;

SettingsDevice settings;

StringQueue stringQueue;

SerialOverPins serialOverPins(Serial2, stringQueue);
Communication communication(serialOverPins, stringQueue);

MainScreenData mainScreenData;
MainScreenCommunication mainScreenCommunication(communication, mainScreenData);
MainScreen mainScreen(mainScreenData, mainScreenCommunication, screenManager, settings);

SettingsScreenCommunication settingsScreenCommunication(communication, settings);
Settings1Screen settings1Screen(screenManager, settingsScreenCommunication, settings);
Settings2Screen settings2Screen(screenManager, settingsScreenCommunication, settings);
Settings3Screen settings3Screen(screenManager, settingsScreenCommunication, settings);
Settings4Screen settings4Screen(screenManager, settingsScreenCommunication, settings);

CalibrationScreen calibrationScreen(screenManager, settings, touchScreen);

void setup()
{
  Serial2.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL2_RX_PIN, SERIAL2_TX_PIN);
  // retrieve settings
  storageHandler.GetInstance();
  storageHandler.ReadSettings(settings);
  settingsScreenCommunication.SendSettings(settings);

  screenManager.AddScreen(&mainScreen);
  screenManager.AddScreen(&settings1Screen);
  screenManager.AddScreen(&settings2Screen);
  screenManager.AddScreen(&settings3Screen);
  screenManager.AddScreen(&settings4Screen);
  screenManager.AddScreen(&calibrationScreen);

  serialManager.GetInstance();
  delay(1000);
  serialManager.setDebugMode(true);
  touchScreen.begin();
  timerManager.GetInstance();

  if (!settings.calibrationSet)
  {
    screenManager.RequestScreen(EScreen::CALIBRATION);
  }
  else
  {
    touchScreen.setCalibration(settings.xmin, settings.xmax, settings.ymin, settings.ymax);
    screenManager.RequestScreen(EScreen::MAIN);
  }
  tickTimer = new Timer(0.1, []()
                        { UpdateTouchScreen(); }, true);
  timerManager.addTimer(tickTimer);
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
  timerManager.update();
  communication.CheckForMessage();
}

void UpdateTouchScreen()
{
  TouchPoint touch = touchScreen.getTouch();
  if (touch.zRaw != 0)
  {
    // printTouchToSerial(touch);
    screenManager.GetActiveScreen()->HandleTouch(touch.x, touch.y);
  }
  else
  {
    screenManager.GetActiveScreen()->ReleaseButtons();
  }
  screenManager.GetActiveScreen()->OnLoop();
}

void printTouchToSerial(TouchPoint p)
{
  serialManager.Debug("Touch: X=" + String(p.x) + ", Y=" + String(p.y) +
                      ", ZRaw=" + String(p.zRaw));
}
