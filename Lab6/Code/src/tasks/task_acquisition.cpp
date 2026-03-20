#include "task_acquisition.h"

#include <Arduino.h>

#include "alert_led.h"
#include "analog_thermistor.h"
#include "app_config.h"
#include "app_state.h"
#include "digital_dht22_sensor.h"
#include "period_control.h"
#include "sensor_pipeline.h"
#include "signal_filters.h"

namespace {
ThresholdAlertConfig g_alertConfig = {
    ALERT_THRESHOLD_C,
    ALERT_HYSTERESIS_C,
    ALERT_CONFIRM_SAMPLES,
};

ThresholdAlertState g_analogAlertState = {};
ThresholdAlertState g_digitalAlertState = {};

AnalogProcessingState g_analogProcessingState = {};
DigitalProcessingState g_digitalProcessingState = {};

uint16_t applyInjectedNoise(uint16_t rawAdc, bool noiseEnabled) {
  if (!noiseEnabled) {
    return rawAdc;
  }

  int16_t noisyValue = static_cast<int16_t>(rawAdc) +
                       random(-ANALOG_WHITE_NOISE_AMPLITUDE, ANALOG_WHITE_NOISE_AMPLITUDE + 1);

  if (static_cast<uint8_t>(random(100)) < ANALOG_SPIKE_PROBABILITY_PERCENT) {
    const int16_t spike = random(2) == 0 ? -ANALOG_SPIKE_NOISE_AMPLITUDE : ANALOG_SPIKE_NOISE_AMPLITUDE;
    noisyValue += spike;
  }

  if (noisyValue < 0) {
    noisyValue = 0;
  }
  if (noisyValue > static_cast<int16_t>(ADC_MAX_VALUE)) {
    noisyValue = static_cast<int16_t>(ADC_MAX_VALUE);
  }

  return static_cast<uint16_t>(noisyValue);
}

float applyInjectedDigitalNoise(float temperatureC, bool noiseEnabled) {
  if (!noiseEnabled) {
    return temperatureC;
  }

  const long noiseMilliC = random(static_cast<long>(-DIGITAL_WHITE_NOISE_AMPLITUDE_C * 1000.0F),
                                  static_cast<long>(DIGITAL_WHITE_NOISE_AMPLITUDE_C * 1000.0F) + 1L);
  return temperatureC + (static_cast<float>(noiseMilliC) / 1000.0F);
}
}  // namespace

void taskAcquisitionInit() {
  pinMode(NOISE_BUTTON_PIN, INPUT_PULLUP);
  randomSeed(analogRead(ANALOG_SENSOR_PIN) + analogRead(PERIOD_CONTROL_PIN));

  sensorPipelineReset(g_appState.analog, "Thermistor");
  sensorPipelineReset(g_appState.digital, "DHT22");
  g_appState.periodControlRaw = periodControlReadRaw();
  g_appState.noiseInjectionEnabled = false;
  g_appState.acquisitionPeriodMs = ACQUISITION_PERIOD_DEFAULT_MS;
  g_appState.reportPeriodMs = REPORT_PERIOD_DEFAULT_MS;
  g_appState.fastChangeFaultActive = false;
  g_appState.fastChangeFaultUntilMs = 0U;

  medianFilterReset(g_analogProcessingState.median);
  weightedAverageReset(g_analogProcessingState.weighted);
  medianFilterReset(g_digitalProcessingState.median);
  weightedAverageReset(g_digitalProcessingState.weighted);
}

void taskAcquisitionRun() {
  const uint32_t nowMs = millis();
  g_appState.periodControlRaw = periodControlReadRaw();
  g_appState.noiseInjectionEnabled = digitalRead(NOISE_BUTTON_PIN) == LOW;
  g_appState.acquisitionPeriodMs = mapPotentiometerToAcquisitionPeriodMs(g_appState.periodControlRaw);
  g_appState.reportPeriodMs = mapAcquisitionToReportPeriodMs(g_appState.acquisitionPeriodMs);

  AnalogTemperatureSample analogSample = analogThermistorRead();
  analogSample.rawAdc = applyInjectedNoise(analogSample.rawAdc, g_appState.noiseInjectionEnabled);
  sensorPipelineUpdateAnalog(g_appState.analog,
                             analogSample.rawAdc,
                             nowMs,
                             g_analogProcessingState,
                             g_alertConfig,
                             g_analogAlertState);

  DigitalTemperatureSample digitalSample = digitalDht22SensorRead();
  if (digitalSample.valid) {
    digitalSample.rawTemperatureC =
        applyInjectedDigitalNoise(digitalSample.rawTemperatureC, g_appState.noiseInjectionEnabled);
  }

  sensorPipelineUpdateDigital(g_appState.digital,
                              digitalSample.rawTemperatureC,
                              digitalSample.valid,
                              nowMs,
                              g_digitalProcessingState,
                              g_alertConfig,
                              g_digitalAlertState);

  if (g_appState.analog.fastChangeDetected || g_appState.digital.fastChangeDetected) {
    g_appState.fastChangeFaultActive = true;
    g_appState.fastChangeFaultUntilMs = nowMs + FAST_CHANGE_LED_HOLD_MS;
  } else if (g_appState.fastChangeFaultActive && nowMs >= g_appState.fastChangeFaultUntilMs) {
    g_appState.fastChangeFaultActive = false;
  }

  const bool anyTrendActive = (g_appState.analog.trendDirection != 0) || (g_appState.digital.trendDirection != 0);

  alertLedSetTrend(anyTrendActive);
  alertLedSetAnalog(g_appState.analog.stableAlert);
  alertLedSetDigital(g_appState.digital.stableAlert);

  ++g_appState.acquisitionCycles;
}
