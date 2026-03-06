#include <Arduino.h>
#include "LogHandler.h"
#include "PressureSensorManager.h"
#include "Settings.h"
#include "SolenoidManager.h"
#include "MainStateMachine.h"
#include "SerialManager.h"
#include "PressureSensor.h"
#include "Solenoid.h"
#include "MainCommunication.h"

Solenoid frontDownSolenoid(ESolenoid::FRONT_DOWN, 4);
Solenoid frontUpSolenoid(ESolenoid::FRONT_UP, 5);
Solenoid backUpSolenoid(ESolenoid::BACK_UP, 6);
Solenoid backDownSolenoid(ESolenoid::BACK_DOWN, 7);

SolenoidManager solenoidManager;

#define analogMin  1638.4
#define analogMax  14745.6
#define barMax  14.82
#define barTankMax 15.2
#define frontFilterSize 16
#define backFilterSize 10

PressureSensor frontPressureSensor(EPressureSensor::FRONT, A0, frontFilterSize, analogMin, analogMax, barMax);
PressureSensor backPressureSensor(EPressureSensor::BACK, A1, backFilterSize,analogMin,analogMax,barMax);
PressureSensor tankPressureSensor(EPressureSensor::TANK, A2, backFilterSize,analogMin,analogMax,barTankMax);

PressureSensorManager pressureSensorManager(frontUpSolenoid, backUpSolenoid);

Settings settings;

//TODO add ISERIAL and StringQUeue
Communication communication();

MainCommunication mainCommunication(communication, settings);

void setupDebugCallback();

unsigned long timePrevious = 0;
int timeInterval = 200;

MainStateMachine mainStateMachine;

void setup() {
  solenoidManager.AddSolenoid(frontDownSolenoid);
  solenoidManager.AddSolenoid(frontUpSolenoid);
  solenoidManager.AddSolenoid(backDownSolenoid);
  solenoidManager.AddSolenoid(backUpSolenoid);
  pressureSensorManager.AddPressureSensor(frontPressureSensor);
  pressureSensorManager.AddPressureSensor(backPressureSensor);
  pressureSensorManager.AddPressureSensor(tankPressureSensor);

  serialManager.Begin(true);

  solenoidManager.Begin();
  pressureSensorManager.Begin();
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
      mainStateMachine.RequestChangeState(message);
    });
}

