#include <cstdint>

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
  void insCycle();
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

