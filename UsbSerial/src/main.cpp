#include <Arduino.h>
#include "LogHandler.h"
#include "PressureSensorManager.h"
#include "Settings.h"
#include "SolenoidManager.h"
#include "MainStateMachine.h"
#include "PressureSensor.h"
#include "Solenoid.h"
#include "MainCommunication.h"
#include "SerialOverPins.h"

Settings settings;

Solenoid frontDownSolenoid(ESolenoid::FRONT_DOWN, PIN_D4);
Solenoid frontUpSolenoid(ESolenoid::FRONT_UP, PIN_D5);
Solenoid backUpSolenoid(ESolenoid::BACK_UP, PIN_D6);
Solenoid backDownSolenoid(ESolenoid::BACK_DOWN, PIN_D7);
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
PressureSensorManager pressureSensorManager(frontUpSolenoid, backUpSolenoid, settings);

StringQueue stringQueue;
SerialOverPins serial(Serial1, stringQueue);
Communication communication(serial, stringQueue);

LogHandlerCommunication logHandlerCommunication(communication);
LogHandler logHandler(logHandlerCommunication, frontPressureSensor, backPressureSensor, tankPressureSensor);

MainStateMachineData mainStateMachineData;
MainStateMachineCommunication mainStateMachineCommunication(communication, mainStateMachineData);
MainStateMachine mainStateMachine(mainStateMachineData, mainStateMachineCommunication, solenoidManager,
                                  pressureSensorManager, logHandler, settings);

MainCommunication mainCommunication(communication, settings);

unsigned long timePrevious = 0;
int timeInterval = 200;

void setup() {
  analogReadResolution(14);
  Serial1.begin(9600, SERIAL_8N1);
  solenoidManager.AddSolenoid(frontDownSolenoid);
  solenoidManager.AddSolenoid(frontUpSolenoid);
  solenoidManager.AddSolenoid(backDownSolenoid);
  solenoidManager.AddSolenoid(backUpSolenoid);
  pressureSensorManager.AddPressureSensor(frontPressureSensor);
  pressureSensorManager.AddPressureSensor(backPressureSensor);
  pressureSensorManager.AddPressureSensor(tankPressureSensor);

  solenoidManager.Begin();
  pressureSensorManager.Begin();
  mainStateMachine.Begin();
  mainCommunication.Init();
}

void loop() {
  if (millis() - timePrevious > timeInterval) {
    pressureSensorManager.Update();
    mainCommunication.SendPressure(frontPressureSensor.GetRawPressure(), backPressureSensor.GetRawPressure());
    timePrevious = millis();
  }
  communication.CheckForMessage();
  mainStateMachine.Loop();
  logHandler.SendLog();
}

