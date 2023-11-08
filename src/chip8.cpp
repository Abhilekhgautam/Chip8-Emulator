#include <cstring>
#include <fstream>
#include <iostream>
#include <random>

#include "../include/chip8.h"

const unsigned int START_ADDR = 0x200;
const unsigned int CHARACTERS_START_ADDRESS = 0x50;
const unsigned int CHARACTER_SIZE = 80;
// hex representation of 0-F (16), 5 bytes each
const unsigned int CHARS[CHARACTER_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, 0x20, 0x60, 0x20, 0x20, 0x70, 0xF0, 0x10,
    0xF0, 0x80, 0xF0, 0xF0, 0x10, 0xF0, 0x10, 0xF0, 0x90, 0x90, 0xF0, 0x10,
    0x10, 0xF0, 0x80, 0xF0, 0x10, 0xF0, 0xF0, 0x80, 0xF0, 0x90, 0xF0, 0xF0,
    0x10, 0x20, 0x40, 0x40, 0xF0, 0x90, 0xF0, 0x90, 0xF0, 0xF0, 0x90, 0xF0,
    0x10, 0xF0, 0xF0, 0x90, 0xF0, 0x90, 0x90, 0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0, 0xE0, 0x90, 0x90, 0x90, 0xE0, 0xF0, 0x80,
    0xF0, 0x80, 0xF0, 0xF0, 0x80, 0xF0, 0x80, 0x80

};

Chip8::Chip8() {
  // set the program counter to startAddr
  pc = START_ADDR;
  // load chars in memory
  for (int i = 0; i < CHARACTER_SIZE; ++i) {
    mem[CHARACTERS_START_ADDRESS] = CHARS[i];
  }
}

void Chip8::loadMem(char const *filename) {
  // move to the end of file..
  std::ifstream insFile(filename, std::ios::binary);
  int count = 0;
  if (insFile.is_open()) {
    while (insFile.good()) {
      // load the character  read to the memory..
      mem[START_ADDR + count] = insFile.get();
      ++count;
    }
  }
  insFile.close();
}

void Chip8::insCycle() {
  // Fetch Instruction:
  //  Instruction are 2bytes; so fetch from pc & pc + 1 and embed into one
  opcode = mem[pc] << 8u | mem[pc + 1];

  // increment the pc by 2
  pc = pc + 2;
  // Decode and Execute
  if (opcode == 0x00E0) {
    op00E0();
    return;
  } else if (opcode == 0x00EE) {
    op00E0();
    return;
  }

  // extract every nibble from the opcode
  auto rightmost = opcode & (0x0001);
  auto right = (opcode & (0x0010)) >> 4u;
  auto left = (opcode & (0x0100)) >> 8u;
  auto leftmost = (opcode & (0x1000)) >> 12u;

  if (leftmost == 1) {
    op1nnn();
    return;
  } else if (leftmost == 2) {
    op2nnn();
    return;
  } else if (leftmost == 3) {
    op3xkk();
    return;
  } else if (leftmost == 4) {
    op4xkk();
    return;
  } else if (leftmost == 5) {
    op5xy0();
    return;
  } else if (leftmost == 6) {
    op6xkk();
    return;
  } else if (leftmost == 7) {
    op7xkk();
    return;
  } else if (leftmost == 8) {
    if (rightmost == 0) {
      op8xy0();
      return;
    } else if (rightmost == 1) {
      op8xy1();
      return;
    } else if (rightmost == 2) {
      op8xy2();
      return;
    } else if (rightmost == 3) {
      op8xy3();
      return;
    } else if (rightmost == 4) {
      op8xy4();
      return;
    } else if (rightmost == 5) {
      op8xy5();
      return;
    } else if (rightmost == 6) {
      op8xy6();
      return;
    } else if (rightmost == 7) {
      op8xy7();
      return;
    } else if (rightmost == 14) {
      op8xyE();
      return;
    }
  } else if (leftmost == 9) {
    op9xy0();
    return;
  } else if (leftmost == 10) {
    opAnnn();
    return;
  } else if (leftmost == 11) {
    opBnnn();
    return;
  } else if (leftmost == 12) {
    opCxkk();
    return;
  } else if (leftmost == 13) {
    opDxyn();
    return;
  } else if (leftmost == 14 && rightmost == 14) {
    opEx9E();
    return;
  } else if (leftmost == 14 && rightmost == 1) {
    opExA1();
    return;
  } else if (leftmost == 15) {
    if (rightmost == 7) {
      opFx07();
      return;
    } else if (rightmost == 10) {
      opFx0A();
      return;
    } else if (rightmost == 5 && right == 1) {
      opFx15();
      return;
    } else if (rightmost == 8) {
      opFx18();
    } else if (rightmost == 14) {
      opFx1E();
    } else if (rightmost == 9) {
      opFx29();
    } else if (rightmost == 3) {
      opFx33();
    } else if (rightmost == 5 && right == 5) {
      opFx55();
    } else if (rightmost == 5 && right == 6) {
      opFx65();
    }
  }
}

