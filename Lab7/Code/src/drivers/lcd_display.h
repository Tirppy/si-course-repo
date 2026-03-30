#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Arduino.h>

void lcdDisplayInit();
void lcdDisplayClearConsole();
void lcdDisplayPutChar(char character);

#endif
