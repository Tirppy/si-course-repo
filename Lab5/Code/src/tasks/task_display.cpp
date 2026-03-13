#include "task_display.h"

#include "app_state.h"
#include "lcd_display.h"

void taskDisplayRun() {
  lcdDisplayRender(g_appState.analog, g_appState.digital);
  ++g_appState.displayCycles;
}