// CLS: clears the display
void Chip8::op00E0() { memset(video, 0, sizeof(video)); }
// RET: Return from a subroutine
void Chip8::op00EE() {
  --stackPtr;
  pc = stack[stackPtr];
}
// Jump to location nnn
void Chip8::op1nnn() {
  auto address = opcode & (0x0FFF);
  pc = address;
}
// CALL
void Chip8::op2nnn() {
  auto address = opcode & (0xFFF);
  ++stackPtr;
  stack[stackPtr] = pc;
  pc = address;
}

void Chip8::op3xkk() {
  uint8_t Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t byte = opcode & (0x00FF);

  if (registers[Vx] == byte) {
    pc += 2;
  }
}

void Chip8::op4xkk() {
  uint8_t Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t byte = opcode & (0x00FF);

  if (registers[Vx] != byte) {
    pc += 2;
  }
}

void Chip8::op5xy0() {
  uint8_t Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t Vy = (opcode & (0x00F0)) >> 4u;

  if (registers[Vx] == registers[Vy]) {
    pc += 2;
  }
}

void Chip8::op6xkk() {
  uint8_t Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t byte = opcode & (0x00FF);

  registers[Vx] = byte;
}

void Chip8::op7xkk() {
  uint8_t Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t byte = opcode & (0x00FF);

  registers[Vx] += byte;
}

void Chip8::op8xy0() {
  uint8_t Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t Vy = (opcode & (0x00F0)) >> 4u;

  registers[Vx] = registers[Vy];
}
void Chip8::op8xy1() {
  uint8_t Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t Vy = (opcode & (0x00F0)) >> 4u;

  registers[Vx] |= registers[Vy];
}

void Chip8::op8xy2() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t y = (opcode & (0x00F0)) >> 4u;

  registers[x] &= registers[y];
}

void Chip8::op8xy3() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t y = (opcode & (0x00F0)) >> 4u;

  registers[x] = registers[x] xor registers[y];
}

void Chip8::op8xy4() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t y = (opcode & (0x00F0)) >> 4u;

  int temp = registers[x] + registers[y];
  if (temp > 255) {
    VF = 1;
  } else
    VF = 0;
  registers[x] = temp % 256;
}
// todo: ambigious reference
void Chip8::op8xy5() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t y = (opcode & (0x00F0)) >> 4u;

  if (registers[x] > registers[y]) {
    VF = 1;
  } else
    VF = 0;

  registers[x] = registers[y] - registers[x];
}
// buggy: the Chip8 ref seems to be ambiguous.
void Chip8::op8xy6() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  // uint8_t y = (opcode & (0x00F0)) >> 4u;

  if ((registers[x] & 0x0001)) {
    VF = 1;
  } else
    VF = 0;
  registers[x] = registers[x] / 2;
}

void Chip8::op8xy7() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t y = (opcode & (0x00F0)) >> 4u;

  if (registers[y] > registers[x])
    VF = 1;
  else
    VF = 0;

  registers[x] = registers[x] - registers[y];
}

// buggy: the Chip8 ref seems to be ambigious.
void Chip8::op8xyE() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  // uint8_t y = (opcode & (0x00F0)) >> 4u;

  if (((registers[x] & 0x0001)) >> 8u) {
    VF = 1;
  }
  registers[x] = registers[x] * 2;
}

void Chip8::op9xy0() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t y = (opcode & (0x00F0)) >> 4u;

  if (registers[x] != registers[y])
    pc += 2;
}

void Chip8::opAnnn() {
  uint16_t addr = opcode & 0x0FFF;

  indexReg = addr;
}

void Chip8::opBnnn() {
  uint16_t addr = opcode & 0x0FFF;

  pc = addr + registers[0];
}

void Chip8::opCxkk() {
  std::random_device rd;  // a seed source for the random number engine
  std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib(0, 255);

  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t kk = (opcode & (0x00FF));
  registers[x] = distrib(gen) & kk;
}

