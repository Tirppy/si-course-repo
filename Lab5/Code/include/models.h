#ifndef MODELS_H
#define MODELS_H

#include <stdint.h>

struct AnalogTemperatureSample {
  uint16_t rawAdc;
  float temperatureC;
  bool valid;
};

struct DigitalTemperatureSample {
  float rawTemperatureC;
  float humidityPercent;
  bool valid;
};

struct ThresholdAlertConfig {
  float thresholdC;
  float hysteresisC;
  uint8_t confirmSamples;
};

struct ThresholdAlertState {
  bool hysteresisState;
  bool stableState;
  uint8_t counter;
};

struct SensorRuntimeData {
  const char *name;
  const char *rawUnit;
  float rawSignal;
  float temperatureC;
  bool thresholdCandidate;
  bool stableAlert;
  uint8_t debounceCounter;
  bool valid;
  uint32_t sampleTimestampMs;
};

struct AppState {
  SensorRuntimeData analog;
  SensorRuntimeData digital;
  float activeThresholdC;
  uint16_t thresholdPotRaw;
  float digitalHumidityPercent;
  float temperatureDifferenceC;
  bool differenceAlert;
  uint32_t acquisitionCycles;
  uint32_t displayCycles;
  uint32_t reportCycles;
};

#endif
