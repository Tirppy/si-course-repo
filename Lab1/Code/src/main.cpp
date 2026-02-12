/*
 * Lab 1.1 - STDIO Serial Interface: LED Control
 * MCU: Arduino Uno (ATmega328P)
 * 
 * Commands (via serial terminal at 9600 baud):
 *   led on  - Turn the LED on
 *   led off - Turn the LED off
 */

#include <Arduino.h>
#include "led.h"
#include "serialCommand.h"

#define LED_PIN       13   /* Built-in LED on Arduino Uno */
#define BAUD_RATE     9600
#define CMD_BUF_SIZE  32

static char cmdBuffer[CMD_BUF_SIZE];

void setup() {
    ledInit(LED_PIN);
    serialCommandInit(BAUD_RATE);
}

void loop() {
    int result = serialCommandRead(cmdBuffer, CMD_BUF_SIZE);
    if (result >= 0) {
        serialCommandProcess(cmdBuffer);
    }
}