void Chip8::opDxyn() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t y = (opcode & (0x00F0)) >> 4u;
  uint8_t n = opcode & (0x000F);

  for (auto i = indexReg; i <= indexReg + n; ++i) {
    auto val = mem[i];
    auto x_coordinate = registers[x];
    auto y_coordinate = registers[y];
    if (x_coordinate > 64)
      x_coordinate = x_coordinate % 64;
    if (y_coordinate > 32)
      y_coordinate = y_coordinate % 32;

    video[x_coordinate][y_coordinate] = val;
  }
}
void Chip8::opEx9E() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t val = registers[x];
  bool skip = false;
  switch (val) {
  case 1:
    if (keyboard[1] == 1)
      skip = true;
    break;

  case 2:
    if (keyboard[2] == 1)
      skip = true;
    break;

  case 3:
    if (keyboard[3] == 1)
      skip = true;
    break;

  case 12:
    if (keyboard[4] == 1)
      skip = true;
    break;
  case 4:
    if (keyboard[5] == 1)
      skip = true;
    break;
  case 5:
    if (keyboard[6] == 1)
      skip = true;
    break;
  case 6:
    if (keyboard[7] == 1)
      skip = true;
    break;
  case 13:
    if (keyboard[8] == 1)
      skip = true;
    break;
  case 7:
    if (keyboard[9] == 1)
      skip = true;
    break;
  case 8:
    if (keyboard[10] == 1)
      skip = true;
    break;
  case 9:
    if (keyboard[11] == 1)
      skip = true;
    break;
  case 10:
    if (keyboard[12] == 1)
      skip = true;
    break;
  case 0:
    if (keyboard[13] == 1)
      skip = true;
    break;
  case 11:
    if (keyboard[14] == 1)
      skip = true;
    break;
  case 15:
    if (keyboard[15] == 1)
      skip = true;
    break;
  default:
    std::cout << "Shouldn't reach here";
  }
  if (skip)
    pc += 2;
}
void Chip8::opExA1() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t val = registers[x];
  bool skip = false;
  switch (val) {
  case 1:
    if (keyboard[1] == 1)
      skip = true;
    break;

  case 2:
    if (keyboard[2] == 1)
      skip = true;
    break;

  case 3:
    if (keyboard[3] == 1)
      skip = true;
    break;

  case 12:
    if (keyboard[4] == 1)
      skip = true;
    break;
  case 4:
    if (keyboard[5] == 1)
      skip = true;
    break;
  case 5:
    if (keyboard[6] == 1)
      skip = true;
    break;
  case 6:
    if (keyboard[7] == 1)
      skip = true;
    break;
  case 13:
    if (keyboard[8] == 1)
      skip = true;
    break;
  case 7:
    if (keyboard[9] == 1)
      skip = true;
    break;
  case 8:
    if (keyboard[10] == 1)
      skip = true;
    break;
  case 9:
    if (keyboard[11] == 1)
      skip = true;
    break;
  case 10:
    if (keyboard[12] == 1)
      skip = true;
    break;
  case 0:
    if (keyboard[13] == 1)
      skip = true;
    break;
  case 11:
    if (keyboard[14] == 1)
      skip = true;
    break;
  case 15:
    if (keyboard[15] == 1)
      skip = true;
    break;
  default:
    std::cout << "Shouldn't reach here";
  }
  if (!skip)
    pc += 2;
}

void Chip8::opFx07() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  registers[x] = delayTimer;
}

void Chip8::opFx0A() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  bool stop = false;
  while (!stop) {
    for (auto i = 0; i < 16; ++i) {
      if (keyboard[i] == 1) {
        stop = true;
        registers[x] = keyboard[i];
        break;
      }
    }
  }
}

void Chip8::opFx15() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  delayTimer = registers[x];
}

void Chip8::opFx18() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  soundTimer = registers[x];
}

void Chip8::opFx1E() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  indexReg = indexReg + registers[x];
}

void Chip8::opFx29() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t val = registers[x];

  indexReg = CHARACTERS_START_ADDRESS + 5 * val;
}

void Chip8::opFx33() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t val = registers[x];

  // TODO: use a loop
  mem[indexReg + 2] = val % 10;
  auto temp = val / 10;
  mem[indexReg + 1] = val % 10;
  temp = val / 10;
  mem[indexReg] = val % 10;
}

void Chip8::opFx55() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;

  for (int i = 0; i <= x; ++i) {
    mem[indexReg + i] = registers[i];
  }
}

void Chip8::opFx65() {
  uint8_t x = (opcode & (0x0F00)) >> 8u;

  for (int i = 0; i <= x; ++i) {
    registers[i] = mem[indexReg + i];
  }
}
