#ifndef SENSOR_PIPELINE_H
#define SENSOR_PIPELINE_H

#include <stdint.h>

#include "models.h"

void sensorPipelineReset(SensorRuntimeData &data, const char *name);
void sensorPipelineUpdateStatistics(SensorRuntimeData &data, float fastChangeThresholdC);
void sensorPipelineUpdateAnalog(SensorRuntimeData &data,
                                uint16_t rawAdc,
                                uint32_t nowMs,
                                AnalogProcessingState &processingState,
                                const ThresholdAlertConfig &alertConfig,
                                ThresholdAlertState &alertState);
void sensorPipelineUpdateDigital(SensorRuntimeData &data,
                                 float rawTemperatureC,
                                 bool validSample,
                                 uint32_t nowMs,
                                 DigitalProcessingState &processingState,
                                 const ThresholdAlertConfig &alertConfig,
                                 ThresholdAlertState &alertState);

#endif
