#include <cstdint>
#include <random>
#include <iostream>
#include <cstring>
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
  uint8_t video[64][32];
  uint16_t opcode;

  uint8_t VF;
public:
  Chip8();
  void loadMem(char const *filename);
  void op00E0();
  void op00EE();
  void op1nnn();
  void op2nnn();
  void op3xkk();
  void op4xkk();
  void op5xy0();
  void op6xkk();
  void op7xkk();
  void op8xy0();
  void op8xy1();
  void op8xy2();
  void op8xy3();
  void op8xy4();
  void op8xy5();
  void op8xy6();
  void op8xy7();
  void op8xyE();
  void op9xy0();
  void opAnnn();
  void opBnnn();
  void opCxkk();
  void opDxyn();
  void opEx9E();
  void opExA1();
  void opFx07();
  void opFx0A();
  void opFx15();
  void opFx18();
  void opFx1E();
  void opFx29();
  void opFx33();
  void opFx55();
  void opFx65();
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
      mem[START_ADDR + count] = insFile.get();
      ++count;
    }
  }
  insFile.close();
}
// CLS: clears the display
void Chip8::op00E0(){
 memset(video, 0, sizeof(video));	
}
// RET: Return from a subroutine
void Chip8::op00EE(){
 --stackPtr;
 pc = stack[stackPtr];
}
// Jump to location nnn
void Chip8::op1nnn(){
  auto address = opcode & (0x0FFF);
  pc = address;
}
// CALL
void Chip8::op2nnn(){
  auto address = opcode & (0xFFF);
  ++stackPtr;
  stack[stackPtr] = pc;
  pc = address;
}

void Chip8::op3xkk(){
  uint8_t  Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t byte = opcode & (0x00FF);

  if(registers[Vx] == byte){
    pc += 2;	  
  }
}

void Chip8::op4xkk(){
  uint8_t Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t byte = opcode & (0x00FF);

  if(registers[Vx] != byte){
    pc += 2;
  }
}

void Chip8::op5xy0(){
  uint8_t Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t Vy = (opcode & (0x00F0)) >> 4u;

  if(registers[Vx] == registers[Vy]){
   pc += 2;	  
  }
}

void Chip8::op6xkk(){
  uint8_t Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t byte = opcode & (0x00FF);

  registers[Vx] = byte;
}

void Chip8::op7xkk(){
  uint8_t Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t byte = opcode & (0x00FF);

  registers[Vx] += byte;
}

void Chip8::op8xy0(){
  uint8_t Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t Vy = (opcode & (0x00F0)) >> 4u;

  registers[Vx] = registers[Vy];
}
void Chip8::op8xy1(){
  uint8_t Vx = (opcode & (0x0F00)) >> 8u;
  uint8_t Vy = (opcode & (0x00F0)) >> 4u;

  registers[Vx] |= registers[Vy];
}

void Chip8::op8xy2(){
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t y = (opcode & (0x00F0)) >> 4u;

  registers[x]  &=   registers[y];
}

void Chip8::op8xy3(){
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t y = (opcode & (0x00F0)) >> 4u;

  registers[x] =  registers[x] xor registers[y];
}

void Chip8::op8xy4(){
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t y = (opcode & (0x00F0)) >> 4u;

  int temp = registers[x] + registers[y];
  if(temp > 255){
    VF = 1;	  
  }
  registers[x] = temp % 256;

}
// todo: ambigious reference
void Chip8::op8xy5(){
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t y = (opcode & (0x00F0)) >> 4u;

  int temp = registers[x] + registers[y];
  if(temp > 255){
    VF = 1;	  
  }
  registers[x] = temp % 256;
}
// buggy: the Chip8 ref seems to be ambigious.
void Chip8::op8xy6(){
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  //uint8_t y = (opcode & (0x00F0)) >> 4u;

  if((registers[x] & 0x0001)){
   VF = 1;
  }
   registers[x] = registers[x] / 2;
}

void Chip8::op8xy7(){
 //todo	
}

// buggy: the Chip8 ref seems to be ambigious.
void Chip8::op8xyE(){
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  //uint8_t y = (opcode & (0x00F0)) >> 4u;

  if(((registers[x] & 0x0001)) >> 8u){
   VF = 1;
  }
   registers[x] = registers[x] *  2;
}

void Chip8::op9xy0(){
   uint8_t x = (opcode & (0x0F00)) >> 8u;
   uint8_t y = (opcode & (0x00F0)) >> 4u;
 
   if(registers[x] != registers[y]) pc += 2;

}

void Chip8::opAnnn(){
  uint16_t addr = opcode & 0x0FFF;

  indexReg = addr;
}

void Chip8::opBnnn(){
   uint16_t addr = opcode & 0x0FFF;
 
   pc = addr + registers[0];

}

