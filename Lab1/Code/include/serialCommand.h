/* serialCommand.h -- Serial command module interface (STDIO over UART) */

#ifndef SERIAL_COMMAND_H
#define SERIAL_COMMAND_H

#include <Arduino.h>

void serialCommandInit(unsigned long baudRate); /* Open UART, redirect STDIO */
void serialCommandRead(char *buffer, uint8_t maxLength); /* Blocking line read via getchar() */
void serialCommandProcess(const char *command);  /* Parse with sscanf(), dispatch */

#endif // SERIAL_COMMAND_H
