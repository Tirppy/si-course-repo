#include "platform_time.h"

#include "mcu_registers.h"

namespace {
volatile uint32_t g_millis = 0U;
}

extern "C" void __vector_14(void) __attribute__((signal, used, externally_visible));
extern "C" void __vector_14(void) {
  g_millis++;
}

void platformTimeInit() {
  TCCR0A_REG = (1U << BIT_WGM01);
  TCCR0B_REG = (1U << BIT_CS01) | (1U << BIT_CS00);
  OCR0A_REG = 249U;
  TIMSK0_REG = (1U << BIT_OCIE0A);
  enableGlobalInterrupts();
}

uint32_t platformMillis() {
  const uint8_t sreg = SREG_REG;
  disableGlobalInterrupts();
  const uint32_t now = g_millis;
  SREG_REG = sreg;
  return now;
}