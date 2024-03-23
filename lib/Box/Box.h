#ifndef BOX_H
#define BOX_H

#include <Arduino.h>

class Box {
public:
  Box(int x, int y, int width, int height, const char* name, bool isButton, bool pageChanger);
  String checkTouch(int touchX, int touchY);
  void releaseButton();
  bool getButton();

private:
  int x;
  int y;
  int width;
  int height;
  const char* name;
  bool isButton;
  bool isPressed;
  bool pageChanger;
};

#endif