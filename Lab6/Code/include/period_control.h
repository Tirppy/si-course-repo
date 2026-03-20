#ifndef PERIOD_CONTROL_H
#define PERIOD_CONTROL_H

#include <stdint.h>

void periodControlInit();
uint16_t periodControlReadRaw();
uint32_t mapPotentiometerToAcquisitionPeriodMs(uint16_t rawAdc);
uint32_t mapAcquisitionToReportPeriodMs(uint32_t acquisitionPeriodMs);

#endif
