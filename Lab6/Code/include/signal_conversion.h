#ifndef SIGNAL_CONVERSION_H
#define SIGNAL_CONVERSION_H

float ntcAdcToCelsius(float rawAdc);
bool isAnalogTemperatureValid(float temperatureC);
bool isDigitalTemperatureValid(float temperatureC);

#endif
