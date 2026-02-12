#ifndef LED_H
#define LED_H

#include <Arduino.h>

void ledInit(uint8_t pin);
void ledOn(void);
void ledOff(void);
uint8_t ledGetState(void);

#endif // LED_H
