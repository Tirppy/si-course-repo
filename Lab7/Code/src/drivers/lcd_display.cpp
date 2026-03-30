#include "drivers/lcd_display.h"

#include <LiquidCrystal_I2C.h>
#include <string.h>

namespace {

LiquidCrystal_I2C g_lcd(0x27, 16, 2);

void normalizeLine(const char *source, char *destination) {
  for (uint8_t index = 0U; index < 16U; ++index) {
    destination[index] = ' ';
  }

  if (source != nullptr) {
    for (uint8_t index = 0U; index < 16U && source[index] != '\0'; ++index) {
      destination[index] = source[index];
    }
  }

  destination[16] = '\0';
}

}  // namespace

void lcdDisplayInit() {
  g_lcd.init();
  g_lcd.backlight();
  lcdDisplayShowLines("Lab5 Variant C", "Type HELP");
}

void lcdDisplayShowLines(const char *line1, const char *line2) {
  char normalizedLine1[17];
  char normalizedLine2[17];

  normalizeLine(line1, normalizedLine1);
  normalizeLine(line2, normalizedLine2);

  g_lcd.setCursor(0, 0);
  g_lcd.print(normalizedLine1);
  g_lcd.setCursor(0, 1);
  g_lcd.print(normalizedLine2);
}
