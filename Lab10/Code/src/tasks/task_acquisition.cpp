#include "tasks/task_acquisition.h"

#include "app/app_state.h"
#include "drivers/thermistor_sensor.h"
#include "services/thermistor_math.h"

void taskAcquisitionRun() {
  const int adcValue = thermistorSensorReadRaw();
  const float voltageV = thermistorVoltageFromAdc(adcValue);
  const float resistanceOhms = thermistorResistanceFromAdc(adcValue);
  const float temperatureC = thermistorTemperatureCFromAdc(adcValue);
  const bool valid = thermistorReadingValid(adcValue, temperatureC);

  g_appState.sensor.adcValue = adcValue;
  g_appState.sensor.voltageV = voltageV;
  g_appState.sensor.resistanceOhms = resistanceOhms;
  g_appState.sensor.measuredTemperatureC = temperatureC;
  g_appState.sensor.valid = valid;
}
