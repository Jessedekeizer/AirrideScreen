#include <Arduino.h>
#include "LogHandler.h"
#include "PressureSensorManager.h"
#include "Settings.h"
#include "SolenoidManager.h"
#include "MainStateMachine.h"
#include "SerialManager.h"

void setupDebugCallback();

String CreatePressureString();

void saveSettings(String);

String getValue(String, char, int);

unsigned long timePrevious = 0;
int timeInterval = 200;

MainStateMachine mainStateMachine;

void setup() {
  serialManager.Begin(true);
  analogReadResolution(14);
  pressureSensorManager.Begin();
  solenoidManager.Begin();
  logHandler.Begin();
  mainStateMachine.Begin();
  setupDebugCallback();
}

void loop() {
  if (millis() - timePrevious > timeInterval) {
    pressureSensorManager.Update();
    serialManager.SendMessage(CreatePressureString());
    timePrevious = millis();
  }
  serialManager.HandleIncoming();
  mainStateMachine.Loop();
  logHandler.SendLog();
}

void setupDebugCallback() {
  serialManager.SetMessageCallback(
    [](String message) {
      if (message.startsWith("settings")) {
        saveSettings(message);
      } else {
        mainStateMachine.RequestChangeState(message);
      }
    });
}

String CreatePressureString() {
  String message = "BAR/";
  double front = pressureSensorManager.GetPressureSensor(EPressureSensor::FRONT)->GetRawPressure();
  double rear = pressureSensorManager.GetPressureSensor(EPressureSensor::BACK)->GetRawPressure();

  return message + String(front) + "/" + String(rear) + "/";
}

void saveSettings(String settingString) {
  settings.frontMax = getValue(settingString, '/', 1).toDouble();
  settings.backMax = getValue(settingString, '/', 2).toDouble();
  settings.rideFront = getValue(settingString, '/', 3).toDouble();
  settings.rideBack = getValue(settingString, '/', 4).toDouble();
  settings.frontUpX = getValue(settingString, '/', 5).toDouble();
  settings.frontDownX = getValue(settingString, '/', 6).toDouble();
  settings.backUpX = getValue(settingString, '/', 7).toDouble();
  settings.backDownX = getValue(settingString, '/', 8).toDouble();
  settings.parkDuration = getValue(settingString, '/', 9).toDouble();
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
