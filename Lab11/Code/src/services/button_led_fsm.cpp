#include "button_led_fsm.h"

namespace {

struct FsmStateDefinition {
  const char *name;
  bool output;
  ButtonLedState next[2];
};

constexpr FsmStateDefinition kFsmTable[] = {
    {"LED_OFF", false, {ButtonLedState::LedOff, ButtonLedState::LedOn}},
    {"LED_ON", true, {ButtonLedState::LedOn, ButtonLedState::LedOff}},
};

uint8_t stateIndex(ButtonLedState state) {
  return static_cast<uint8_t>(state);
}

uint8_t inputIndex(ButtonLedInput input) {
  return static_cast<uint8_t>(input);
}

}  // namespace

void buttonLedFsmInit(ButtonLedFsm &fsm, ButtonLedState initialState) {
  fsm.state = initialState;
}

void buttonLedFsmDispatch(ButtonLedFsm &fsm, ButtonLedInput input) {
  fsm.state = kFsmTable[stateIndex(fsm.state)].next[inputIndex(input)];
}

ButtonLedState buttonLedFsmState(const ButtonLedFsm &fsm) {
  return fsm.state;
}

bool buttonLedFsmOutput(const ButtonLedFsm &fsm) {
  return kFsmTable[stateIndex(fsm.state)].output;
}

const char *buttonLedFsmStateName(const ButtonLedFsm &fsm) {
  return buttonLedFsmStateName(fsm.state);
}

const char *buttonLedFsmStateName(ButtonLedState state) {
  return kFsmTable[stateIndex(state)].name;
}
