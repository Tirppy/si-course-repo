#ifndef SERIAL_STDIO_H
#define SERIAL_STDIO_H

#include <Arduino.h>

void serialStdioInit(unsigned long baudRate);
void serialStdioSetLcdMirror(bool enabled);

#endif
