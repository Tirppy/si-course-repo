#include "sensor_pipeline.h"

#include <math.h>

#include "threshold_alert.h"

void sensorPipelineReset(SensorRuntimeData &data, const char *name, const char *rawUnit) {
  data.name = name;
  data.rawUnit = rawUnit;
  data.rawSignal = 0.0F;
  data.temperatureC = 0.0F;
  data.thresholdCandidate = false;
  data.stableAlert = false;
  data.debounceCounter = 0U;
  data.valid = false;
  data.sampleTimestampMs = 0U;
}

void sensorPipelineUpdate(SensorRuntimeData &data,
                          float rawSignal,
                          float temperatureC,
                          bool valid,
                          uint32_t nowMs,
                          const ThresholdAlertConfig &config,
                          ThresholdAlertState &alertState) {
  thresholdAlertUpdate(temperatureC, valid, config, alertState);

  data.rawSignal = rawSignal;
  data.temperatureC = valid ? temperatureC : NAN;
  data.thresholdCandidate = alertState.hysteresisState;
  data.stableAlert = alertState.stableState;
  data.debounceCounter = alertState.counter;
  data.valid = valid;
  data.sampleTimestampMs = nowMs;
}
