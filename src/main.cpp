#include <iostream>
#include <string>
#include <assert.h>
#include "chip8.h"
 
int main() {

    uint8_t value = 0xFF;
    uint8_t msb = value & 0x80 >> 7u;
    assert(msb == 0b1);

    return 0;
}

