#ifndef BUTTON_BRIDGE_H
#define BUTTON_BRIDGE_H

#include <stdbool.h>
#include <stdint.h>

void buttonBridgeInit();
void buttonBridgeUpdate(uint32_t nowMs);
bool buttonBridgeIsPressed();

#endif