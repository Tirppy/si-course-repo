#include "drivers/setpoint_input.h"

#include <Arduino.h>

#include "config/pins.h"

void setpointInputInit() { pinMode(kSetpointPotPin, INPUT); }

int setpointInputReadRaw() { return analogRead(kSetpointPotPin); }
