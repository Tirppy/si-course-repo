#ifndef MODELS_H
#define MODELS_H

#include <stdint.h>

struct AnalogTemperatureSample {
  uint16_t rawAdc;
};

struct DigitalTemperatureSample {
  float rawTemperatureC;
  bool valid;
};

struct MedianFilterState {
  float buffer[5];
  uint8_t count;
  uint8_t nextIndex;
};

struct WeightedAverageState {
  float buffer[4];
  uint8_t count;
  uint8_t nextIndex;
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
  float rawValue;
  float saturatedValue;
  float medianValue;
  float weightedValue;
  float temperatureC;
  float previousTemperatureC;
  float lastDeltaTemperatureC;
  float minimumTemperatureC;
  float maximumTemperatureC;
  int8_t trendDirection;
  uint8_t unchangedSampleCount;
  bool fastChangeDetected;
  bool alertCandidate;
  bool stableAlert;
  uint8_t alertCounter;
  bool valid;
  uint32_t sampleTimestampMs;
};

struct AnalogProcessingState {
  MedianFilterState median;
  WeightedAverageState weighted;
};

struct DigitalProcessingState {
  MedianFilterState median;
  WeightedAverageState weighted;
};

struct AppState {
  SensorRuntimeData analog;
  SensorRuntimeData digital;
  uint16_t periodControlRaw;
  bool noiseInjectionEnabled;
  uint32_t acquisitionPeriodMs;
  uint32_t reportPeriodMs;
  bool fastChangeFaultActive;
  uint32_t fastChangeFaultUntilMs;
  uint32_t acquisitionCycles;
  uint32_t displayCycles;
  uint32_t reportCycles;
};

#endif
