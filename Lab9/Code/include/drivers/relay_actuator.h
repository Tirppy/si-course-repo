#ifndef DRIVERS_RELAY_ACTUATOR_H
#define DRIVERS_RELAY_ACTUATOR_H

#include "app/app_types.h"

void actuatorOutputInit();
void actuatorOutputApply(ActuatorState state);

#endif
