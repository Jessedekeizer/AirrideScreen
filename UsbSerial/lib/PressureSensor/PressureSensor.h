#ifndef USBSERIAL_PRESSURESENSOR_H
#define USBSERIAL_PRESSURESENSOR_H
#include <vector>

#include "EPressureSensor.h"


class PressureSensor {
public:
    PressureSensor(EPressureSensor pressureSensor, int pin, int filterSize, double analogMin, double analogMax,
                   double barMax)
        : pressureSensor(pressureSensor), pin(pin), filterSize(filterSize), analogMin(analogMin), analogMax(analogMax),
          barMax(barMax) {
    }

    ~PressureSensor();

    EPressureSensor GetPressureSensor() { return pressureSensor; }

    double GetAveragePressure() { return bufferAverage; }

    double GetRawPressure();

    void UpdateBuffer();

    void Begin();

private:
    const int pin;
    const EPressureSensor pressureSensor;
    const int filterSize;

    std::vector<double> buffer;
    int bufferIndex = 0;
    double bufferTotal = 0;
    double bufferAverage = 0;
    double analogMin;
    double analogMax;
    double barMax;

    double ReadPressure();
};

double DMap(double x, double in_min, double in_max, double out_min, double out_max);

#endif //USBSERIAL_PRESSURESENSOR_H
