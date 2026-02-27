#include "stdio_bridge.h"

#include <stdio.h>

#include "mcu_registers.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

namespace {
void writeRaw(uint8_t byte) {
  while ((UCSR0A_REG & (1U << BIT_UDRE0)) == 0U) {
  }
  UDR0_REG = byte;
}

int serialPutCharacter(char character, FILE*) {
  if (character == '\n') {
    writeRaw(static_cast<uint8_t>('\r'));
  }
  writeRaw(static_cast<uint8_t>(character));
  return 0;
}

FILE g_stdoutStream;
}

void stdioBridgeInit(uint32_t baudRate) {
  const uint16_t ubrr = static_cast<uint16_t>((F_CPU / (16UL * baudRate)) - 1UL);

  UBRR0H_REG = static_cast<uint8_t>(ubrr >> 8);
  UBRR0L_REG = static_cast<uint8_t>(ubrr & 0xFFU);
  UCSR0A_REG = 0U;
  UCSR0B_REG = (1U << BIT_TXEN0);
  UCSR0C_REG = (1U << BIT_UCSZ01) | (1U << BIT_UCSZ00);

  fdev_setup_stream(&g_stdoutStream, serialPutCharacter, nullptr, _FDEV_SETUP_WRITE);
  stdout = &g_stdoutStream;
}