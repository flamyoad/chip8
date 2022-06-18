#include "chip8.h"
#include <cstring>   

constexpr uint32_t START_ADDRESS = 0x200; // 0x000 to 0x1FF is reserved for system.
constexpr uint32_t FONTSET_START_ADDRESS = 0x50;
constexpr uint32_t FONTSET_SIZE = 80;

Chip8::Chip8() {
    // Notice that only the upper half (high nibble) has value
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
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }
}

void Chip8::init() {
    pc = START_ADDRESS;
    opcode = 0;
    index = 0;
    sp = 0;
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

void Chip8::emulate_cycle() {
    opcode = memory[pc] << 8 | memory[pc + 1];

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x000F) {
                case 0x0000: 
                    OP_00EE(); 
                    break;

                case 0x000E:
                    OP_00EE(); 
                    break;
            }
        break;

        case 0x1000:
            OP_1NNN();
            break;

        case 0x2000:
            OP_2NNN();
            break;

        case 0x3000:
            OP_3XNN();
            break;

        case 0x4000:
            OP_4XNN();
            break;

        case 0x5000:
            OP_5XY0();
            break;

        case 0x6000:
            OP_6XNN();
            break;

        case 0x7000:
            OP_7XNN();
            break;

        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    OP_8XY0(); 
                    break;
                
                case 0x0001:
                    OP_8XY1();
                    break;

                case 0x0002:
                    OP_8XY2();
                    break;

                case 0x0003:
                    OP_8XY3();
                    break;

                case 0x0004:
                    OP_8XY4();
                    break;

                case 0x0005:
                    OP_8XY5();
                    break; 

                case 0x0006:
                    OP_8XY6();
                    break;    

                case 0x0007:
                    OP_8XY7();
                    break;    

                case 0x000E:
                    OP_8XYE();
                    break;                       
            }
            break;

            case 0x9000:
                OP_9XY0();
                break;

            case 0xA000:
                OP_ANNN();
                break;

            case 0xB000:
                OP_BNNN();
                break;
            
            case 0xC000:
                OP_CXNN();
                break;

            case 0xD000:
                OP_DXYN();
                break;
            
            case 0xE000:
                switch (opcode & 0x00FF) {
                    case 0x009E:
                        OP_EX9E();
                        break;

                    case 0x0A1:
                        OP_EXA1();
                        break;
                }
            break;

            case 0xF000:
                switch (opcode & 0x00FF) {
                    case 0x0007:
                        OP_FX07();
                        break;

                    case 0x000A:
                        OP_FX0A();
                        break;

                    case 0x0015:
                        OP_FX15();
                        break;

                    case 0x0018:
                        OP_FX18();
                        break;

                    case 0x001E:
                        OP_FX1E();
                        break;

                    case 0x0029:
                        OP_FX29();
                        break;

                    case 0x0033:
                        OP_FX33();
                        break;

                    case 0x0055:
                        OP_FX55();
                        break;    

                    case 0x0065:
                        OP_FX65();
                        break;                                                                   
                }
            break;

    default:
        break;
    }

    // Increment program counter by 2
    if ((opcode & 0xF000) != 0x2000) {
        pc += 2;
    }
}

// Returns from a subroutine. 
void Chip8::OP_00EE() {
    --sp;
    pc = stack[sp];
}

// Clears the screen.
void Chip8::OP_OOE0() {
    memset(display, 0, sizeof(display));
}

// Jumps to address NNN.
void Chip8::OP_1NNN() {
    uint16_t address = opcode & 0x0FFFu; // Unsigned 0xFFF
    pc = address;
}

// Calls subroutine at NNN.
void Chip8::OP_2NNN() {
    uint16_t subroutine_address = pc & 0x0FFFu;
    stack[sp] = pc;
    ++sp;
    pc = subroutine_address;
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
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t VY = (opcode & 0x00F0u) >> 4u;

    if (registers[VX] != registers[VY]) {
        pc += 2;
    }
}

// Sets I to the address NNN.
void Chip8::OP_ANNN() {
    uint16_t address = opcode & 0x0FFFu;
    index = address;
}

// Jumps to the address NNN plus V0.
void Chip8::OP_BNNN() {
    uint16_t address = opcode & 0x0FFFu;
    pc = registers[0] + address;
}

// Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
void Chip8::OP_CXNN() {
    
}

// Draw(Vx, Vy, N)	
void Chip8::OP_DXYN() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;
	// Wrap if going beyond screen boundaries
	uint8_t x_pos = registers[Vx] % 64;
	uint8_t y_pos = registers[Vy] % 32;
	registers[0xF] = 0;
	for (unsigned int row = 0; row < height; ++row) {
		uint8_t sprite_byte = memory[index + row];
		for (unsigned int col = 0; col < 8; ++col) {
			uint8_t sprite_pixel = sprite_byte & (0x80u >> col);
			uint32_t* screen_pixel = &display[(y_pos + row) * 64 + (x_pos + col)];
			// Sprite pixel is on
			if (sprite_pixel) {
				// Screen pixel also on - collision
				if (*screen_pixel == 0xFFFFFFFF) {
					registers[0xF] = 1;
				}
				// Effectively XOR with the sprite pixel
				*screen_pixel ^= 0xFFFFFFFF;
			}
		}
	}
}

// Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block);
void Chip8::OP_EX9E() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[VX];

    if (keypad[key] == 1) {
        pc += 2;
    }
}

// Skips the next instruction if the key stored in VX is not pressed. (Usually the next instruction is a jump to skip a code block);
void Chip8::OP_EXA1() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[VX];

    if (keypad[key] != 1) {
        pc += 2;
    }
}

// Sets VX to the value of the delay timer.
void Chip8::OP_FX07() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    registers[VX] = delay_timer;
}

// A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event);
void Chip8::OP_FX0A() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;

    for (int i = 0; i < 16; ++i) {
        if (keypad[i] == 1) {
            registers[VX] = keypad[i];
            pc += 2;
        }
    }
}

// Sets the delay timer to VX.
void Chip8::OP_FX15() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    delay_timer = registers[VX];
}

// Sets the sound timer to VX.
void Chip8::OP_FX18() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    sound_timer = registers[VX];
}

// Adds VX to I. VF is not affected.
void Chip8::OP_FX1E() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    index += registers[VX];
}

// Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
// todo: check whether to use VX or registers[VX]
void Chip8::OP_FX29() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    index = memory[FONTSET_START_ADDRESS + 5 * VX];
}

// Stores the binary-coded decimal representation of VX, with the most significant of three digits 
// at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. 
// (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I,
// the tens digit at location I+1, and the ones digit at location I+2.);
void Chip8::OP_FX33() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    uint8_t value = registers[VX];


    // Least-significant bit
    memory[index + 2] = value % 10;
    value /= 10;

    // Middle
    memory[index + 1] = value % 10;
    value /= 10;

    // Most-significant-bit
    memory[index] = value; 
}

// Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 
// for each value written, but I itself is left unmodified.
void Chip8::OP_FX55() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    for (uint8_t i = 0; i <= VX; ++i) {
        memory[index + i] = registers[i];
    }
}

// Fills V0 to VX (including VX) with values from memory starting at address I. 
// The offset from I is increased by 1 for each value written, but I itself is left unmodified
void Chip8::OP_FX65() {
    uint8_t VX = (opcode & 0x0F00u) >> 8u;
    for (uint8_t i = 0; i <= VX; ++i) {
        registers[i] = memory[index + 1];
    }
}







