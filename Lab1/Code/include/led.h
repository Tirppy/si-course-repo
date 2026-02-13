/* led.h -- LED peripheral module interface */

#ifndef LED_H
#define LED_H

#include <Arduino.h>

void ledInit(uint8_t pin);   /* Configure pin as OUTPUT, LED off */
void ledOn(void);            /* Set LED HIGH */
void ledOff(void);           /* Set LED LOW  */
uint8_t ledGetState(void);   /* Return current state (HIGH/LOW) */

#endif // LED_H
