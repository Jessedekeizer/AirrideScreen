#include <Arduino.h>
#include "SPI.h"
#include "Button.h"
#include "Screen.h"
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
void checkTouchedBox(int, int);

String getValue(String, char, int);
void getPressure();
void sendSettings();

// Define the box coordinates
/* std::vector<Button> mainScreenButtons = {
    {0, 0, 70, 120, "Front Up", true, false},
    {0, 120, 70, 120, "Front Down", true, false},
    {263, 0, 70, 120, "Back Up", true, false},
    {263, 120, 70, 120, "Back Down", true, false},
    {132, 174, 48, 42, "Settings1", false, true},
    {84, 174, 48, 42, "Ride", false, false},
    {186, 174, 48, 42, "Park", false, false}}; */

/* //std::vector<Button> settings1Buttons = {
    {20, 20, 50, 50, "mainScreen", false, true},
    {245, 20, 50, 50, "save", false, true},
    {120, 80, 30, 30, "RideFD", false, true},
    {205, 80, 30, 30, "RideFU", false, true},
    {120, 113, 30, 30, "RideBD", false, true},
    {205, 113, 30, 30, "RideBU", false, true},
    {120, 158, 30, 30, "ParkFD", false, true},
    {205, 158, 30, 30, "ParkFU", false, true},
    {120, 190, 30, 30, "ParkBD", false, true},
    {205, 190, 30, 30, "ParkBU", false, true},
    {260, 190, 30, 30, "Settings2", false, true}}; */

/* //std::vector<Button> settings2Buttons = {
    {20, 20, 50, 50, "mainScreen", false, true},
    {245, 20, 50, 50, "save", false, true},
    {120, 80, 30, 30, "MaxFD", false, true},
    {205, 80, 30, 30, "MaxFU", false, true},
    {120, 113, 30, 30, "MaxBD", false, true},
    {205, 113, 30, 30, "MaxBU", false, true},
    {120, 158, 30, 30, "MinFD", false, true},
    {205, 158, 30, 30, "MinFU", false, true},
    {120, 190, 30, 30, "MinBD", false, true},
    {205, 190, 30, 30, "MinBU", false, true},
    {20, 190, 30, 30, "Settings1", false, true}}; */

double frontMin = 0;
double frontMax = 0;
double backMin = 0;
double backMax = 0;
double rideFront = 0;
double rideBack = 0;
double parkFront = 0;
double parkBack = 0;

double front = 0;
double back = 0;

bool buttonHandler(String);

Screen mainScreen("mainScreen", "/MainScreen.png");
Screen settings1("Settings1", "/Settings1.png");
Screen settings2("Settings2", "/Settings2.png");

ScreenManager Manager(&mainScreen);

String incoming = "";

//====================================================================================
//                                    Setup
//====================================================================================
void setup()
{
  Serial.begin(9600, SERIAL_8N1);
  Serial2.begin(9600, SERIAL_8N1, 27, 22);
  Serial.setTimeout(20);

  ts.begin();

  TFTStorageHandler &storageHandler = TFTStorageHandler::getInstance();

  // mainScreen.addButtons(mainScreenButtons);
  // settings1.addButtons(settings1Buttons);
  // settings2.addButtons(settings2Buttons);
  mainScreen.activate(true);
  // printScreen(SD, mainScreen.getPath());
  Manager.addScreen(&mainScreen);
  Manager.addScreen(&settings1);
  Manager.addScreen(&settings2);

  // testing

  //

  // readAirSuspensionData(SD);
  delay(2000);
  sendSettings();
  // printPressure();
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
  TouchPoint touch = ts.getTouch();
  if (Manager.getActiveScreen()->getName() == "mainScreen")
  {
    getPressure();
    // printPressure();
  }
  if (touch.zRaw != 0)
  {
    // printTouchToSerial(touch);
    String button = Manager.getActiveScreen()->handleTouch(touch.x, touch.y);
    if (!buttonHandler(button))
    {
      if (button == "save")
      {
        // writeSettings(SD);
        sendSettings();
        Manager.changeActiveScreen("mainScreen");
      }
      else
      {
        Manager.changeActiveScreen(button);
      }
    }
    else
    {
      if (Manager.getActiveScreen()->getName() == "mainScreen")
      {
        // printPressure();
      }
      else if (Manager.getActiveScreen()->getName() == "Settings1")
      {
        // printSettings(true);
      }
      else if (Manager.getActiveScreen()->getName() == "Settings2")
      {
        // printSettings(false);
      }
    }
  }
  else
  {
    Manager.getActiveScreen()->releaseButton();
  }

  if (Manager.getChange())
  {
    Manager.Change(false);
    // printScreen(SD, Manager.getActiveScreen()->getPath());

    if (Manager.getActiveScreen()->getName() == "mainScreen")
    {
      // printPressure();
    }
    else if (Manager.getActiveScreen()->getName() == "Settings1")
    {
      // readAirSuspensionData(SD);
      // printSettings(true);
    }
    else if (Manager.getActiveScreen()->getName() == "Settings2")
    {
      // printSettings(false);
    }
  }
  delay(100);
}

void sendSettings()
{
  Serial2.print("settings/" + String(frontMin) +
                "/" + frontMax +
                "/" + backMin +
                "/" + backMax +
                "/" + rideFront +
                "/" + rideBack +
                "/" + parkFront +
                "/" + parkBack + "/");
}

void getPressure()
{
  while (Serial2.available())
  {
    delay(2);
    char c = Serial2.read();
    incoming += c;
  }
  if (incoming != "")
  {
    Serial.println(incoming);
    try
    {
      front = getValue(incoming, '/', 0).toDouble();
      back = getValue(incoming, '/', 1).toDouble();
      Serial.print(front);
      Serial.println(back);
    }
    catch (const std::exception &e)
    {
    }
    incoming = "";
  }
}

String getValue(String data, char separator, int index)
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

bool buttonHandler(String button)
{

  if (button.startsWith("Ride"))
  {
    if (button.endsWith("FU"))
    {
      rideFront += 0.1;
    }
    else if (button.endsWith("FD"))
    {
      rideFront -= 0.1;
    }
    else if (button.endsWith("BU"))
    {
      rideBack += 0.1;
    }
    else if (button.endsWith("BD"))
    {
      rideBack -= 0.1;
    }
  }
  else if (button.startsWith("Park"))
  {
    if (button.endsWith("FU"))
    {
      parkFront += 0.1;
    }
    else if (button.endsWith("FD"))
    {
      parkFront -= 0.1;
    }
    else if (button.endsWith("BU"))
    {
      parkBack += 0.1;
    }
    else if (button.endsWith("BD"))
    {
      parkBack -= 0.1;
    }
  }
  else if (button.startsWith("Max"))
  {
    if (button.endsWith("FU"))
    {
      frontMax += 0.1;
    }
    else if (button.endsWith("FD"))
    {
      frontMax -= 0.1;
    }
    else if (button.endsWith("BU"))
    {
      backMax += 0.1;
    }
    else if (button.endsWith("BD"))
    {
      backMax -= 0.1;
    }
  }
  else if (button.startsWith("Min"))
  {
    if (button.endsWith("FU"))
    {
      frontMin += 0.1;
    }
    else if (button.endsWith("FD"))
    {
      frontMin -= 0.1;
    }
    else if (button.endsWith("BU"))
    {
      backMin += 0.1;
    }
    else if (button.endsWith("BD"))
    {
      backMin -= 0.1;
    }
  }
  else
  {
    return false;
  }
  return true;
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
