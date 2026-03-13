#ifndef SIGNAL_CONVERSION_H
#define SIGNAL_CONVERSION_H

float ntcAdcToCelsius(int rawAdc);
float mapPotentiometerToThresholdC(int rawAdc);
bool isAnalogTemperatureValid(float temperatureC);
bool isDigitalTemperatureValid(float temperatureC);

#endif
