#ifndef SERIAL_COMMAND_H
#define SERIAL_COMMAND_H

#include <Arduino.h>

void serialCommandInit(unsigned long baudRate);
int  serialCommandRead(char *buffer, uint8_t maxLength);
void serialCommandProcess(const char *command);

#endif // SERIAL_COMMAND_H
