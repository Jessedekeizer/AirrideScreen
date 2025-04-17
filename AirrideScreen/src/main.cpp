#include <Arduino.h>
#include <SPI.h>
#include <XPT2046_Bitbang.h>
#include "SerialManager.h"
#include "ScreenManager.h"
#include "TFTStorageHandler.h"

#define MOSI_PIN 32
#define MISO_PIN 39
#define CLK_PIN 25
#define CS_PIN 33

XPT2046_Bitbang ts(MOSI_PIN, MISO_PIN, CLK_PIN, CS_PIN);

void printTouchToSerial(TouchPoint);

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
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
  TouchPoint touch = ts.getTouch();
  if (touch.zRaw != 0)
  {
    screenManager.GetActiveScreen()->HandleTouch(touch.x, touch.y);
  }
  else
  {
    screenManager.GetActiveScreen()->ReleaseButtons();
  }

  screenManager.GetActiveScreen()->OnLoop();
  serialManager.handleIncoming();

  delay(100);
}

void printTouchToSerial(TouchPoint p)
{
  Serial.print("Pressure = ");
  Serial.print(p.zRaw);
  Serial.print(", x = ");
  Serial.print(p.x);
  Serial.print(", y = ");
  Serial.print(p.y);
  Serial.println();
}

XPT2046_Bitbang *GetTouchScreen()
{
  return &ts;
}
