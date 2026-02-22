#ifndef USBSERIAL_ESTATE_H
#define USBSERIAL_ESTATE_H

enum class EState {
    IDLE,
    FRONT_UP,
    FRONT_DOWN,
    BACK_UP,
    BACK_DOWN,
    RIDE,
    PARK
};

#endif //USBSERIAL_ESTATE_H
