#ifndef DRIVERS_SERIAL_STDIO_H
#define DRIVERS_SERIAL_STDIO_H

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
                            bool relayOn,
                            bool serialOverrideEnabled,
                            const char* sourceLabel);
void serialStdioPrintMessage(const char* message);

#endif
