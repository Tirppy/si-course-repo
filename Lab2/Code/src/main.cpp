#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#include "Config.h"
#include "LedControl.h"
#include "LcdDisplay.h"
#include "KeypadInput.h"
#include "StdioSerial.h"

LedControl ledControl(GreenLedPin, RedLedPin);
LcdDisplay lcdDisplay;
KeypadInput keypadInput;

char enteredCode[CodeLength + 1];
char runtimeAccessCode[CodeLength + 1];
uint8_t currentIndex = 0;

enum class InputMode {
    EnterAccessCode,
    VerifyCurrentCode,
    EnterNewCode
};

InputMode inputMode = InputMode::EnterAccessCode;

void clearInputBuffer() {
    memset(enteredCode, 0, sizeof(enteredCode));
    currentIndex = 0;
}

void showPromptForMode() {
    if (inputMode == InputMode::EnterAccessCode) {
        lcdDisplay.printMessage("Enter 4-digit", "code:");
        return;
    }

    if (inputMode == InputMode::VerifyCurrentCode) {
        lcdDisplay.printMessage("Old code:", "");
        return;
    }

    lcdDisplay.printMessage("New code:", "");
}

void resetCodeInput() {
    clearInputBuffer();
    showPromptForMode();
}

void updateCodePreview() {
    char preview[CodeLength + 1];

    for (uint8_t index = 0; index < currentIndex; index++) {
        preview[index] = '*';
    }

    preview[currentIndex] = '\0';
    if (inputMode == InputMode::EnterAccessCode) {
        lcdDisplay.printMessage("Enter 4-digit", preview);
        return;
    }

    if (inputMode == InputMode::VerifyCurrentCode) {
        lcdDisplay.printMessage("Old code:", preview);
        return;
    }

    lcdDisplay.printMessage("New code:", preview);
}

void evaluateAccessCode() {
    enteredCode[currentIndex] = '\0';

    int numericCode = 0;
    int convertedValues = 0;
    if (Serial.available() > 0) {
        convertedValues = scanf("%d", &numericCode);
    }
    bool isValidCode = strcmp(enteredCode, runtimeAccessCode) == 0;

    printf("Entered Code: %s\n", enteredCode);
    printf("Scanf parse status: %d\n", convertedValues);
    printf("Scanf parsed value: %d\n", numericCode);

    if (isValidCode) {
        printf("Access Granted\n");
        lcdDisplay.printMessage("Access", "Granted");
        ledControl.showValid();
    } else {
        printf("Access Denied\n");
        lcdDisplay.printMessage("Access", "Denied");
        ledControl.showInvalid();
    }

    delay(StatusDisplayDelayMs);
    ledControl.turnOff();
    resetCodeInput();
}

void evaluateCurrentCodeForChange() {
    enteredCode[currentIndex] = '\0';
    printf("Verify old code: %s\n", enteredCode);

    if (strcmp(enteredCode, runtimeAccessCode) == 0) {
        printf("Old code correct. Enter new code.\n");
        inputMode = InputMode::EnterNewCode;
        resetCodeInput();
        return;
    }

    printf("Old code invalid. Password unchanged.\n");
    lcdDisplay.printMessage("Old code", "invalid");
    ledControl.showInvalid();
    delay(StatusDisplayDelayMs);
    ledControl.turnOff();
    inputMode = InputMode::EnterAccessCode;
    resetCodeInput();
}

void saveNewCode() {
    enteredCode[currentIndex] = '\0';
    strcpy(runtimeAccessCode, enteredCode);

    printf("Password changed successfully.\n");
    lcdDisplay.printMessage("Password", "Updated");
    ledControl.showValid();
    delay(StatusDisplayDelayMs);
    ledControl.turnOff();

    inputMode = InputMode::EnterAccessCode;
    resetCodeInput();
}

void evaluateCodeByMode() {
    if (inputMode == InputMode::EnterAccessCode) {
        evaluateAccessCode();
        return;
    }

    if (inputMode == InputMode::VerifyCurrentCode) {
        evaluateCurrentCodeForChange();
        return;
    }

    saveNewCode();
}

void setup() {
    Serial.begin(SerialBaudRate);
    initializeStdioSerial();

    strcpy(runtimeAccessCode, ValidAccessCode);

    ledControl.initialize();
    lcdDisplay.initialize();

    resetCodeInput();
    printf("System Ready. Enter Code via Keypad.\n");
    printf("Press # to change password, * to clear input.\n");
}

void loop() {
    char key = keypadInput.getKey();

    if (key != '\0') {
        printf("Key Pressed: %c\n", key);

        if (key == '#') {
            if (inputMode == InputMode::EnterAccessCode && currentIndex == 0) {
                inputMode = InputMode::VerifyCurrentCode;
                printf("Password change mode enabled.\n");
                resetCodeInput();
                delay(KeyDebounceDelayMs);
                return;
            }

            if (inputMode != InputMode::EnterAccessCode) {
                inputMode = InputMode::EnterAccessCode;
                printf("Password change cancelled.\n");
                resetCodeInput();
                delay(KeyDebounceDelayMs);
                return;
            }
        }

        if (key == '*') {
            printf("Input cleared\n");
            resetCodeInput();
            delay(KeyDebounceDelayMs);
            return;
        }

        if (key >= '0' && key <= '9' && currentIndex < CodeLength) {
            enteredCode[currentIndex++] = key;
            updateCodePreview();

            if (currentIndex == CodeLength) {
                evaluateCodeByMode();
            }
        }

        delay(KeyDebounceDelayMs);
    }
}
