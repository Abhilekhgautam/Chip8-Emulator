#include <cstdint>
#include <fstream>

class Chip8{
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
  unsigned int startAddr = 0x200;
 public:
  Chip8();
  void loadMem(char const* filename);
};

Chip8::Chip8(){
  // set the program counter to startAddr
  pc = startAddr;
}

void Chip8::loadMem(char const* filename){
   // move to the end of file..
   std::ifstream insFile(filename, std::ios::binary);
   int count = 0;
   if(insFile.is_open()){
      while(insFile.good()){
	// load the instructions read to the memory..
        mem[count] = insFile.get();	      
	++count;
      }
   }
}
