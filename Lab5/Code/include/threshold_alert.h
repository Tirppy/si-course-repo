#ifndef THRESHOLD_ALERT_H
#define THRESHOLD_ALERT_H

#include "models.h"

void thresholdAlertReset(ThresholdAlertState &state);
void thresholdAlertUpdate(float temperatureC,
                          bool valid,
                          const ThresholdAlertConfig &config,
                          ThresholdAlertState &state);

#endif
