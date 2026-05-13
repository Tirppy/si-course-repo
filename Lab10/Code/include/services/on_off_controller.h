#ifndef SERVICES_ON_OFF_CONTROLLER_H
#define SERVICES_ON_OFF_CONTROLLER_H

#include "app/app_types.h"

ActuatorState onOffControllerCompute(float measuredTemperatureC,
                                     float setpointTemperatureC,
                                     float hysteresisWidthC,
                                     ActuatorState previousState,
                                     float* lowThresholdC,
                                     float* highThresholdC);

#endif
