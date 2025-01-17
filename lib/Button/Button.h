#ifndef BUTTON_H
#define BUTTON_H
#pragma once

#include <Arduino.h>
#include "ButtonType.h"

class Button
{
public:
  Button(int x, int y, int width, int height, String name, ButtonType type, std::function<void(Button &)> callback = nullptr)
      : x(x), y(y), width(width), height(height), name(name), type(type), callback(callback) {};
  bool CheckTouch(int touchX, int touchY);
  String GetName() { return name; };
  void OnPress(bool);
  bool GetToggle() { return toggle; };

private:
  int x;
  int y;
  int width;
  int height;
  bool toggle = false;
  ButtonType type;
  std::function<void(Button &)> callback;
  String name;
};

#endif