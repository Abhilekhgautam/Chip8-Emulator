#include <cstdint>
#include <fstream>

const unsigned int START_ADDR = 0x200;
const unsigned int CHARACTERS_START_ADDRESS = 0x50;
const unsigned int CHARACTER_SIZE = 80;
// hex representation of 0-F (16), 5 bytes each
const unsigned int CHARS[CHARACTER_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0, 
    0x90, 0x90, 0xF0, 0x10, 0x10,                               
    0xF0, 0x80, 0xF0, 0x10, 0xF0,                               
    0xF0, 0x80, 0xF0, 0x90, 0xF0,                              
    0xF0, 0x10, 0x20, 0x40, 0x40,                               
    0xF0, 0x90, 0xF0, 0x90, 0xF0,                               
    0xF0, 0x90, 0xF0, 0x10, 0xF0,                               
    0xF0, 0x90, 0xF0, 0x90, 0x90,                               
    0xE0, 0x90, 0xE0, 0x90, 0xE0,                               
    0xF0, 0x80, 0x80, 0x80, 0xF0,                                
    0xE0, 0x90, 0x90, 0x90, 0xE0,                               
    0xF0, 0x80, 0xF0, 0x80, 0xF0,                                
    0xF0, 0x80, 0xF0, 0x80, 0x80                                

};

class Chip8 {
public:
  uint8_t registers[16];
  uint8_t mem[4096];
  uint16_t pc;
  uint8_t delayTimer;
  uint8_t soundTimer;
  uint8_t keyboard[16];
  uint16_t indexReg;
  uint8_t stackPtr;
  uint8_t stack[16];
  uint8_t video[64 * 32];
  uint16_t opcode;

public:
  Chip8();
  void loadMem(char const *filename);
};

Chip8::Chip8() {
  // set the program counter to startAddr
  pc = START_ADDR;
  //load chars in memory 
  for(int i = 0 ; i < CHARACTER_SIZE ; ++i){
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
      mem[count] = insFile.get();
      ++count;
    }
  }
  insFile.close();
}
