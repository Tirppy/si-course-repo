#include "sensor_pipeline.h"

#include <math.h>

#include "app_config.h"
#include "signal_conversion.h"
#include "signal_filters.h"
#include "threshold_alert.h"

void sensorPipelineReset(SensorRuntimeData &data, const char *name) {
  data.name = name;
  data.rawValue = 0.0F;
  data.saturatedValue = 0.0F;
  data.medianValue = 0.0F;
  data.weightedValue = 0.0F;
  data.temperatureC = 0.0F;
  data.previousTemperatureC = NAN;
  data.lastDeltaTemperatureC = 0.0F;
  data.minimumTemperatureC = NAN;
  data.maximumTemperatureC = NAN;
  data.trendDirection = 0;
  data.unchangedSampleCount = 0U;
  data.fastChangeDetected = false;
  data.alertCandidate = false;
  data.stableAlert = false;
  data.alertCounter = 0U;
  data.valid = false;
  data.sampleTimestampMs = 0U;
}

void sensorPipelineUpdateStatistics(SensorRuntimeData &data, float fastChangeThresholdC) {
  if (!data.valid || isnan(data.temperatureC)) {
    data.lastDeltaTemperatureC = 0.0F;
    data.trendDirection = 0;
    data.unchangedSampleCount = 0U;
    data.fastChangeDetected = false;
    return;
  }

  data.fastChangeDetected = false;

  if (isnan(data.minimumTemperatureC) || data.temperatureC < data.minimumTemperatureC) {
    data.minimumTemperatureC = data.temperatureC;
  }
  if (isnan(data.maximumTemperatureC) || data.temperatureC > data.maximumTemperatureC) {
    data.maximumTemperatureC = data.temperatureC;
  }

  if (!isnan(data.previousTemperatureC)) {
    const float delta = data.temperatureC - data.previousTemperatureC;
    data.lastDeltaTemperatureC = delta;
    data.fastChangeDetected = fabs(delta) >= fastChangeThresholdC;
    if (delta > TREND_DELTA_THRESHOLD_C) {
      data.trendDirection = 1;
      data.unchangedSampleCount = 0U;
    } else if (delta < -TREND_DELTA_THRESHOLD_C) {
      data.trendDirection = -1;
      data.unchangedSampleCount = 0U;
    } else {
      if (data.unchangedSampleCount < TREND_STABLE_SAMPLE_COUNT) {
        ++data.unchangedSampleCount;
      }
      if (data.unchangedSampleCount >= TREND_STABLE_SAMPLE_COUNT) {
        data.trendDirection = 0;
      }
    }
  } else {
    data.lastDeltaTemperatureC = 0.0F;
    data.unchangedSampleCount = 0U;
  }

  data.previousTemperatureC = data.temperatureC;
}

void sensorPipelineUpdateAnalog(SensorRuntimeData &data,
                                uint16_t rawAdc,
                                uint32_t nowMs,
                                AnalogProcessingState &processingState,
                                const ThresholdAlertConfig &alertConfig,
                                ThresholdAlertState &alertState) {
  data.rawValue = static_cast<float>(rawAdc);
  data.saturatedValue =
      saturateFloat(data.rawValue, static_cast<float>(ANALOG_SATURATION_MIN),
                    static_cast<float>(ANALOG_SATURATION_MAX));
  data.medianValue = medianFilterApply(processingState.median, data.saturatedValue);
  data.weightedValue = weightedAverageApply(processingState.weighted,
                                            data.medianValue,
                                            WEIGHTED_FILTER_WEIGHTS,
                                            WEIGHTED_WINDOW_SIZE);
  data.temperatureC = ntcAdcToCelsius(data.weightedValue);
  data.valid = isAnalogTemperatureValid(data.temperatureC);
  sensorPipelineUpdateStatistics(data, ANALOG_FAST_CHANGE_THRESHOLD_C);

  thresholdAlertUpdate(data.temperatureC, data.valid, alertConfig, alertState);
  data.alertCandidate = alertState.hysteresisState;
  data.stableAlert = alertState.stableState;
  data.alertCounter = alertState.counter;
  data.sampleTimestampMs = nowMs;
}

void sensorPipelineUpdateDigital(SensorRuntimeData &data,
                                 float rawTemperatureC,
                                 bool validSample,
                                 uint32_t nowMs,
                                 DigitalProcessingState &processingState,
                                 const ThresholdAlertConfig &alertConfig,
                                 ThresholdAlertState &alertState) {
  data.rawValue = rawTemperatureC;
  data.saturatedValue = saturateFloat(rawTemperatureC, DIGITAL_SATURATION_MIN_C, DIGITAL_SATURATION_MAX_C);
  data.medianValue = medianFilterApply(processingState.median, data.saturatedValue);
  data.weightedValue = weightedAverageApply(processingState.weighted,
                                            data.medianValue,
                                            WEIGHTED_FILTER_WEIGHTS,
                                            WEIGHTED_WINDOW_SIZE);
  data.temperatureC = data.weightedValue;
  data.valid = validSample && isDigitalTemperatureValid(data.temperatureC);
  sensorPipelineUpdateStatistics(data, DIGITAL_FAST_CHANGE_THRESHOLD_C);

  thresholdAlertUpdate(data.temperatureC, data.valid, alertConfig, alertState);
  data.alertCandidate = alertState.hysteresisState;
  data.stableAlert = alertState.stableState;
  data.alertCounter = alertState.counter;
  data.sampleTimestampMs = nowMs;
}
