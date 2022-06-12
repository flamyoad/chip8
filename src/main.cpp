#include <iostream>
#include <string>
#include <assert.h>
#include "chip8.cpp"
 
int main(int argc, char** argv) {
    Chip8 *chip8 = new Chip8();
    chip8->init();

    return 0;
}

