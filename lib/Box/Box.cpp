#include "Box.h"

Box::Box(int x, int y, int width, int height, const char *name, bool isButton, bool pageChanger)
    : x(x), y(y), width(width), height(height), name(name), isButton(isButton), pageChanger(pageChanger), isPressed(false) {}

String Box::checkTouch(int touchX, int touchY)
{
  if (touchX >= x && touchX <= x + width && touchY >= y && touchY <= y + height)
  {

    if (pageChanger)
    {
      return name;
    }
    else
    {
      // Handle button press
      if (isButton && !isPressed)
      {
        isPressed = true;
        
        Serial2.print(name + String(" On"));
        
        
      }
      else if (!isButton)
      {
        // Handle other box touches
        
        Serial2.print(name);
        
      }
    }
  }
  else
  {
    // If the touch is outside the box, mark it as not pressed
    
    isPressed = false;
  }
  return "";
}

bool Box::getButton()
{
  return isButton;
}

void Box::releaseButton()
{
  // Release the button and print OFF command
  if (isButton && isPressed)
  {
    Serial2.print(name + String(" Off"));
    isPressed = false;
  }
}
