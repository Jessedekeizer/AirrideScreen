#include "Button.h"

ButtonState Button::CheckTouch(int touchX, int touchY)
{
  if (touchX >= x && touchX <= x + width && touchY >= y && touchY <= y + height)
  {
    if (toggle)
    {
      if (toggleOn)
      {
        return ButtonState::Idle;
      }
      else
      {
        toggleOn = true;
        return ButtonState::ToggleOn;
      }
    }
    return ButtonState::Push;
  }
  else
  {
    if (toggle && toggleOn)
    {
      toggleOn = false;
      return ButtonState::ToggleOff;
    }
    return ButtonState::Idle;
  }
}
