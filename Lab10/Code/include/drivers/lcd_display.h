#ifndef DRIVERS_LCD_DISPLAY_H
#define DRIVERS_LCD_DISPLAY_H

#include <Arduino.h>

#include "app/app_types.h"

void lcdDisplayInit();
void lcdDisplayShowStatus(float measuredTemperatureC,
                          float setpointTemperatureC,
                          ControlMode mode,
                          bool heaterOn,
                          uint8_t fanDutyPercent,
                          bool serialOverrideEnabled);

#endif
