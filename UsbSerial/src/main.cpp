#include <arduino.h>
#include "LogHandler.h"

String incoming = "";

long tinv = 200;
long tprev = 0;
long tnow = 0;

long pinv = 4000;
long pprev = 0;

double frontMax = 5.00;
double backMax = 4.60;
double rideFront = 3.00;
double rideBack = 2.50;
double frontUpX = 1.00;
double frontDownX = 1.66;
double backUpX = 0.50;
double backDownX = 1.66;

double AnalogMin = 1638.4;
double AnalogMax = 14745.6;
double BarMax = 14.82;
double BarTankMax = 15.2;

bool rideF = false;
bool rideB = false;

long frontTimeInv;
long frontTimePrev;
long backTimeInv;
long backTimePrev;

long calculateTime(double, double, double);

enum State
{
  IDLE,
  FRONT_MAX,
  FRONT_MIN,
  FRONT_UP,
  FRONT_DOWN,
  BACK_MAX,
  BACK_MIN,
  BACK_UP,
  BACK_DOWN,
  RIDE,
  PARK
};

const int frontNumReadings = 16;
const int backNumReadings = 10;

double frontReadings[frontNumReadings];
int frontIndex = 0;
double frontTotal = 0;
double frontAverage = 0;

double backReadings[backNumReadings];
int backIndex = 0;
double backTotal = 0;
double backAverage = 0;

void stop();

double fmap(double, double, double, double, double);

State currentState = IDLE;

void resetSolenoid();
void setState(String);
void handleState();
void saveSettings(String);
String readPressure();
String getValue(String, char, int);

#define frontSensor A0
#define backSensor A1
#define tankSensor A2
#define frontUp 5
#define frontDown 4
#define backUp 6
#define backDown 7

void smoothAnalogRead();

LogHandler *logHandler = new LogHandler();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600, SERIAL_8N1);
  Serial1.begin(9600, SERIAL_8N1);
  Serial1.setTimeout(20);
  analogReadResolution(14);
  pinMode(frontUp, OUTPUT);
  pinMode(frontDown, OUTPUT);
  pinMode(backUp, OUTPUT);
  pinMode(backDown, OUTPUT);

  for (int i = 0; i < frontNumReadings; i++)
  {

    frontReadings[i] = fmap(analogRead(frontSensor), AnalogMin, AnalogMax, 0, BarMax);
    frontTotal += frontReadings[i];
  }

  for (int i = 0; i < backNumReadings; i++)
  {

    backReadings[i] = fmap(analogRead(backSensor), AnalogMin, AnalogMax, 0, BarMax);
    backTotal += backReadings[i];
  }
}

void loop()
{
  while (Serial1.available())
  {
    delay(2);
    char c = Serial1.read();
    incoming += c;
  }
  if (incoming != "")
  {
    Serial.println("incoming message:" + incoming);
  }

  if (incoming.startsWith("settings"))
  {
    saveSettings(incoming);
  }
  else
  {
    setState(incoming);
  }

  tnow = millis();
  if (tnow - tprev > tinv)
  {
    smoothAnalogRead();
    Serial1.println(readPressure());
    // Serial.println(fmap(analogRead(tankSensor), AnalogMin, AnalogMax, 0, BarTankMax));
    //  Serial.println(fmap(analogRead(tankSensor), AnalogMin, AnalogMax, 0, BarTankMax));
    //   Serial.println(fmap(analogRead(backSensor), AnalogMin, AnalogMax, 0, BarMax));
    tprev = millis();
  }
  stop();
  handleState();

  incoming = "";
}

String readPressure()
{
  String message = "BAR/";
  double front = fmap(analogRead(frontSensor), AnalogMin, AnalogMax, 0, BarMax);
  double rear = fmap(analogRead(backSensor), AnalogMin, AnalogMax, 0, BarMax);

  return message + String(front) + "/" + String(rear) + "/";
}

