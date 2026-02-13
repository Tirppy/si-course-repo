/* serialCommand.cpp -- STDIO redirect + command processing */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "serialCommand.h"
#include "led.h"

/* --- STDIO redirect callbacks --- */

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
    while (!Serial.available()) {} /* wait for data */
    return Serial.read();
}

static FILE uartStream;

/* --- Public functions --- */

void serialCommandInit(unsigned long baudRate) {
    Serial.begin(baudRate);

    /* Redirect stdin/stdout to UART */
    fdev_setup_stream(&uartStream, uartPutChar, uartGetChar, _FDEV_SETUP_RW);
    stdout = &uartStream;
    stdin  = &uartStream;

    printf("System initialized.\n");
    printf("Available commands:\n");
    printf("  led on  - Turn LED on\n");
    printf("  led off - Turn LED off\n");
}

void serialCommandRead(char *buffer, uint8_t maxLength) {
    uint8_t i = 0;
    int c;

    printf("> ");

    while (i < maxLength - 1) {
        c = getchar();              /* blocking read */

        /* Handle backspace */
        if (c == '\b' || c == 127) {
            if (i > 0) {
                i--;
                printf("\b \b");    /* erase char on terminal */
            }
            continue;
        }

        /* End of line */
        if (c == '\r' || c == '\n') {
            putchar('\n');          /* echo newline */
            break;
        }

        /* Store and echo printable character */
        if (isprint(c)) {
            buffer[i++] = (char)c;
            putchar(c);             /* echo character */
        }
    }
    buffer[i] = '\0';
}

static void toLowerStr(char *str) {
    for (uint8_t i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void serialCommandProcess(const char *command) {
    char device[16] = {0};
    char action[16] = {0};

    /* Parse command with sscanf */
    int fields = sscanf(command, "%15s %15s", device, action);

    toLowerStr(device);
    if (fields >= 2) {
        toLowerStr(action);
    }

    if (fields == 2 && strcmp(device, "led") == 0) {
        if (strcmp(action, "on") == 0) {
            ledOn();
            printf("LED is now ON.\n");
        } else if (strcmp(action, "off") == 0) {
            ledOff();
            printf("LED is now OFF.\n");
        } else {
            printf("Unknown action: \"%s\"\n", action);
            printf("Use: led on / led off\n");
        }
    } else if (strlen(command) > 0) {
        printf("Unknown command: \"%s\"\n", command);
        printf("Use: led on / led off\n");
    }
}
