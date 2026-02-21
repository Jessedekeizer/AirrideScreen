#ifndef USBSERIAL_SETTINGS_H
#define USBSERIAL_SETTINGS_H

struct Settings {
    double frontMax = 5.00;
    double backMax = 4.60;
    double rideFront = 3.00;
    double rideBack = 2.50;
    double frontUpX = 1.00;
    double frontDownX = 1.66;
    double backUpX = 0.50;
    double backDownX = 1.66;
    double parkDuration = 4.0;
};

extern Settings settings;
#endif //USBSERIAL_SETTINGS_H
