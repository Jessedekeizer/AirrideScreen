#include "Handler.h"
class FrontUpHandler : Handler {

    
    public:
    bool supports(String message) override {
        return message.startsWith("Front Up On");
    } ;

    void handle(String message) override {
        // Handle the message
    } ;
};