#ifndef BUTTON_LED_FSM_H
#define BUTTON_LED_FSM_H

#include <stdint.h>

enum class ButtonLedState : uint8_t {
  LedOff = 0U,
  LedOn = 1U,
};

enum class ButtonLedInput : uint8_t {
  NoEvent = 0U,
  PressEvent = 1U,
};

struct ButtonLedFsm {
  ButtonLedState state;
};

void buttonLedFsmInit(ButtonLedFsm &fsm, ButtonLedState initialState);
void buttonLedFsmDispatch(ButtonLedFsm &fsm, ButtonLedInput input);
ButtonLedState buttonLedFsmState(const ButtonLedFsm &fsm);
bool buttonLedFsmOutput(const ButtonLedFsm &fsm);
const char *buttonLedFsmStateName(const ButtonLedFsm &fsm);
const char *buttonLedFsmStateName(ButtonLedState state);

#endif
