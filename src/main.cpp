#include <Arduino.h>
#include "SPI.h"
#include "Button.h"
#include "IScreen.h"
#include "vector"
#include <ScreenManager.h>
#include "TFTStorageHandler.h"

// Include the TFT library https://github.com/Bodmer/TFT_eSPI
#include "SPI.h"

#define MOSI_PIN 32
#define MISO_PIN 39
#define CLK_PIN 25
#define CS_PIN 33

#include <XPT2046_Bitbang.h>

XPT2046_Bitbang ts(MOSI_PIN, MISO_PIN, CLK_PIN, CS_PIN);

void printTouchToSerial(TouchPoint);






//====================================================================================
//                                    Setup
//====================================================================================
void setup()
{
  Serial.begin(9600, SERIAL_8N1);
  delay(10000);
  Serial.println("hello world");
  Serial2.begin(9600, SERIAL_8N1, 27, 22);
  //Serial.setTimeout(20);

  ts.begin();
  ScreenManager &manager  = ScreenManager::getInstance();
  TFTStorageHandler &storageHandler = TFTStorageHandler::getInstance();
  delay(1000);
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
  ScreenManager &manager  = ScreenManager::getInstance();
  TouchPoint touch = ts.getTouch();
  if (touch.zRaw != 0)
  {
    manager.GetActiveScreen()->HandleTouch(touch.x, touch.y);
    }
  else
  {
    manager.GetActiveScreen()->ReleaseButtons();
  }

  manager.GetActiveScreen()->OnLoop();
  //manager.Change();

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
