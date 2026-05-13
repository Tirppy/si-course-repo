#ifndef SERVICES_THERMISTOR_MATH_H
#define SERVICES_THERMISTOR_MATH_H

float thermistorVoltageFromAdc(int adcValue);
float thermistorResistanceFromAdc(int adcValue);
float thermistorTemperatureCFromAdc(int adcValue);
bool thermistorReadingValid(int adcValue, float temperatureC);

#endif
