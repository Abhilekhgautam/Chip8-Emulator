#include <cstdint>

class Engine{
 public:
   Engine(int screenWidth, int screenHeight);
   void handleInput(uint8_t*);
   void start(uint8_t*, uint8_t video[64][32]);
   ~Engine();
};