void stop()
{
  // double front = fmap(analogRead(frontSensor), AnalogMin, AnalogMax, 0, BarMax);
  // double back = fmap(analogRead(backSensor), AnalogMin, AnalogMax, 0, BarMax);
  if (frontAverage >= frontMax)
  {
    digitalWrite(frontUp, LOW);
    digitalWrite(frontDown, LOW);
  }
  if (backAverage >= backMax)
  {
    digitalWrite(backUp, LOW);
    digitalWrite(backDown, LOW);
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

void saveSettings(String incoming)
{
  frontMax = getValue(incoming, '/', 1).toDouble();
  backMax = getValue(incoming, '/', 2).toDouble();
  rideFront = getValue(incoming, '/', 3).toDouble();
  rideBack = getValue(incoming, '/', 4).toDouble();
  frontUpX = getValue(incoming, '/', 5).toDouble();
  frontDownX = getValue(incoming, '/', 6).toDouble();
  backUpX = getValue(incoming, '/', 7).toDouble();
  backDownX = getValue(incoming, '/', 8).toDouble();
}

void resetSolenoid()
{
  digitalWrite(frontUp, LOW);
  digitalWrite(frontDown, LOW);
  digitalWrite(backUp, LOW);
  digitalWrite(backDown, LOW);
}

void setState(String message)
{
  /*for (handler of handlers)
  {
    if (handler.supports(message))
    {
      handler.handle(message);
      return;
    }
  }*/

  if (message.equals("Front Up On"))
  {
    resetSolenoid();
    logHandler->startFrontLog();
    digitalWrite(frontUp, HIGH);
    currentState = FRONT_UP;
  }
  else if (message.equals("Front Up Off"))
  {
    digitalWrite(frontUp, LOW);
    logHandler->endFrontLog();
    currentState = IDLE;
  }
  else if (message.equals("Front Down On"))
  {
    resetSolenoid();
    logHandler->startFrontLog();
    digitalWrite(frontDown, HIGH);
    currentState = FRONT_DOWN;
  }
  else if (message.equals("Front Down Off"))
  {
    digitalWrite(frontDown, LOW);
    logHandler->endFrontLog();
    currentState = IDLE;
  }

  else if (message.equals("Back Up On"))
  {
    resetSolenoid();
    logHandler->startBackLog();
    digitalWrite(backUp, HIGH);
    currentState = BACK_UP;
  }
  else if (message.equals("Back Up Off"))
  {
    digitalWrite(backUp, LOW);
    logHandler->endBackLog();
    currentState = IDLE;
  }
  else if (message.equals("Back Down On"))
  {
    resetSolenoid();
    logHandler->startBackLog();
    digitalWrite(backDown, HIGH);
    currentState = BACK_DOWN;
  }
  else if (message.equals("Back Down Off"))
  {
    digitalWrite(backDown, LOW);
    logHandler->endBackLog();
    currentState = IDLE;
  }
  else if (message.equals("Ride"))
  {
    resetSolenoid();
    rideF = false;
    rideB = false;

    double currentFront = fmap(analogRead(frontSensor), AnalogMin, AnalogMax, 0, BarMax);
    double currentBack = fmap(analogRead(backSensor), AnalogMin, AnalogMax, 0, BarMax);

    if (currentFront > rideFront)
    {
      frontTimePrev = millis();
      frontTimeInv = calculateTime(currentFront, rideFront, frontDownX);

      digitalWrite(frontUp, LOW);
      digitalWrite(frontDown, HIGH);
    }
    else if (currentFront < rideFront)
    {
      frontTimePrev = millis();
      frontTimeInv = calculateTime(currentFront, rideFront, frontUpX);
      digitalWrite(frontDown, LOW);
      digitalWrite(frontUp, HIGH);
    }
    else
    {
      rideF = true;
    }

    if (currentBack > rideBack)
    {
      backTimePrev = millis();
      backTimeInv = calculateTime(currentBack, rideBack, backDownX);

      digitalWrite(backUp, LOW);
      digitalWrite(backDown, HIGH);
    }
    else if (currentBack < rideBack)
    {
      backTimePrev = millis();
      backTimeInv = calculateTime(currentBack, rideBack, backUpX);
      digitalWrite(backDown, LOW);
      digitalWrite(backUp, HIGH);
    }
    else
    {
      rideB = true;
    }

    if (!rideF && !rideB)
    {
      logHandler->startFrontLog(true);
      logHandler->startBackLog(true);
    }
    else
    {
      logHandler->startFrontLog();
      logHandler->startBackLog();
    }

    currentState = RIDE;
  }
  else if (message.equals("Park"))
  {
    resetSolenoid();
    pprev = millis();
    logHandler->startFrontLog(false);
    logHandler->startBackLog(false);
    digitalWrite(frontDown, HIGH);
    digitalWrite(backDown, HIGH);
    currentState = PARK;
  }
}

void handleState()
{
  switch (currentState)
  {
  case FRONT_UP:
    if (frontAverage > frontMax)
    {
      // Pressure exceeded, close the solenoid and return to IDLE
      digitalWrite(frontUp, LOW);
      logHandler->endFrontLog();
      currentState = IDLE;
    }
    break;

  case FRONT_DOWN:
    if (frontAverage < 0.5)
    {
      // Pressure exceeded, close the solenoid and return to IDLE
      digitalWrite(frontDown, LOW);
      logHandler->endFrontLog();
      currentState = IDLE;
    }
    break;

  case BACK_UP:
    if (backAverage > backMax)
    {
      // Pressure exceeded, close the solenoid and return to IDLE
      digitalWrite(backUp, LOW);
      logHandler->endBackLog();
      currentState = IDLE;
    }
    break;

  case BACK_DOWN:
    if (backAverage < 0.5)
    {
      // Pressure exceeded, close the solenoid and return to IDLE
      digitalWrite(backDown, LOW);
      logHandler->endBackLog();
      currentState = IDLE;
    }
    break;

  case RIDE:
    tnow = millis();
    if ((tnow - frontTimePrev > frontTimeInv) && !rideF)
    {
      digitalWrite(frontUp, LOW);
      digitalWrite(frontDown, LOW);
      logHandler->endFrontLog();
      rideF = true;
    }

    if ((tnow - backTimePrev > backTimeInv) && !rideB)
    {
      digitalWrite(backUp, LOW);
      digitalWrite(backDown, LOW);
      logHandler->endBackLog();
      rideB = true;
    }

    if (rideF && rideB)
    {
      resetSolenoid();
      currentState = IDLE;
    }
    break;

  case PARK:
    tnow = millis();
    if (tnow - pprev > pinv)
    {
      resetSolenoid();
      logHandler->endFrontLog();
      logHandler->endBackLog();
      currentState = IDLE;
    }
    break;
  }
}

long calculateTime(double current, double desired, double x)
{
  return ((x * desired) - (x * current)) * 1000;
}

void smoothAnalogRead()
{
  frontTotal = frontTotal - frontReadings[frontIndex];
  frontReadings[frontIndex] = fmap(analogRead(frontSensor), AnalogMin, AnalogMax, 0, BarMax);
  frontTotal = frontTotal + frontReadings[frontIndex];
  frontIndex = (frontIndex + 1) % frontNumReadings;
  frontAverage = frontTotal / frontNumReadings;

  backTotal = backTotal - backReadings[backIndex];
  backReadings[backIndex] = fmap(analogRead(backSensor), AnalogMin, AnalogMax, 0, BarMax);
  backTotal = backTotal + backReadings[backIndex];
  backIndex = (backIndex + 1) % backNumReadings;
  backAverage = backTotal / backNumReadings;
}

double fmap(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
