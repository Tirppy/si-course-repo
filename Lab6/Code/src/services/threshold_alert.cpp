#include "threshold_alert.h"

void thresholdAlertReset(ThresholdAlertState &state) {
  state.hysteresisState = false;
  state.stableState = false;
  state.counter = 0U;
}

void thresholdAlertUpdate(float temperatureC,
                          bool valid,
                          const ThresholdAlertConfig &config,
                          ThresholdAlertState &state) {
  if (!valid) {
    thresholdAlertReset(state);
    return;
  }

  if (temperatureC >= (config.thresholdC + config.hysteresisC)) {
    state.hysteresisState = true;
  } else if (temperatureC <= (config.thresholdC - config.hysteresisC)) {
    state.hysteresisState = false;
  }

  if (state.hysteresisState) {
    if (state.counter < config.confirmSamples) {
      ++state.counter;
    }
  } else if (state.counter > 0U) {
    --state.counter;
  }

  if (state.counter >= config.confirmSamples) {
    state.stableState = true;
  } else if (state.counter == 0U) {
    state.stableState = false;
  }
}
