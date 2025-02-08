#include "Handler.h"
class SettingsHandler : Handler {


    public:
    bool supports(String message) override {
        return message.startsWith("settings");
    } ;

    void handle(String message) override {
        // Handle the message
    } ;
};