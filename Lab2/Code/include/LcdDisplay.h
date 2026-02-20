#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <LiquidCrystal.h>

class LcdDisplay {
public:
    LcdDisplay();

    void initialize();
    void printMessage(const char* line1, const char* line2);
    void clear();

private:
    LiquidCrystal lcd;
};

#endif
