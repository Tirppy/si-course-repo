#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Arduino.h>

constexpr uint8_t LCD_RS_PIN = 7U;
constexpr uint8_t LCD_ENABLE_PIN = 6U;
constexpr uint8_t LCD_D4_PIN = 5U;
constexpr uint8_t LCD_D5_PIN = 4U;
constexpr uint8_t LCD_D6_PIN = 3U;
constexpr uint8_t LCD_D7_PIN = 13U;

void lcdDisplayInit();
void lcdDisplayShowLines(const char *line1, const char *line2);

#endif