void Chip8::opCxkk(){
   std::random_device rd;  // a seed source for the random number engine
   std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
   std::uniform_int_distribution<> distrib(0, 255);
  
   uint8_t x = (opcode & (0x0F00)) >> 8u;
   uint8_t kk = (opcode & (0x00FF));
   registers[x] = distrib(gen) &  kk;
  
}

void Chip8::opDxyn(){
   uint8_t x = (opcode & (0x0F00)) >> 8u;
   uint8_t y = (opcode & (0x00F0)) >> 4u;
   uint8_t n = opcode & (0x000F);

   for(auto i = indexReg ; i <= indexReg + n; ++i){
    auto val = mem[i];
    auto x_coordinate = registers[x];
    auto y_coordinate = registers[y];
    if(x_coordinate > 64) x_coordinate = x_coordinate % 64;
    if(y_coordinate > 32) y_coordinate = y_coordinate % 32;

    video[x_coordinate][y_coordinate] = val;
   }

}
void Chip8::opEx9E(){
  uint8_t x = (opcode & (0x0F00)) >> 8u;
   uint8_t val = registers[x];
   bool skip = false;
   switch (val) {
     case 1:
	if(keyboard[1] == 1) skip = true;
	break;
     
     case 2:
	if(keyboard[2] == 1) skip = true;
	break;
 
     case 3:
	if(keyboard[3] == 1) skip = true;
	break;
     
     case 12:
	if(keyboard[4] == 1) skip = true;
	break;
     case 4:
	if(keyboard[5] == 1) skip = true;
	break;
     case 5:
	if(keyboard[6] == 1) skip = true;
	break;
     case 6:
        if(keyboard[7] == 1) skip = true;
	break;
     case 13:
	if(keyboard[8] == 1) skip = true;
	break;
     case 7:
	if(keyboard[9] == 1) skip = true;
	break;
     case 8:
	if(keyboard[10] == 1) skip = true;
	break;
     case 9:
	if(keyboard[11] == 1) skip = true;
	break;
     case 10:
	if(keyboard[12] == 1) skip = true;
	break;
     case 0:
	if(keyboard[13] == 1) skip = true;
	break;
     case 11:
	if(keyboard[14] == 1) skip = true;
	break;
     case 15:
	if(keyboard[15] == 1) skip = true;
	break;
     default:
	std::cout << "Shouldn't reach here";
   }
   if(skip) pc += 2;
}
void Chip8::opExA1(){
   uint8_t x = (opcode & (0x0F00)) >> 8u;
   uint8_t val = registers[x];
   bool skip = false;
   switch (val) {
     case 1:
	if(keyboard[1] == 1) skip = true;
	break;
     
     case 2:
	if(keyboard[2] == 1) skip = true;
	break;
 
     case 3:
	if(keyboard[3] == 1) skip = true;
	break;
     
     case 12:
	if(keyboard[4] == 1) skip = true;
	break;
     case 4:
	if(keyboard[5] == 1) skip = true;
	break;
     case 5:
	if(keyboard[6] == 1) skip = true;
	break;
     case 6:
        if(keyboard[7] == 1) skip = true;
	break;
     case 13:
	if(keyboard[8] == 1) skip = true;
	break;
     case 7:
	if(keyboard[9] == 1) skip = true;
	break;
     case 8:
	if(keyboard[10] == 1) skip = true;
	break;
     case 9:
	if(keyboard[11] == 1) skip = true;
	break;
     case 10:
	if(keyboard[12] == 1) skip = true;
	break;
     case 0:
	if(keyboard[13] == 1) skip = true;
	break;
     case 11:
	if(keyboard[14] == 1) skip = true;
	break;
     case 15:
	if(keyboard[15] == 1) skip = true;
	break;
     default:
	std::cout << "Shouldn't reach here";
   }
   if(!skip) pc += 2;
}

void Chip8::opFx07(){
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  registers[x] = delayTimer;
}

void Chip8::opFx0A(){}

void Chip8::opFx15(){
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  delayTimer = registers[x];
}

void Chip8::opFx18(){
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  soundTimer = registers[x];
}

void Chip8::opFx1E(){
   uint8_t x = (opcode & (0x0F00)) >> 8u;
   indexReg  = indexReg + registers[x];
}

void Chip8::opFx29(){
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  uint8_t val = registers[x];

  indexReg = CHARACTERS_START_ADDRESS + 5 * val;
}

void Chip8::opFx33(){}

void Chip8::opFx55(){
   uint8_t x = (opcode & (0x0F00)) >> 8u;

   for(int i = 0 ; i <= x ; ++i){
     mem[indexReg + i] = registers[i];	   
   }
}

void Chip8::opFx65(){
  uint8_t x = (opcode & (0x0F00)) >> 8u;
  
  for(int i = 0 ; i <= x ; ++i){
    registers[i] = mem[indexReg + i];	  
  }
}

