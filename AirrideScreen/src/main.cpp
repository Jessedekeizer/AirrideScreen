#include <Arduino.h>
#include <SPI.h>
#include <XPT2046_Bitbang.h>
#include "SerialManager.h"
#include "ScreenManager.h"
#include "TFTStorageHandler.h"
#include "TimerManager.h"

#define MOSI_PIN 32
#define MISO_PIN 39
#define CLK_PIN 25
#define CS_PIN 33

XPT2046_Bitbang ts(MOSI_PIN, MISO_PIN, CLK_PIN, CS_PIN);

void printTouchToSerial(TouchPoint);
void UpdateTouchScreen();

Timer *tickTimer = nullptr;

//====================================================================================
//                                    Setup
//====================================================================================
void setup()
{
  serialManager.GetInstance();
  delay(1000);
  serialManager.setDebugMode(true);
  ts.begin();
  storageHandler.GetInstance();
  screenManager.GetInstance();
  storageHandler.ReadSettings();
  storageHandler.SendSettings();
  timerManager.GetInstance();
  SettingsDevice &settings = storageHandler.getSettings();
  if (!settings.calibrationSet)
  {
    screenManager.ChangeScreen("CalibrationScreen");
  }
  else
  {
    ts.setCalibration(settings.xmin, settings.xmax, settings.ymin, settings.ymax);
    screenManager.ChangeScreen("MainScreen");
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
  serialManager.handleIncoming();
  timerManager.update();
}

void UpdateTouchScreen()
{
  // serialManager.Debug("UpdateTouchScreen called");
  TouchPoint touch = ts.getTouch();
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

XPT2046_Bitbang *GetTouchScreen()
{
  return &ts;
}
