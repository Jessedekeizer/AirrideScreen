#include "PressureSensor.h"
#include <api/Common.h>

void PressureSensor::Begin() {
    for (int i = 0; i < filterSize; i++) {
        buffer.push_back(readPressure());
        bufferTotal += buffer[i];
    }
}

void PressureSensor::UpdateBuffer() {
    bufferTotal = bufferTotal - buffer[bufferIndex];
    buffer[bufferIndex] = readPressure();
    bufferTotal = bufferTotal + buffer[bufferIndex];
    bufferIndex = (bufferIndex + 1) % filterSize;
    bufferAverage = bufferTotal / filterSize;
}

double PressureSensor::GetRawPressure() {
    return readPressure();
}

double PressureSensor::readPressure() {
    return DMap(analogRead(pin), analogMin, analogMax, 0, barMax);
}

double DMap(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
