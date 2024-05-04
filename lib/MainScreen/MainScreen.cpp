#include <MainScreen.h>


MainScreen::MainScreen(){
    name= "mainScreen";
    path= "/MainScreen.png";

    buttons = std::vector<Button> {
  {0, 0, 70, 120, "Front Up"},
  {0, 120, 70, 120, "Front Down"},
  {263, 0, 70, 120, "Back Up"},
  {263, 120, 70, 120, "Back Down"},
  {132, 174, 48, 42, "Settings1"},
  {84, 174, 48, 42, "Ride"},
  {186, 174, 48, 42, "Park"}
};
}

void MainScreen::HandleTouch(int touchX, int touchY){
    for (Button& button : buttons) {

        if(button.CheckTouch(touchX, touchY)){
            

            }
        }
}

void MainScreen::ConnectButton(Button *button){
    String buttonName = button->GetName();
    if(buttonName == "Front Up" || buttonName == "Front Down" || buttonName == "Back Up" || buttonName == "Back Down"){
        AdjustHeight(button);
    }
}

void MainScreen::AdjustHeight(Button *button){
    //manager for sending over serial

}