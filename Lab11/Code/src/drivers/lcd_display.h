#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

void lcdDisplayInit();
void lcdDisplayShowStatus(const char *fsmStateName,
                          bool ledOn,
                          bool buttonPressed,
                          unsigned long toggleCount);

#endif
