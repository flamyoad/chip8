#include <assert.h>

// todo: Please find unit test framework :D
void test() {
    uint8_t value = 0xFF;
    uint8_t msb = value & 0x80 >> 7u;
    assert(msb == 0b1);
}