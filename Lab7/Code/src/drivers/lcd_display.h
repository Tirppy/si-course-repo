#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Arduino.h>

void lcdDisplayInit();
void lcdDisplayShowLines(const char *line1, const char *line2);

#endif
