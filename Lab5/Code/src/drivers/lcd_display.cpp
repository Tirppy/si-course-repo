#include "lcd_display.h"

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdio.h>
#include <string.h>

#include "app_config.h"

namespace {
LiquidCrystal g_lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN,
                    LCD_D7_PIN);

void writePaddedLine(uint8_t row, const char *text) {
  char buffer[LCD_COLUMNS + 1] = {};
  snprintf(buffer, sizeof(buffer), "%-16.16s", text);
  g_lcd.setCursor(0, row);
  g_lcd.print(buffer);
}

const char *shortAlertLabel(bool active) {
  return active ? "!" : "-";
}
}  // namespace

void lcdDisplayInit() {
  g_lcd.begin(LCD_COLUMNS, LCD_ROWS);
  writePaddedLine(0, "Temp display");
  writePaddedLine(1, "Initializing...");
}

void lcdDisplayRender(const SensorRuntimeData &analogData, const SensorRuntimeData &digitalData) {
  char line0[LCD_COLUMNS + 1] = {};
  char line1[LCD_COLUMNS + 1] = {};

  if (analogData.valid) {
    snprintf(line0,
             sizeof(line0),
             "Therm:%5.1fC %s",
             static_cast<double>(analogData.temperatureC),
             shortAlertLabel(analogData.stableAlert));
  } else {
    snprintf(line0, sizeof(line0), "Therm: wait %s", shortAlertLabel(false));
  }

  if (digitalData.valid) {
    snprintf(line1,
             sizeof(line1),
             "DHT11:%5.1fC %s",
             static_cast<double>(digitalData.temperatureC),
             shortAlertLabel(digitalData.stableAlert));
  } else {
    snprintf(line1, sizeof(line1), "DHT11: wait %s", shortAlertLabel(false));
  }

  writePaddedLine(0, line0);
  writePaddedLine(1, line1);
}
