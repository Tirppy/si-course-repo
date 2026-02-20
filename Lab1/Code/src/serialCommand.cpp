#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "serialCommand.h"
#include "led.h"

static int uartPutChar(char c, FILE *stream) {
    (void)stream;
    if (c == '\n') {
        Serial.write('\r');
    }
    Serial.write((uint8_t)c);
    return 0;
}

static int uartGetChar(FILE *stream) {
    (void)stream;
    while (!Serial.available()) {
        
    }
    int c = Serial.read();
    if (c == '\r' || c == '\n') {
        Serial.write('\r');
        Serial.write('\n');
    } else {
        Serial.write((uint8_t)c);
    }
    return c;
}

static FILE uartStream;

void serialCommandInit(unsigned long baudRate) {
    Serial.begin(baudRate);

    fdev_setup_stream(&uartStream, uartPutChar, uartGetChar, _FDEV_SETUP_RW);
    stdout = &uartStream;
    stdin  = &uartStream;

    printf("System initialized.\n");
    printf("Available commands:\n");
    printf("  led on  - Turn LED on\n");
    printf("  led off - Turn LED off\n");
}

int serialCommandRead(char *buffer, uint8_t maxLength) {
    printf("> ");
    if (scanf(" %31[^\r\n]", buffer) == 1) {
        return (int)strlen(buffer);
    }
    buffer[0] = '\0';
    return 0;
}

static void toLowerStr(char *str) {
    for (uint8_t i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void serialCommandProcess(const char *command) {
    char cmd[32];
    strncpy(cmd, command, sizeof(cmd) - 1);
    cmd[sizeof(cmd) - 1] = '\0';
    toLowerStr(cmd);

    if (strcmp(cmd, "led on") == 0) {
        ledOn();
        printf("LED is now ON.\n");
    } else if (strcmp(cmd, "led off") == 0) {
        ledOff();
        printf("LED is now OFF.\n");
    } else if (strlen(cmd) > 0) {
        printf("Unknown command: \"%s\"\n", command);
        printf("Use: led on / led off\n");
    }
}
