#include "drivers/lcd_display.h"

#include <LiquidCrystal_I2C.h>
#include <ctype.h>
#include <string.h>

namespace {

LiquidCrystal_I2C g_lcd(0x27, 16, 2);
char g_lines[2][17];
uint8_t g_row = 0U;
uint8_t g_col = 0U;

void redrawDisplay() {
  g_lcd.clear();
  g_lcd.setCursor(0, 0);
  g_lcd.print(g_lines[0]);
  g_lcd.setCursor(0, 1);
  g_lcd.print(g_lines[1]);
}

void resetLine(char *line) {
  for (uint8_t index = 0U; index < 16U; ++index) {
    line[index] = ' ';
  }
  line[16] = '\0';
}

void advanceLine() {
  if (g_row == 0U) {
    g_row = 1U;
    g_col = 0U;
    resetLine(g_lines[1]);
    return;
  }

  memcpy(g_lines[0], g_lines[1], sizeof(g_lines[0]));
  resetLine(g_lines[1]);
  g_row = 1U;
  g_col = 0U;
}

}  // namespace

void lcdDisplayInit() {
  g_lcd.init();
  g_lcd.backlight();
  lcdDisplayClearConsole();
}

void lcdDisplayClearConsole() {
  resetLine(g_lines[0]);
  resetLine(g_lines[1]);
  g_row = 0U;
  g_col = 0U;
  redrawDisplay();
}

void lcdDisplayPutChar(char character) {
  if (character == '\f') {
    lcdDisplayClearConsole();
    return;
  }

  if (character == '\r') {
    return;
  }

  if (character == '\n') {
    advanceLine();
    redrawDisplay();
    return;
  }

  if (character == '\b') {
    if (g_col > 0U) {
      --g_col;
      g_lines[g_row][g_col] = ' ';
      redrawDisplay();
    }
    return;
  }

  if (!isprint(static_cast<unsigned char>(character))) {
    return;
  }

  g_lines[g_row][g_col] = character;
  ++g_col;

  if (g_col >= 16U) {
    advanceLine();
  }

  redrawDisplay();
}
