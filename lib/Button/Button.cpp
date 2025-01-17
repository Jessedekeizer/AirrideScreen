#include "Button.h"

bool Button::CheckTouch(int touchX, int touchY)
{
  if (touchX >= x && touchX <= x + width && touchY >= y && touchY <= y + height)
  {
    return true;
  }
  return false;
}

void Button::OnPress(bool press)
{
  if (callback)
  {
    if (type == ButtonType::push && press)
    {
      callback(*this);
    }
    else if (type == ButtonType::Toggle)
    {
      if (press != toggle)
      {
        toggle = press;
        callback(*this);
      }
    }
  }
}
