#include "services/command_parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace {

void normalizeToken(const char *source, char *destination, size_t size) {
  size_t index = 0U;
  while (source[index] != '\0' && index + 1U < size) {
    destination[index] = static_cast<char>(toupper(static_cast<unsigned char>(source[index])));
    ++index;
  }
  destination[index] = '\0';
}

ParsedCommand invalidCommand() {
  ParsedCommand command = {PARSED_COMMAND_INVALID, ACTUATOR_COMMAND_OFF, 0.0F, 0U, INPUT_MODE_HYBRID, false, false};
  return command;
}

}  // namespace

ParsedCommand parseCommandLine(const char *lineBuffer) {
  ParsedCommand parsed = {PARSED_COMMAND_NONE, ACTUATOR_COMMAND_OFF, 0.0F, 0U, INPUT_MODE_HYBRID, false, false};
  char localBuffer[48];
  char firstToken[24];
  char secondToken[24];
  char extraToken[24];

  localBuffer[0] = '\0';
  firstToken[0] = '\0';
  secondToken[0] = '\0';
  extraToken[0] = '\0';

  strncpy(localBuffer, lineBuffer, sizeof(localBuffer) - 1U);
  localBuffer[sizeof(localBuffer) - 1U] = '\0';

  const int tokenCount = sscanf(localBuffer, "%23s %23s %23s", firstToken, secondToken, extraToken);
  if (tokenCount <= 0) {
    return parsed;
  }

  normalizeToken(firstToken, firstToken, sizeof(firstToken));
  normalizeToken(secondToken, secondToken, sizeof(secondToken));
  normalizeToken(extraToken, extraToken, sizeof(extraToken));

  if (strcmp(firstToken, "ON") == 0) {
      parsed.type = PARSED_COMMAND_SET_BINARY_STATE;
      parsed.actuatorCommand = ACTUATOR_COMMAND_ON;
      return parsed;
  }

  if (strcmp(firstToken, "OFF") == 0) {
      parsed.type = PARSED_COMMAND_SET_BINARY_STATE;
      parsed.actuatorCommand = ACTUATOR_COMMAND_OFF;
      return parsed;
  }

  if (strcmp(firstToken, "SET_RELAY") == 0) {
    if (strcmp(secondToken, "ON") == 0) {
      parsed.type = PARSED_COMMAND_SET_BINARY_STATE;
      parsed.actuatorCommand = ACTUATOR_COMMAND_ON;
      return parsed;
    }

    if (strcmp(secondToken, "OFF") == 0) {
      parsed.type = PARSED_COMMAND_SET_BINARY_STATE;
      parsed.actuatorCommand = ACTUATOR_COMMAND_OFF;
      return parsed;
    }

    return invalidCommand();
  }

  if (strcmp(firstToken, "SET_ANALOG") == 0) {
    char *endPointer = nullptr;
    const long analogPercentLong = strtol(secondToken, &endPointer, 10);
    if (endPointer == secondToken || *endPointer != '\0') {
      return invalidCommand();
    }

    parsed.type = PARSED_COMMAND_SET_ANALOG_LEVEL;
    parsed.analogPercent = static_cast<float>(analogPercentLong);
    return parsed;
  }

  if (strcmp(firstToken, "ANALOG_UP") == 0) {
    parsed.type = PARSED_COMMAND_ANALOG_UP;
    return parsed;
  }

  if (strcmp(firstToken, "ANALOG_DOWN") == 0) {
    parsed.type = PARSED_COMMAND_ANALOG_DOWN;
    return parsed;
  }

  if (strcmp(firstToken, "ANALOG_STOP") == 0) {
    parsed.type = PARSED_COMMAND_ANALOG_STOP;
    return parsed;
  }

  if (strcmp(firstToken, "STATUS") == 0) {
    parsed.type = PARSED_COMMAND_STATUS;
    return parsed;
  }

  if (strcmp(firstToken, "HELP") == 0) {
    parsed.type = PARSED_COMMAND_HELP;
    return parsed;
  }

  if (strcmp(firstToken, "REPORT") == 0) {
    parsed.type = PARSED_COMMAND_REPORT;
    return parsed;
  }

  if (strcmp(firstToken, "INPUT") == 0) {
    if (strcmp(secondToken, "SERIAL") == 0) {
      parsed.type = PARSED_COMMAND_SET_INPUT_MODE;
      parsed.inputMode = INPUT_MODE_SERIAL;
      return parsed;
    }
    if (strcmp(secondToken, "HARDWARE") == 0) {
      parsed.type = PARSED_COMMAND_SET_INPUT_MODE;
      parsed.inputMode = INPUT_MODE_HARDWARE;
      return parsed;
    }
    if (strcmp(secondToken, "HYBRID") == 0) {
      parsed.type = PARSED_COMMAND_SET_INPUT_MODE;
      parsed.inputMode = INPUT_MODE_HYBRID;
      return parsed;
    }
    return invalidCommand();
  }

  if (strcmp(firstToken, "HISTORY") == 0) {
    if (strcmp(secondToken, "CLEAR") == 0) {
      parsed.type = PARSED_COMMAND_CLEAR_HISTORY;
      return parsed;
    }
    parsed.type = PARSED_COMMAND_PRINT_HISTORY;
    return parsed;
  }

  if (strcmp(firstToken, "STATS") == 0) {
    if (strcmp(secondToken, "RESET") == 0) {
      parsed.type = PARSED_COMMAND_RESET_STATS;
      return parsed;
    }
    parsed.type = PARSED_COMMAND_PRINT_STATS;
    return parsed;
  }

  if (strcmp(firstToken, "PLOT") == 0) {
    if (strcmp(secondToken, "ON") == 0) {
      parsed.type = PARSED_COMMAND_SET_PLOT;
      parsed.plotEnabled = true;
      return parsed;
    }

    if (strcmp(secondToken, "OFF") == 0) {
      parsed.type = PARSED_COMMAND_SET_PLOT;
      parsed.plotEnabled = false;
      return parsed;
    }

    return invalidCommand();
  }

  if (strcmp(firstToken, "AUTOREPORT") == 0) {
    if (strcmp(secondToken, "ON") == 0) {
      parsed.type = PARSED_COMMAND_SET_AUTOREPORT;
      parsed.autoReportEnabled = true;
      return parsed;
    }

    if (strcmp(secondToken, "OFF") == 0) {
      parsed.type = PARSED_COMMAND_SET_AUTOREPORT;
      parsed.autoReportEnabled = false;
      return parsed;
    }

    return invalidCommand();
  }

  if (strcmp(firstToken, "DEBOUNCE") == 0) {
    const long debounceWindowMs = strtol(secondToken, nullptr, 10);
    if (debounceWindowMs < 20L || debounceWindowMs > 500L) {
      return invalidCommand();
    }

    parsed.type = PARSED_COMMAND_SET_DEBOUNCE;
    parsed.debounceWindowMs = static_cast<uint16_t>(debounceWindowMs);
    return parsed;
  }

  return invalidCommand();
}
