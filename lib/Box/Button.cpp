#include "Button.h"

Button::Button(int x, int y, int width, int height, const char *name)
    : x(x), y(y), width(width), height(height), name(name){}

bool Button::CheckTouch(int touchX, int touchY)
{
  if (touchX >= x && touchX <= x + width && touchY >= y && touchY <= y + height)
  {
    isPressed = true;
    return true;
  }
  else
  {
    isPressed = false;
    return false;
  }
 
}


