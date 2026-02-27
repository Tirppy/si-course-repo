#include "button.h"

#include "mcu_registers.h"
#include "pins.h"

void buttonInit() {
  DDRD_REG &= static_cast<uint8_t>(~(1U << PIN_BUTTON_BIT));
  PORTD_REG |= (1U << PIN_BUTTON_BIT);
}

bool buttonIsPressed() {
  return (PIND_REG & (1U << PIN_BUTTON_BIT)) == 0U;
}