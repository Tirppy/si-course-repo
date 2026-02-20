#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define SerialBaudRate 9600

#define GreenLedPin A0
#define RedLedPin A1

#define KeypadRows 4
#define KeypadCols 4

static const byte KeypadRowPins[KeypadRows] = {7, 6, 5, 4};
static const byte KeypadColPins[KeypadCols] = {3, 2, A3, A2};

#define CodeLength 4
#define ValidAccessCode "1234"

#define LcdColumns 16
#define LcdRows 2

#define LcdRsPin 13
#define LcdEnPin 12
#define LcdD4Pin 11
#define LcdD5Pin 10
#define LcdD6Pin 9
#define LcdD7Pin 8

#define KeyDebounceDelayMs 180
#define StatusDisplayDelayMs 2000

#endif
