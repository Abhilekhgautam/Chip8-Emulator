#include "../include/chip8.h"
#include "../include/engine.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Invalid Arguments\n Usage: <program> <filepath>";
    std::exit(1);
  }

  Engine engine(800, 400);
  Chip8 chip8;
  chip8.loadMem(argv[1]);
  engine.start(chip8);

}
