#ifndef DRIVERS_RELAY_ACTUATOR_H
#define DRIVERS_RELAY_ACTUATOR_H

#include "app/app_types.h"

void heaterOutputInit();
void heaterOutputApply(ActuatorState state);

#endif
