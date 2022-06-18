#include <cstdint>
#include <random>
#include <fstream>

class Chip8 {
    public:
        uint8_t memory[4096]; // 4K memory (1 byte = 8 bit)
        uint8_t registers[16]; // V0 to VF
        uint16_t index; // Index register
        uint16_t pc; // Program counter
        uint16_t stack[16]; // 16-level stack
        uint8_t sp; // Stack pointer
        uint8_t delay_timer;
        uint8_t sound_timer;

        // Keypad (16 keys ranging from 0 to F)
        uint8_t keypad[16];

        // Display
        uint32_t display[64*32];

        // 16bit opcode (First instruction in 1byte + Second instruction in 1byte)
        uint16_t opcode;

        std::default_random_engine randGen;
        std::uniform_int_distribution<uint8_t> randByte;

        Chip8();
        void init();
        void load_rom(char const *file_path);
        void emulate_cycle();

        // Opcodes (35 total)
        void OP_OOE0();
        void OP_00EE();
        void OP_1NNN();
        void OP_2NNN();
        void OP_3XNN();
        void OP_4XNN();
        void OP_5XY0();
        void OP_6XNN();
        void OP_7XNN();
        void OP_8XY0();
        void OP_8XY1();
        void OP_8XY2();
        void OP_8XY3();
        void OP_8XY4();
        void OP_8XY5();
        void OP_8XY6();
        void OP_8XY7();
        void OP_8XYE();
        void OP_9XY0();
        void OP_ANNN();
        void OP_BNNN();
        void OP_CXNN();
        void OP_DXYN();
        void OP_EX9E();
        void OP_EXA1();
        void OP_FX07();
        void OP_FX0A();
        void OP_FX15();
        void OP_FX18();
        void OP_FX1E();
        void OP_FX29();
        void OP_FX33();
        void OP_FX55();
        void OP_FX65();
};