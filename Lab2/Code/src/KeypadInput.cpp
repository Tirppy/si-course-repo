#include "KeypadInput.h"

#include "Config.h"

char keys[KeypadRows][KeypadCols] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

KeypadInput::KeypadInput() {
    keypad = new Keypad(makeKeymap(keys), KeypadRowPins, KeypadColPins, KeypadRows, KeypadCols);
}

KeypadInput::~KeypadInput() {
    delete keypad;
}

char KeypadInput::getKey() {
    return keypad->getKey();
}
