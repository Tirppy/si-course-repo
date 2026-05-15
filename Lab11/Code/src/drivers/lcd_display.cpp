#include "lcd_display.h"

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdio.h>

namespace {

constexpr uint8_t kLcdRsPin = 7U;
constexpr uint8_t kLcdEnablePin = 6U;
constexpr uint8_t kLcdD4Pin = 5U;
constexpr uint8_t kLcdD5Pin = 4U;
constexpr uint8_t kLcdD6Pin = 3U;
constexpr uint8_t kLcdD7Pin = 13U;

LiquidCrystal g_lcd(kLcdRsPin, kLcdEnablePin, kLcdD4Pin, kLcdD5Pin, kLcdD6Pin,
                    kLcdD7Pin);
FILE *g_lcdStream = nullptr;

int lcdPutChar(char ch, FILE *stream) {
  (void)stream;
  g_lcd.write(ch);
  return 0;
}

void moveToLine(uint8_t row) {
  g_lcd.setCursor(0, row);
}

}  // namespace

void lcdDisplayInit() {
  g_lcd.begin(16, 2);
  g_lcd.clear();
  g_lcdStream = fdevopen(lcdPutChar, nullptr);

  moveToLine(0);
  fprintf(g_lcdStream, "%-16.16s", "Lab11 FSM LED");
  moveToLine(1);
  fprintf(g_lcdStream, "%-16.16s", "Booting...");
  fflush(g_lcdStream);
}

void lcdDisplayShowStatus(const char *fsmStateName,
                          bool ledOn,
                          bool buttonPressed,
                          unsigned long toggleCount) {
  moveToLine(0);
  fprintf(g_lcdStream, "%-8.8s LED:%-2.2s ", fsmStateName, ledOn ? "ON" : "OF");
  moveToLine(1);
  fprintf(g_lcdStream, "B:%-3.3s T:%03lu     ", buttonPressed ? "PRS" : "REL",
          toggleCount % 1000UL);
  fflush(g_lcdStream);
}
