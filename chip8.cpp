#include "chip8.h"

constexpr uint32_t START_ADDRESS = 0x200; // 0x000 to 0x1FF is reserved for system.
constexpr uint32_t FONTSET_SIZE = 80;

Chip8::Chip8() {
    pc = START_ADDRESS; // Initialize program counter to 0x200.

    uint8_t fontset[FONTSET_SIZE] = {
	    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	    0x20, 0x60, 0x20, 0x20, 0x70, // 1
	    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // Load the font into memory
    for (int i = 0; i < FONTSET_SIZE; ++i)  {
        memory[i] = fontset[i];
    }
}

void Chip8::load_rom(char const *file_path) {
    std::ifstream file(file_path, std::ios::binary);

    if (file.is_open()) {
        // Get size of ROM file
        file.seekg(0, file.end);
        auto rom_size = file.tellg();
        char *buffer = new char[rom_size];

        // Fill the buffer with ROM data
        file.seekg(0, file.beg);
        file.read(buffer, rom_size);
        file.close();

        // Load ROM content into Chip8 memory, starting from 0x200
        for (int i = 0; i < rom_size; ++i) {
            memory[START_ADDRESS + 1] = buffer[i];
        }

        delete[] buffer;
    }
}

// Clears the screen.
void Chip8::OP_00EE() {
    memset(display, 0, sizeof(display));
}

// Returns from a subroutine. 
void Chip8::OP_OOE0() {
    --sp;
    pc = stack[sp];
}

// Jumps to address NNN.
void Chip8::OP_1NNN() {
    uint16_t address = opcode & 0x0FFFu; // Unsigned 0xFFF
    pc = address;
}

// Calls subroutine at NNN.
void Chip8::OP_2NNN() {
    uint16_t subroutineAddress = pc & 0x0FFFu;
    stack[sp] = pc;
    ++sp;
    pc = subroutineAddress;
}

// Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block);
void Chip8::OP_3XNN() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u; // Left shift (Example: 0xF00 => 0x00F)
    uint8_t NN = opcode & 0x00FFu;
    if (registers[VX] == NN) {
        pc += 2;
    }
}

// Skips the next instruction if VX does not equal NN. (Usually the next instruction is a jump to skip a code block);
void Chip8::OP_4XNN() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t NN = opcode & 0x00FFu;
    if (registers[VX] != NN) {
        pc += 2;
    }
}

// Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block);
void Chip8::OP_5XY0() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t VY = (opcode & 0x00F0u) >> 4u;
    if (registers[VX] == registers[VY]) {
        pc += 2;
    }
}

// Sets VX to NN.
void Chip8::OP_6XNN() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t NN = (opcode & 0x00FFu);
    registers[VX] = NN;
}

// Adds NN to VX. (Carry flag is not changed);
void Chip8::OP_7XNN() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t NN = opcode & 0x00FFu;
    registers[VX] += NN;
}

// Sets VX to the value of VY.
void Chip8::OP_8XY0() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t VY = (opcode & 0x00F0u) >> 4u;
    registers[VX] = registers[VY];
}

// Sets VX to VX or VY. (Bitwise OR operation);
void Chip8::OP_8XY1() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t VY = (opcode & 0x00F0u) >> 4u;
    registers[VX] |= registers[VY];
}

// Sets VX to VX and VY. (Bitwise AND operation);
void Chip8::OP_8XY2() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t VY = (opcode & 0x00F0u) >> 4u;
    registers[VX] &= registers[VY];
}

// Sets VX to VX xor VY.
void Chip8::OP_8XY3() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t VY = (opcode & 0x00F0u) >> 4u;
    registers[VX] ^= registers[VY];
}

// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there is not.
void Chip8::OP_8XY4() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t VY = (opcode & 0x00F0u) >> 4u;

    uint16_t sum = registers[VX] + registers[VY];
    if (sum > 255u) {
        registers[0xF] = 1;
    } else {
        registers[0xF] = 0;
    }

    registers[VX] = sum & 0xFFu;
}

// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not. VX -= VY
void Chip8::OP_8XY5() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t VY = (opcode & 0x00F0u) >> 4u;

    if (registers[VX] > registers[VY]) {
        registers[0xF] = 1;
    } else {
        registers[0xF] = 0;
    }

    registers[VX] -= registers[VY];
}

// Stores the least significant bit of VX in VF and then shifts VX to the right by 1
void Chip8::OP_8XY6() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;

    // Example: 0000 1101 & 0000 0001 = 0000 0001
    registers[0xF] = registers[VX] & 0x1u;
    registers[VX] >>= 1;
}

// Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not.
void Chip8::OP_8XY7() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t VY = (opcode & 0x00F0u) >> 4u;

    if (registers[VX] > registers[VY]) {
        registers[0xF] = 0;
    } else {
        registers[0xF] = 1;
    }
    registers[VX] = registers[VY] - registers[VX];
}

// Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
// todo: Write unit test for this
void Chip8::OP_8XYE() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;

    registers[0xF] = (registers[VX] & 0x80) >> 7u;
    registers[VX] <<= 1;
}

// Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block);
void Chip8::OP_9XY0() {

}








