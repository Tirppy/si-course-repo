#include "services/setpoint_manager.h"

#include <Arduino.h>

#include "app/app_state.h"
#include "config/constants.h"
#include "drivers/serial_stdio.h"
#include "drivers/setpoint_input.h"

namespace {

char g_commandBuffer[32];
uint8_t g_commandLength = 0U;

void resetPidState() {
  g_appState.control.pidIntegral = 0.0F;
  g_appState.control.pidPreviousError = 0.0F;
  g_appState.control.pidOutputPercent = 0.0F;
}

bool startsWithIgnoreCase(const String& input, const char* prefix) {
  String candidate = input;
  candidate.trim();
  candidate.toUpperCase();
  String expected(prefix);
  expected.toUpperCase();
  return candidate.startsWith(expected);
}

void handleCommand(const String& rawCommand) {
  String command = rawCommand;
  command.trim();

  if (command.length() == 0) {
    return;
  }

  if (startsWithIgnoreCase(command, "AUTO")) {
    g_appState.control.serialOverrideEnabled = false;
    g_appState.control.source = CommandSource::Potentiometer;
    serialStdioPrintMessage("Mode=AUTO -> setpoint from potentiometer.");
    return;
  }

  if (startsWithIgnoreCase(command, "HELP")) {
    serialStdioPrintHelp();
    return;
  }

  if (startsWithIgnoreCase(command, "REPORT OFF")) {
    g_appState.periodicReportingEnabled = false;
    serialStdioPrintMessage("Periodic reporting disabled.");
    return;
  }

  if (startsWithIgnoreCase(command, "REPORT ON")) {
    g_appState.periodicReportingEnabled = true;
    serialStdioPrintMessage("Periodic reporting enabled.");
    return;
  }

  if (startsWithIgnoreCase(command, "REPORT NOW")) {
    g_appState.oneShotReportRequested = true;
    serialStdioPrintMessage("One-time report requested.");
    return;
  }

  if (startsWithIgnoreCase(command, "MODE ")) {
    const String valueText = command.substring(command.indexOf(' ') + 1);
    if (startsWithIgnoreCase(valueText, "P1") || startsWithIgnoreCase(valueText, "PART1")) {
      g_appState.control.mode = ControlMode::Part1OnOff;
      resetPidState();
      serialStdioPrintMessage("Control mode set to Part 1 ON-OFF heating.");
    } else if (startsWithIgnoreCase(valueText, "P2") ||
               startsWithIgnoreCase(valueText, "PART2")) {
      g_appState.control.mode = ControlMode::Part2Pid;
      resetPidState();
      serialStdioPrintMessage("Control mode set to Part 2 PID cooling.");
    } else if (startsWithIgnoreCase(valueText, "HY") ||
               startsWithIgnoreCase(valueText, "HYBRID")) {
      g_appState.control.mode = ControlMode::Hybrid;
      resetPidState();
      serialStdioPrintMessage("Control mode set to hybrid heating/cooling.");
    } else {
      serialStdioPrintMessage("Invalid mode. Use MODE P1, MODE P2 or MODE HYBRID.");
    }
    return;
  }

  if (startsWithIgnoreCase(command, "HYST ")) {
    const int separatorIndex = command.indexOf(' ');
    const float requestedValue = command.substring(separatorIndex + 1).toFloat();
    if (requestedValue >= kHysteresisMinC && requestedValue <= kHysteresisMaxC) {
      g_appState.control.hysteresisWidthC = requestedValue;
      serialStdioPrintMessage("Hysteresis width updated.");
    } else {
      serialStdioPrintMessage("Invalid hysteresis. Use 0.2..10.0 C.");
    }
    return;
  }

  if (startsWithIgnoreCase(command, "KP ")) {
    g_appState.control.pidKp = command.substring(command.indexOf(' ') + 1).toFloat();
    resetPidState();
    serialStdioPrintMessage("PID Kp updated.");
    return;
  }

  if (startsWithIgnoreCase(command, "KI ")) {
    g_appState.control.pidKi = command.substring(command.indexOf(' ') + 1).toFloat();
    resetPidState();
    serialStdioPrintMessage("PID Ki updated.");
    return;
  }

  if (startsWithIgnoreCase(command, "KD ")) {
    g_appState.control.pidKd = command.substring(command.indexOf(' ') + 1).toFloat();
    resetPidState();
    serialStdioPrintMessage("PID Kd updated.");
    return;
  }

  if (startsWithIgnoreCase(command, "TMIN ")) {
    const int separatorIndex = command.indexOf(' ');
    const float requestedValue = command.substring(separatorIndex + 1).toFloat();
    if (requestedValue < g_appState.control.acceptableMaxTemperatureC) {
      g_appState.control.acceptableMinTemperatureC = requestedValue;
      serialStdioPrintMessage("Acceptable minimum temperature updated.");
    } else {
      serialStdioPrintMessage("Invalid TMIN. It must stay below TMAX.");
    }
    return;
  }

  if (startsWithIgnoreCase(command, "TMAX ")) {
    const int separatorIndex = command.indexOf(' ');
    const float requestedValue = command.substring(separatorIndex + 1).toFloat();
    if (requestedValue > g_appState.control.acceptableMinTemperatureC) {
      g_appState.control.acceptableMaxTemperatureC = requestedValue;
      serialStdioPrintMessage("Acceptable maximum temperature updated.");
    } else {
      serialStdioPrintMessage("Invalid TMAX. It must stay above TMIN.");
    }
    return;
  }

  if (startsWithIgnoreCase(command, "SET ")) {
    const int separatorIndex = command.indexOf(' ');
    const String valueText = command.substring(separatorIndex + 1);
    const float requestedTemperature = valueText.toFloat();

    if (requestedTemperature >= kSetpointMinC && requestedTemperature <= kSetpointMaxC) {
      g_appState.control.setpointTemperatureC = requestedTemperature;
      g_appState.control.serialOverrideEnabled = true;
      g_appState.control.source = CommandSource::Serial;
      resetPidState();
      serialStdioPrintMessage("Serial setpoint updated.");
    } else {
      serialStdioPrintMessage("Invalid setpoint. Use 18.0..45.0 C.");
    }
    return;
  }

  if (startsWithIgnoreCase(command, "UP")) {
    g_appState.control.setpointTemperatureC += kSerialSetpointStepC;
    if (g_appState.control.setpointTemperatureC > kSetpointMaxC) {
      g_appState.control.setpointTemperatureC = kSetpointMaxC;
    }
    g_appState.control.serialOverrideEnabled = true;
    g_appState.control.source = CommandSource::Serial;
    resetPidState();
    serialStdioPrintMessage("Serial setpoint increased.");
    return;
  }

  if (startsWithIgnoreCase(command, "DOWN")) {
    g_appState.control.setpointTemperatureC -= kSerialSetpointStepC;
    if (g_appState.control.setpointTemperatureC < kSetpointMinC) {
      g_appState.control.setpointTemperatureC = kSetpointMinC;
    }
    g_appState.control.serialOverrideEnabled = true;
    g_appState.control.source = CommandSource::Serial;
    resetPidState();
    serialStdioPrintMessage("Serial setpoint decreased.");
    return;
  }

  serialStdioPrintMessage("Unknown command. Use HELP.");
}

}  // namespace

