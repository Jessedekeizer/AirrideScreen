#ifndef SETTINGSDEVICE_H
#define SETTINGSDEVICE_H
#pragma once

struct SettingsDevice
{
    double frontMax = 0.0;
    double backMax = 0.0;
    double rideFront = 0.0;
    double rideBack = 0.0;
    double frontUpX = 0.0;
    double frontDownX = 0.0;
    double backUpX = 0.0;
    double backDownX = 0.0;
    bool autoRide = false;
    double autoRideSec = 0.0;
    bool autoPark = false;
    double autoParkSec = 0.0;
    bool logging = false;
    bool machineLearning = false;
    bool bluetooth = false;
    double parkDuration = 0.0;
    int xmin = 0;
    int xmax = 320;
    int ymin = 0;
    int ymax = 240;
    bool calibrationSet = false;

    // Helper method to ensure values don't go below 0
    void adjustValue(double &value, double delta)
    {
        value += delta;
        if (value < 0.0)
            value = 0.0;
    }
};

#endif // SETTINGS_H