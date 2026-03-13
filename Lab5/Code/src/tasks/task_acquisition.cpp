#include "task_acquisition.h"

#include <Arduino.h>
#include <math.h>

#include "alert_buzzer.h"
#include "alert_led.h"
#include "analog_ntc_sensor.h"
#include "app_config.h"
#include "app_state.h"
#include "digital_dht11_sensor.h"
#include "sensor_pipeline.h"
#include "signal_conversion.h"
#include "threshold_potentiometer.h"

namespace {
ThresholdAlertConfig g_alertConfig = {
    ALERT_THRESHOLD_DEFAULT_C,
    ALERT_HYSTERESIS_C,
    ALERT_CONFIRM_SAMPLES,
};

ThresholdAlertState g_analogAlertState = {};
ThresholdAlertState g_digitalAlertState = {};
}  // namespace

void taskAcquisitionInit() {
  sensorPipelineReset(g_appState.analog, "Thermistor", "adc");
  sensorPipelineReset(g_appState.digital, "DHT11 Temp", "C");
  g_appState.activeThresholdC = ALERT_THRESHOLD_DEFAULT_C;
  g_appState.thresholdPotRaw = thresholdPotentiometerReadRaw();
  g_appState.digitalHumidityPercent = NAN;
  g_appState.temperatureDifferenceC = NAN;
  g_appState.differenceAlert = false;
}

void taskAcquisitionRun() {
  const uint32_t nowMs = millis();
  const uint16_t potRaw = thresholdPotentiometerReadRaw();
  g_appState.thresholdPotRaw = potRaw;
  g_appState.activeThresholdC = mapPotentiometerToThresholdC(potRaw);
  g_alertConfig.thresholdC = g_appState.activeThresholdC;

  const AnalogTemperatureSample analogSample = analogNtcSensorRead();
  sensorPipelineUpdate(g_appState.analog,
                       static_cast<float>(analogSample.rawAdc),
                       analogSample.temperatureC,
                       analogSample.valid,
                       nowMs,
                       g_alertConfig,
                       g_analogAlertState);

  const DigitalTemperatureSample digitalSample = digitalDht11SensorRead();
  g_appState.digitalHumidityPercent = digitalSample.humidityPercent;
  sensorPipelineUpdate(g_appState.digital,
                       digitalSample.rawTemperatureC,
                       digitalSample.rawTemperatureC,
                       digitalSample.valid,
                       nowMs,
                       g_alertConfig,
                       g_digitalAlertState);

  g_appState.temperatureDifferenceC = NAN;
  g_appState.differenceAlert = false;
  if (g_appState.analog.valid && g_appState.digital.valid) {
    g_appState.temperatureDifferenceC =
        fabsf(g_appState.analog.temperatureC - g_appState.digital.temperatureC);
    g_appState.differenceAlert = g_appState.temperatureDifferenceC >= DIFFERENCE_ALERT_THRESHOLD_C;
  }

  alertLedSetAnalog(g_appState.analog.stableAlert);
  alertLedSetDigital(g_appState.digital.stableAlert);
  alertBuzzerSet(g_appState.differenceAlert);
  ++g_appState.acquisitionCycles;
}