bool setpointManagerProcessSerial() {
  bool commandHandled = false;
  static unsigned long lastRxMs = 0UL;

  while (Serial.available() > 0) {
    const char currentChar = static_cast<char>(Serial.read());
    lastRxMs = millis();

    if (currentChar == '\r') {
      continue;
    }

    if (currentChar == '\n') {
      g_commandBuffer[g_commandLength] = '\0';
      handleCommand(String(g_commandBuffer));
      g_commandLength = 0U;
      commandHandled = true;
      continue;
    }

    if (g_commandLength < (sizeof(g_commandBuffer) - 1U)) {
      g_commandBuffer[g_commandLength++] = currentChar;
    }
  }

  if (g_commandLength > 0U && lastRxMs > 0UL) {
    const unsigned long silenceMs = 1200UL;
    if ((millis() - lastRxMs) >= silenceMs) {
      g_commandBuffer[g_commandLength] = '\0';
      handleCommand(String(g_commandBuffer));
      g_commandLength = 0U;
      lastRxMs = 0UL;
      commandHandled = true;
    }
  }

  return commandHandled;
}

float setpointManagerReadPotTemperature() {
  const int rawValue = setpointInputReadRaw();
  const float span = kSetpointMaxC - kSetpointMinC;
  return kSetpointMinC + (static_cast<float>(rawValue) / static_cast<float>(kAdcMaxValue)) * span;
}
