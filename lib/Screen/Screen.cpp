#include <Screen.h>

void Screen::addButtons(const std::vector<Box>& boxesToAdd) {
    buttons.insert(buttons.end(), boxesToAdd.begin(), boxesToAdd.end());


    
    };


String Screen::handleTouch(int touchX, int touchY) {
    String result = "";
    for (Box& box : buttons) {
        result = box.checkTouch(touchX, touchY);
        if(result != ""){
            return result;
        }
        }
        return "";
    };

void Screen::releaseButton() {
    for (Box& box : buttons) {
        box.releaseButton();
        }
    };