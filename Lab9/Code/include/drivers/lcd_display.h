#ifndef DRIVERS_LCD_DISPLAY_H
#define DRIVERS_LCD_DISPLAY_H

void lcdDisplayInit();
void lcdDisplayShowStatus(float measuredTemperatureC,
                          float setpointTemperatureC,
                          bool relayOn,
                          bool serialOverrideEnabled);

#endif
