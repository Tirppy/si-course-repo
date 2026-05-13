#ifndef DRIVERS_SERIAL_STDIO_H
#define DRIVERS_SERIAL_STDIO_H

#include <Arduino.h>

#include "app/app_types.h"

void serialStdioInit(unsigned long baudRate);
void serialStdioPrintHelp();
void serialStdioPrintStatus(float measuredTemperatureC,
                            float setpointTemperatureC,
                            float hysteresisWidthC,
                            float hysteresisLowC,
                            float hysteresisHighC,
                            float acceptableMinTemperatureC,
                            float acceptableMaxTemperatureC,
                            bool temperatureWithinRange,
                            ControlMode mode,
                            bool heaterOn,
                            uint8_t fanDutyPercent,
                            float pidKp,
                            float pidKi,
                            float pidKd,
                            bool serialOverrideEnabled,
                            const char* sourceLabel);
void serialStdioPrintMessage(const char* message);

#endif
