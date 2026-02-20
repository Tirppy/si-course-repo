#include "LcdDisplay.h"

#include "Config.h"

LcdDisplay::LcdDisplay() : lcd(LcdRsPin, LcdEnPin, LcdD4Pin, LcdD5Pin, LcdD6Pin, LcdD7Pin) {}

void LcdDisplay::initialize() {
    lcd.begin(LcdColumns, LcdRows);
}

void LcdDisplay::printMessage(const char* line1, const char* line2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}

void LcdDisplay::clear() {
    lcd.clear();
}
