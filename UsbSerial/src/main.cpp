#include <Arduino.h>
#include "LogHandler.h"
#include "PressureSensorManager.h"
#include "Settings.h"
#include "SolenoidManager.h"
#include "MainStateMachine.h"

String incoming = "";

void saveSettings(String);

String readPressure();

String getValue(String, char, int);

long timePrevious = 0;
int timeInterval = 200;

void smoothAnalogRead();

MainStateMachine *mainStateMachine;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600, SERIAL_8N1);
  Serial1.begin(9600, SERIAL_8N1);
  Serial1.setTimeout(20);
  analogReadResolution(14);
  pressureSensorManager.Begin();
  solenoidManager.Begin();
  logHandler.Begin();
  mainStateMachine = new MainStateMachine();
  mainStateMachine->Begin();
}

void loop() {
  while (Serial1.available()) {
    delay(2);
    char c = Serial1.read();
    incoming += c;
  }
  if (incoming != "") {
    Serial.println("incoming message:" + incoming);
  }

  if (incoming.startsWith("settings")) {
    saveSettings(incoming);
  } else {
    mainStateMachine->RequestChangeState(incoming);
  }

  if (millis() - timePrevious > timeInterval) {
    pressureSensorManager.Update();
    Serial1.println(readPressure());
    timePrevious = millis();
  }

  incoming = "";
  mainStateMachine->Loop();
}

String readPressure() {
  String message = "BAR/";
  double front = pressureSensorManager.GetPressureSensor(EPressureSensor::FRONT)->GetRawPressure();
  double rear = pressureSensorManager.GetPressureSensor(EPressureSensor::BACK)->GetRawPressure();

  return message + String(front) + "/" + String(rear) + "/";
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

void saveSettings(String incoming) {
  settings.frontMax = getValue(incoming, '/', 1).toDouble();
  settings.backMax = getValue(incoming, '/', 2).toDouble();
  settings.rideFront = getValue(incoming, '/', 3).toDouble();
  settings.rideBack = getValue(incoming, '/', 4).toDouble();
  settings.frontUpX = getValue(incoming, '/', 5).toDouble();
  settings.frontDownX = getValue(incoming, '/', 6).toDouble();
  settings.backUpX = getValue(incoming, '/', 7).toDouble();
  settings.backDownX = getValue(incoming, '/', 8).toDouble();
}
