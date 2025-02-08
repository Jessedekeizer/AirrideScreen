#include <Arduino.h>

class Handler {
    public:
    virtual bool supports(String message) = 0;

    public:
    virtual void handle(String message) = 0;
};