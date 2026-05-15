#include "app_controller.h"

#include <Arduino.h>
#include <stdio.h>

#include "drivers/button_driver.h"
#include "drivers/lcd_display.h"
#include "drivers/led_driver.h"
#include "drivers/serial_stdio.h"
#include "services/button_led_fsm.h"
#include "services/debounce_filter.h"

namespace {

constexpr unsigned long kButtonSamplePeriodMs = 5UL;
constexpr unsigned long kReportPeriodMs = 500UL;
constexpr uint8_t kDebounceRequiredSamples = 6U;

ButtonLedFsm g_fsm;
DebounceFilter g_buttonDebounce;
unsigned long g_nextButtonSampleMs = 0UL;
unsigned long g_nextReportMs = 0UL;
unsigned long g_toggleCount = 0UL;

void applyFsmOutput() {
  ledDriverSet(buttonLedFsmOutput(g_fsm));
}

void updateLcd() {
  lcdDisplayShowStatus(buttonLedFsmStateName(g_fsm),
                       buttonLedFsmOutput(g_fsm),
                       debounceFilterStableValue(g_buttonDebounce),
                       g_toggleCount);
}

void printBanner() {
  printf("\nLab 11 - FSM Button-LED, Part 1 only\n");
  printf("Button: D12 to GND, active LOW with INPUT_PULLUP\n");
  printf("LED: D9 through resistor, active HIGH\n");
  printf("LCD1602: RS=D7 E=D6 D4=D5 D5=D4 D6=D3 D7=D13\n");
  printf("Debounce: %u samples x %lu ms = %lu ms\n",
         kDebounceRequiredSamples,
         kButtonSamplePeriodMs,
         static_cast<unsigned long>(kDebounceRequiredSamples) *
             kButtonSamplePeriodMs);
  printf("Press the button once to toggle the FSM state.\n\n");
}

void printStatus(unsigned long nowMs) {
  printf("STATUS t=%lu state=%s led=%s button=%s toggles=%lu\n",
         nowMs,
         buttonLedFsmStateName(g_fsm),
         buttonLedFsmOutput(g_fsm) ? "ON" : "OFF",
         debounceFilterStableValue(g_buttonDebounce) ? "PRESSED" : "RELEASED",
         g_toggleCount);
}

void sampleButton(unsigned long nowMs) {
  const bool rawPressed = buttonDriverIsPressed();
  const bool changed = debounceFilterUpdate(g_buttonDebounce, rawPressed);

  if (!changed) {
    return;
  }

  if (debounceFilterStableValue(g_buttonDebounce)) {
    const ButtonLedState previousState = buttonLedFsmState(g_fsm);
    buttonLedFsmDispatch(g_fsm, ButtonLedInput::PressEvent);
    applyFsmOutput();
    g_toggleCount++;
    updateLcd();

    printf("EVENT t=%lu type=PRESS from=%s to=%s led=%s\n",
           nowMs,
           buttonLedFsmStateName(previousState),
           buttonLedFsmStateName(g_fsm),
           buttonLedFsmOutput(g_fsm) ? "ON" : "OFF");
    return;
  }

  printf("EVENT t=%lu type=RELEASE state=%s led=%s\n",
         nowMs,
         buttonLedFsmStateName(g_fsm),
         buttonLedFsmOutput(g_fsm) ? "ON" : "OFF");
  updateLcd();
}

}  // namespace

void appControllerSetup() {
  buttonDriverSetup();
  ledDriverSetup();
  lcdDisplayInit();
  serialStdioSetup(115200UL);

  buttonLedFsmInit(g_fsm, ButtonLedState::LedOff);
  debounceFilterInit(g_buttonDebounce,
                     buttonDriverIsPressed(),
                     kDebounceRequiredSamples);
  applyFsmOutput();

  const unsigned long nowMs = millis();
  g_nextButtonSampleMs = nowMs;
  g_nextReportMs = nowMs + kReportPeriodMs;

  printBanner();
  updateLcd();
  printStatus(nowMs);
}

void appControllerLoop() {
  const unsigned long nowMs = millis();

  if (static_cast<long>(nowMs - g_nextButtonSampleMs) >= 0) {
    g_nextButtonSampleMs += kButtonSamplePeriodMs;
    sampleButton(nowMs);
  }

  if (static_cast<long>(nowMs - g_nextReportMs) >= 0) {
    g_nextReportMs += kReportPeriodMs;
    updateLcd();
    printStatus(nowMs);
  }
}
