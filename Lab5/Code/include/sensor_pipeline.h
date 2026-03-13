#ifndef SENSOR_PIPELINE_H
#define SENSOR_PIPELINE_H

#include <stdint.h>

#include "models.h"

void sensorPipelineReset(SensorRuntimeData &data, const char *name, const char *rawUnit);
void sensorPipelineUpdate(SensorRuntimeData &data,
                          float rawSignal,
                          float temperatureC,
                          bool valid,
                          uint32_t nowMs,
                          const ThresholdAlertConfig &config,
                          ThresholdAlertState &alertState);

#endif
