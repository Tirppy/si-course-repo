#include "drivers/lcd_display.h"

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdio.h>
#include <stdlib.h>

#include "config/pins.h"

namespace {

LiquidCrystal g_lcd(kLcdRsPin, kLcdEnablePin, kLcdD4Pin, kLcdD5Pin, kLcdD6Pin,
                    kLcdD7Pin);

void writePaddedLine(uint8_t row, const char* text) {
  char buffer[17];
  snprintf(buffer, sizeof(buffer), "%-16.16s", text);
  g_lcd.setCursor(0, row);
  g_lcd.print(buffer);
}

}  // namespace

void lcdDisplayInit() {
  g_lcd.begin(16, 2);
  g_lcd.clear();
  g_lcd.home();
  writePaddedLine(0, "Thermistor ctrl");
  writePaddedLine(1, "Booting...");
}

void lcdDisplayShowStatus(float measuredTemperatureC,
                          float setpointTemperatureC,
                          bool relayOn,
                          bool serialOverrideEnabled) {
  char line1[17];
  char line2[17];
  const int measuredX10 = static_cast<int>(measuredTemperatureC * 10.0F + 0.5F);
  const int setpointX10 = static_cast<int>(setpointTemperatureC * 10.0F + 0.5F);

  snprintf(line1, sizeof(line1), "T:%2d.%1d S:%2d.%1d", measuredX10 / 10,
           abs(measuredX10 % 10), setpointX10 / 10, abs(setpointX10 % 10));
  snprintf(line2, sizeof(line2), "%s %s", relayOn ? "OUT:ON " : "OUT:OFF",
           serialOverrideEnabled ? "SER" : "POT");

  writePaddedLine(0, line1);
  writePaddedLine(1, line2);
}
