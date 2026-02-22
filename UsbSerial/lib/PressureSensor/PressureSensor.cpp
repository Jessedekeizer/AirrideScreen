#include "PressureSensor.h"
#include <Arduino.h>

void PressureSensor::Begin() {
    buffer.clear();
    buffer.reserve(filterSize);
    bufferTotal = 0;
    for (int i = 0; i < filterSize; i++) {
        buffer.push_back(ReadPressure());
        bufferTotal += buffer[i];
    }
}

void PressureSensor::UpdateBuffer() {
    bufferTotal = bufferTotal - buffer[bufferIndex];
    buffer[bufferIndex] = ReadPressure();
    bufferTotal = bufferTotal + buffer[bufferIndex];
    bufferIndex = (bufferIndex + 1) % filterSize;
    bufferAverage = bufferTotal / filterSize;
}

PressureSensor::~PressureSensor() {
    buffer.clear();
}

double PressureSensor::GetRawPressure() {
    return ReadPressure();
}

double PressureSensor::ReadPressure() {
    return DMap(analogRead(pin), analogMin, analogMax, 0, barMax);
}

double DMap(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
