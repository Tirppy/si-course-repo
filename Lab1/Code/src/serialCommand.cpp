#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "serialCommand.h"
#include "led.h"

/* -------- STDIO redirect over UART -------- */

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
        // wait for data
    }
    return Serial.read();
}

static FILE uartStream;

/* -------- Public functions -------- */

void serialCommandInit(unsigned long baudRate) {
    Serial.begin(baudRate);

    /* Redirect stdin/stdout to UART via STDIO */
    fdev_setup_stream(&uartStream, uartPutChar, uartGetChar, _FDEV_SETUP_RW);
    stdout = &uartStream;
    stdin  = &uartStream;

    printf("System initialized.\n");
    printf("Available commands:\n");
    printf("  led on  - Turn LED on\n");
    printf("  led off - Turn LED off\n");
    printf("> ");
}

int serialCommandRead(char *buffer, uint8_t maxLength) {
    static uint8_t index = 0;

    while (Serial.available()) {
        char c = (char)Serial.read();

        /* Handle backspace */
        if (c == '\b' || c == 127) {
            if (index > 0) {
                index--;
                printf("\b \b");
            }
            continue;
        }

        /* End of line */
        if (c == '\r' || c == '\n') {
            buffer[index] = '\0';
            uint8_t len = index;
            index = 0;
            printf("\n");
            return len;
        }

        /* Store printable character */
        if (index < maxLength - 1 && isprint((unsigned char)c)) {
            buffer[index++] = c;
            printf("%c", c);  /* echo */
        }
    }
    return -1;  /* command not yet complete */
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
    printf("> ");
}
