/* main.cpp -- Lab 1.1: STDIO Serial Interface, LED Control (ATmega328P) */

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
    serialCommandRead(cmdBuffer, CMD_BUF_SIZE);   /* blocks until line is complete */
    serialCommandProcess(cmdBuffer);
}