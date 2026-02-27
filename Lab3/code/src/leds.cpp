#include "leds.h"

#include "mcu_registers.h"
#include "pins.h"

void ledsInit() {
  DDRB_REG |= (1U << PIN_LED_GREEN_BIT) | (1U << PIN_LED_RED_BIT) | (1U << PIN_LED_YELLOW_BIT);
  PORTB_REG &= static_cast<uint8_t>(~((1U << PIN_LED_GREEN_BIT) | (1U << PIN_LED_RED_BIT) | (1U << PIN_LED_YELLOW_BIT)));
}

void ledsGreenOn() {
  PORTB_REG |= (1U << PIN_LED_GREEN_BIT);
}

void ledsGreenOff() {
  PORTB_REG &= static_cast<uint8_t>(~(1U << PIN_LED_GREEN_BIT));
}

void ledsRedOn() {
  PORTB_REG |= (1U << PIN_LED_RED_BIT);
}

void ledsRedOff() {
  PORTB_REG &= static_cast<uint8_t>(~(1U << PIN_LED_RED_BIT));
}

void ledsYellowOn() {
  PORTB_REG |= (1U << PIN_LED_YELLOW_BIT);
}

void ledsYellowOff() {
  PORTB_REG &= static_cast<uint8_t>(~(1U << PIN_LED_YELLOW_BIT));
}

void ledsShortLongOff() {
  ledsGreenOff();
  ledsRedOff();
}