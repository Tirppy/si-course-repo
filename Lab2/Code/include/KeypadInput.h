#ifndef KEYPAD_INPUT_H
#define KEYPAD_INPUT_H

#include <Keypad.h>

class KeypadInput {
public:
    KeypadInput();
    ~KeypadInput();

    char getKey();

private:
    Keypad* keypad;
};

#endif
