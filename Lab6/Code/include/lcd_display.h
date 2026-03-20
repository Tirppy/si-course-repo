#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include "models.h"

void lcdDisplayInit();
void lcdDisplayRender(const SensorRuntimeData &analogData, const SensorRuntimeData &digitalData);

#endif
