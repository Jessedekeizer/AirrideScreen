#ifndef BUTTON_H
#define BUTTON_H
#pragma once

#include <Arduino.h>
#include "ButtonState.h"

class Button
{
public:
  Button(int x, int y, int width, int height, String name, bool toggle = false)
      : x(x), y(y), width(width), height(height), name(name), toggle(toggle){};
  ButtonState CheckTouch(int touchX, int touchY);
  String GetName() { return name; };
  bool GetToggleState() { return toggleOn; };

private:
  int x;
  int y;
  int width;
  int height;
  bool toggle;
  bool toggleOn;
  String name;
};

#endif