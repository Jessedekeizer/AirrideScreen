#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
public:
  Button(int x, int y, int width, int height, const char* name);
  bool CheckTouch(int touchX, int touchY);
  String GetName(){return name;};

private:
  int x;
  int y;
  int width;
  int height;
  const char* name;
  bool isPressed;
};

#endif