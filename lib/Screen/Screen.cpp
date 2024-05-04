#include <Screen.h>

void Screen::addButtons(const std::vector<Button>& boxesToAdd) {
    buttons.insert(buttons.end(), boxesToAdd.begin(), boxesToAdd.end());


    
    };


String Screen::handleTouch(int touchX, int touchY) {
    String result = "";
    for (Button& box : buttons) {
        result = box.checkTouch(touchX, touchY);
        if(result != ""){
            return result;
        }
        }
        return "";
    };

void Screen::releaseButton() {
    for (Button& box : buttons) {
        box.releaseButton();
        }
    };