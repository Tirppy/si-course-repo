#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <Arduino.h>

#include "app/app_state.h"

enum ParsedCommandType : uint8_t {
  PARSED_COMMAND_NONE = 0,
  PARSED_COMMAND_SET_BINARY_STATE,
  PARSED_COMMAND_SET_ANALOG_LEVEL,
  PARSED_COMMAND_ANALOG_UP,
  PARSED_COMMAND_ANALOG_DOWN,
  PARSED_COMMAND_ANALOG_STOP,
  PARSED_COMMAND_STATUS,
  PARSED_COMMAND_HELP,
  PARSED_COMMAND_REPORT,
  PARSED_COMMAND_SET_DEBOUNCE,
  PARSED_COMMAND_SET_INPUT_MODE,
  PARSED_COMMAND_PRINT_HISTORY,
  PARSED_COMMAND_CLEAR_HISTORY,
  PARSED_COMMAND_PRINT_STATS,
  PARSED_COMMAND_RESET_STATS,
  PARSED_COMMAND_SET_AUTOREPORT,
  PARSED_COMMAND_SET_PLOT,
  PARSED_COMMAND_INVALID,
};

struct ParsedCommand {
  ParsedCommandType type;
  bool binaryState;
  float analogPercent;
  uint16_t debounceWindowMs;
  InputMode inputMode;
  bool plotEnabled;
  bool autoReportEnabled;
};

ParsedCommand parseCommandLine(const char *lineBuffer);

#endif
