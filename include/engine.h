#include <cstdint>
#include "chip8.h"

class Engine {
public:
  Engine(int screenWidth, int screenHeight);
  void handleInput(uint8_t *);
  void start(Chip8& chip8);
  ~Engine();
};
