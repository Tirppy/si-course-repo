#ifndef MCU_REGISTERS_H
#define MCU_REGISTERS_H

#include <stdint.h>

inline volatile uint8_t& reg8(uint16_t address) {
  return *reinterpret_cast<volatile uint8_t*>(address);
}

#define DDRB_REG reg8(0x24U)
#define PORTB_REG reg8(0x25U)
#define DDRD_REG reg8(0x2AU)
#define PORTD_REG reg8(0x2BU)
#define PIND_REG reg8(0x29U)

#define TCCR0A_REG reg8(0x44U)
#define TCCR0B_REG reg8(0x45U)
#define OCR0A_REG reg8(0x47U)
#define TIMSK0_REG reg8(0x6EU)

#define UCSR0A_REG reg8(0xC0U)
#define UCSR0B_REG reg8(0xC1U)
#define UCSR0C_REG reg8(0xC2U)
#define UBRR0L_REG reg8(0xC4U)
#define UBRR0H_REG reg8(0xC5U)
#define UDR0_REG reg8(0xC6U)

#define SREG_REG reg8(0x5FU)

constexpr uint8_t BIT_WGM01 = 1U;
constexpr uint8_t BIT_CS00 = 0U;
constexpr uint8_t BIT_CS01 = 1U;
constexpr uint8_t BIT_OCIE0A = 1U;

constexpr uint8_t BIT_TXEN0 = 3U;
constexpr uint8_t BIT_UCSZ00 = 1U;
constexpr uint8_t BIT_UCSZ01 = 2U;
constexpr uint8_t BIT_UDRE0 = 5U;

inline void disableGlobalInterrupts() {
  __asm__ __volatile__("cli" ::: "memory");
}

inline void enableGlobalInterrupts() {
  __asm__ __volatile__("sei" ::: "memory");
}

#endif