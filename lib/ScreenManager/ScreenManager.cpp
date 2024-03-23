#include <ScreenManager.h>

void ScreenManager::addScreen(Screen* screen){
    screens.insert(screens.end(), screen);
}




void ScreenManager::changeActiveScreen(String screenName){
    if(screenName != ""){
        for(Screen* screen : screens)
	{
		if(screen->getName() == screenName){
            activeScreen->activate(false);
            activeScreen = screen;
            screen->activate(true);
            change = true;
        }
	} 
    }
}