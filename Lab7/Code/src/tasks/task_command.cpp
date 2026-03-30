#include "tasks/task_command.h"

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>

#include "app/app_state.h"
#include "services/analog_conditioning.h"
#include "services/command_conditioning.h"
#include "services/command_parser.h"
#include "services/runtime_telemetry.h"
#include "tasks/task_reporting.h"

namespace {

constexpr size_t COMMAND_BUFFER_SIZE = 32U;
constexpr float ANALOG_STEP_PERCENT = 10.0F;

char g_commandBuffer[COMMAND_BUFFER_SIZE];
size_t g_commandLength = 0U;
bool g_lastCharacterWasTerminator = false;

void printHelp() {
  printf_P(PSTR("Commands:\n"));
  printf_P(PSTR(" SET_RELAY ON|OFF\n"));
  printf_P(PSTR(" ON | OFF\n"));
  printf_P(PSTR(" SET_ANALOG <0..100>\n"));
  printf_P(PSTR(" ANALOG_UP | ANALOG_DOWN | ANALOG_STOP\n"));
  printf_P(PSTR(" STATUS | REPORT\n"));
  printf_P(PSTR(" INPUT SERIAL|HARDWARE|HYBRID\n"));
  printf_P(PSTR(" HISTORY | HISTORY CLEAR\n"));
  printf_P(PSTR(" STATS | STATS RESET\n"));
  printf_P(PSTR(" DEBOUNCE <20..500>\n"));
  printf_P(PSTR(" AUTOREPORT ON|OFF\n"));
  printf_P(PSTR(" PLOT ON|OFF\n"));
}

void storeLastCommand(const char *commandText) {
  strncpy(g_appState.lastCommandText, commandText, sizeof(g_appState.lastCommandText) - 1U);
  g_appState.lastCommandText[sizeof(g_appState.lastCommandText) - 1U] = '\0';
}

void handleParsedCommand(const ParsedCommand &parsed, const char *commandText) {
  const unsigned long nowMs = millis();
  storeLastCommand(commandText);

  switch (parsed.type) {
    case PARSED_COMMAND_SET_BINARY_STATE:
      commandConditioningSetRawCommand(parsed.actuatorCommand, nowMs);
      ++g_appState.acceptedCommandCount;
      runtimeTelemetryLog(RUNTIME_EVENT_BINARY_CMD,
                          parsed.actuatorCommand == ACTUATOR_COMMAND_ON ? 1 : 0,
                          nowMs);
      printf_P(PSTR("ACK binary command %s accepted; debounce pending\n"),
              actuatorCommandToText(parsed.actuatorCommand));
      break;

    case PARSED_COMMAND_SET_ANALOG_LEVEL:
      analogConditioningSetRawPercent(parsed.analogPercent, nowMs);
      ++g_appState.acceptedCommandCount;
      runtimeTelemetryLog(RUNTIME_EVENT_ANALOG_CMD, static_cast<int16_t>(parsed.analogPercent), nowMs);
      printf_P(PSTR("ACK analog request %d%% accepted\n"), static_cast<int>(parsed.analogPercent));
      break;

    case PARSED_COMMAND_ANALOG_UP:
      analogConditioningSetRawPercent(g_appState.analog.rawPercent + ANALOG_STEP_PERCENT, nowMs);
      ++g_appState.acceptedCommandCount;
      runtimeTelemetryLog(RUNTIME_EVENT_ANALOG_CMD, static_cast<int16_t>(g_appState.analog.rawPercent), nowMs);
      printf_P(PSTR("ACK analog step up\n"));
      break;

    case PARSED_COMMAND_ANALOG_DOWN:
      analogConditioningSetRawPercent(g_appState.analog.rawPercent - ANALOG_STEP_PERCENT, nowMs);
      ++g_appState.acceptedCommandCount;
      runtimeTelemetryLog(RUNTIME_EVENT_ANALOG_CMD, static_cast<int16_t>(g_appState.analog.rawPercent), nowMs);
      printf_P(PSTR("ACK analog step down\n"));
      break;

    case PARSED_COMMAND_ANALOG_STOP:
      analogConditioningSetRawPercent(0.0F, nowMs);
      ++g_appState.acceptedCommandCount;
      runtimeTelemetryLog(RUNTIME_EVENT_ANALOG_CMD, 0, nowMs);
      printf_P(PSTR("ACK analog stop\n"));
      break;

    case PARSED_COMMAND_STATUS:
    case PARSED_COMMAND_REPORT:
      ++g_appState.acceptedCommandCount;
      taskReportingPrintImmediate("manual");
      break;

    case PARSED_COMMAND_SET_INPUT_MODE:
      g_appState.inputMode = parsed.inputMode;
      ++g_appState.acceptedCommandCount;
      runtimeTelemetryLog(RUNTIME_EVENT_INPUT_MODE, parsed.inputMode, nowMs);
      printf_P(PSTR("ACK input mode %s\n"), inputModeToText(parsed.inputMode));
      break;

    case PARSED_COMMAND_PRINT_HISTORY:
      ++g_appState.acceptedCommandCount;
      runtimeTelemetryPrintHistory();
      break;

    case PARSED_COMMAND_CLEAR_HISTORY:
      runtimeTelemetryClearHistory();
      runtimeTelemetryLog(RUNTIME_EVENT_HISTORY_CLEAR, 0, nowMs);
      ++g_appState.acceptedCommandCount;
      printf_P(PSTR("ACK history cleared\n"));
      break;

    case PARSED_COMMAND_PRINT_STATS:
      ++g_appState.acceptedCommandCount;
      runtimeTelemetryPrintStats();
      break;

    case PARSED_COMMAND_RESET_STATS:
      runtimeTelemetryResetStats();
      runtimeTelemetryLog(RUNTIME_EVENT_STATS_RESET, 0, nowMs);
      ++g_appState.acceptedCommandCount;
      printf_P(PSTR("ACK stats reset\n"));
      break;

    case PARSED_COMMAND_HELP:
      ++g_appState.acceptedCommandCount;
      printHelp();
      break;

    case PARSED_COMMAND_SET_DEBOUNCE:
      commandConditioningSetDebounceWindow(parsed.debounceWindowMs);
      g_appState.binary.debounceWindowMs = parsed.debounceWindowMs;
      ++g_appState.acceptedCommandCount;
      printf_P(PSTR("ACK debounce window set to %u ms\n"), parsed.debounceWindowMs);
      break;

    case PARSED_COMMAND_SET_AUTOREPORT:
      g_appState.autoReportEnabled = parsed.autoReportEnabled;
      ++g_appState.acceptedCommandCount;
      printf_P(PSTR("ACK auto report %s\n"), parsed.autoReportEnabled ? "enabled" : "paused");
      break;

    case PARSED_COMMAND_SET_PLOT:
      g_appState.plotEnabled = parsed.plotEnabled;
      ++g_appState.acceptedCommandCount;
      printf_P(PSTR("ACK plot output %s\n"), parsed.plotEnabled ? "enabled" : "disabled");
      break;

    case PARSED_COMMAND_INVALID:
      ++g_appState.rejectedCommandCount;
      runtimeTelemetryLog(RUNTIME_EVENT_INVALID_CMD, 0, nowMs);
      printf_P(PSTR("ERR invalid command: %s\n"), commandText);
      printf_P(PSTR("Type HELP for command list\n"));
      break;

    case PARSED_COMMAND_NONE:
    default:
      break;
  }
}

void processCommandLine() {
  ParsedCommand parsed = parseCommandLine(g_commandBuffer);
  handleParsedCommand(parsed, g_commandBuffer);
  g_commandBuffer[0] = '\0';
  g_commandLength = 0U;
}

}  // namespace

void taskCommandSetup() {
  g_commandBuffer[0] = '\0';
  g_commandLength = 0U;
  g_lastCharacterWasTerminator = false;
}

void taskCommandRun() {
  while (Serial.available() > 0) {
    const char input = static_cast<char>(Serial.read());

    if (input == '\r' || input == '\n') {
      if (g_lastCharacterWasTerminator) {
        continue;
      }

      g_lastCharacterWasTerminator = true;
      printf_P(PSTR("\n"));
      if (g_commandLength > 0U) {
        g_commandBuffer[g_commandLength] = '\0';
        processCommandLine();
      }
      continue;
    }

    g_lastCharacterWasTerminator = false;

    if (input == '\b' || static_cast<unsigned char>(input) == 127U) {
      if (g_commandLength > 0U) {
        --g_commandLength;
        g_commandBuffer[g_commandLength] = '\0';
        printf_P(PSTR("\b \b"));
      }
      continue;
    }

    if (g_commandLength + 1U < COMMAND_BUFFER_SIZE) {
      g_commandBuffer[g_commandLength++] = input;
      g_commandBuffer[g_commandLength] = '\0';
      Serial.write(input);
    }
  }
}
