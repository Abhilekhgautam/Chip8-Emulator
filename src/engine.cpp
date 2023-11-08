#include <raylib.h>
#include <cstdint>

#include "../include/engine.h"

Engine::Engine(int screenWidth, int screenHeight){
  InitWindow(screenWidth, screenHeight, "CHIP8 Emulator");	
  SetTargetFPS(60);
}

void Engine::start(uint8_t* key, uint8_t video[64][32]){
 while(!WindowShouldClose()){
   handleInput(key);	 
   BeginDrawing();
      ClearBackground(BLACK);
      for(int i = 0 ; i < 64 ; ++i){
	for(int j = 0 ; j < 32 ; ++j){
	  if(video[i][j] == 1)
	    DrawPixel(i, j, WHITE);
	}      
      }
   EndDrawing();
 }	
}

Engine::~Engine(){CloseWindow();}

void Engine::handleInput(uint8_t* key){
  if(IsKeyDown(KEY_ONE)){
    key[0] = 1;
  } else if (IsKeyDown(KEY_TWO)) {
    key[1] = 1;
  } else if(IsKeyDown(KEY_THREE)){
    key[2] = 1;	  
  } else if(IsKeyDown(KEY_C)){
    key[3] = 1;	  
  } else if(IsKeyDown(KEY_FOUR)){
    key[4] = 1;	  
  } else if(IsKeyDown(KEY_FIVE)){
    key[5] = 1;	  
  }else if(IsKeyDown(KEY_SIX)){
    key[6] = 1;	  
  }else if(IsKeyDown(KEY_D)){
    key[7] = 1;	  
  }else if(IsKeyDown(KEY_SEVEN)){
    key[8] = 1;	  
  }else if(IsKeyDown(KEY_EIGHT)){
    key[9] = 1;	  
  }else if(IsKeyDown(KEY_NINE)){
    key[10] = 1;	  
  }else if(IsKeyDown(KEY_E)){
    key[11] = 1;	  
  }else if(IsKeyDown(KEY_A)){
    key[12] = 1;	  
  }else if(IsKeyDown(KEY_ZERO)){
    key[13] = 1;	  
  }else if(IsKeyDown(KEY_B)){
    key[14] = 1;	  
  }else if(IsKeyDown(KEY_F)){
    key[15] = 1;	  
  }
  if(IsKeyUp(KEY_ONE)){
    key[0] = 0;
  } else if (IsKeyUp(KEY_TWO)) {
    key[1] = 0;
  } else if(IsKeyUp(KEY_THREE)){
    key[2] = 0;	  
  } else if(IsKeyUp(KEY_C)){
    key[3] = 0;	  
  } else if(IsKeyUp(KEY_FOUR)){
    key[4] = 0;	  
  } else if(IsKeyUp(KEY_FIVE)){
    key[5] = 0;	  
  }else if(IsKeyUp(KEY_SIX)){
    key[6] = 0;	  
  }else if(IsKeyUp(KEY_D)){
    key[7] = 0;	  
  }else if(IsKeyUp(KEY_SEVEN)){
    key[8] = 0;	  
  }else if(IsKeyUp(KEY_EIGHT)){
    key[9] = 0;	  
  }else if(IsKeyUp(KEY_NINE)){
    key[10] = 0;	  
  }else if(IsKeyUp(KEY_E)){
    key[11] = 0;	  
  }else if(IsKeyUp(KEY_A)){
    key[12] = 0;	  
  }else if(IsKeyUp(KEY_ZERO)){
    key[13] = 0;	  
  }else if(IsKeyUp(KEY_B)){
    key[14] = 0;	  
  }else if(IsKeyUp(KEY_F)){
    key[15] = 0;	  
  }
}
