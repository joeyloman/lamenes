#include "system/buttons.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "lame6502/lame6502.h"
#include "lame6502/debugger.h"

#include "lamenes.h"
#include "input.h"

void poll_buttons(void) {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
        case SDLK_DOWN:
        #ifdef PAD_DEBUG
        printf("debug [%d]: DOWN key (down) press detected\n",debug_cnt);
        #endif
        set_input(1);
        break;

        case SDLK_UP:
        #ifdef PAD_DEBUG
        printf("debug [%d]: UP key press (down) detected\n",debug_cnt);
        #endif
        set_input(2);
        break;

        case SDLK_LEFT:
        #ifdef PAD_DEBUG
        printf("debug [%d]: LEFT key press (down) detected\n",debug_cnt);
        #endif
        set_input(3);
        break;

        case SDLK_RIGHT:
        #ifdef PAD_DEBUG
        printf("debug [%d]: RIGHT key press (down) detected\n",debug_cnt);
        #endif
        set_input(4);
        break;

        case SDLK_LCTRL:
        #ifdef PAD_DEBUG
        printf("debug [%d]: LCTRL (NES -> start) key (down) press detected\n",debug_cnt);
        #endif
        set_input(5);
        break;

        case SDLK_LSHIFT:
        #ifdef PAD_DEBUG
        printf("debug [%d]: LSHIFT (NES -> select) key (down) press detected\n",debug_cnt);
        #endif
        set_input(6);
        break;

        case SDLK_p:
        if(pause_emulation) {
          printf("[*] LameNES continue emulation!\n");
          CPU_is_running = 1;
          pause_emulation = 0;
        } else if(!pause_emulation) {
          printf("[*] LameNES paused!\n");
          CPU_is_running = 0;
          pause_emulation = 1;
        }
        break;

        case SDLK_x:
        #ifdef PAD_DEBUG
        printf("debug [%d]: Z (NES -> A) key (down) press detected\n",debug_cnt);
        #endif
        set_input(7);
        break;

        case SDLK_z:
        #ifdef PAD_DEBUG
        printf("debug [%d]: X (NES -> B) key (down) press detected\n",debug_cnt);
        #endif
        set_input(8);
        break;

        case SDLK_q:
        #ifdef PAD_DEBUG
        printf("debug [%d]: Q (quit lamenes) key (down) press detected\n",debug_cnt);
        #endif
        quit_emulation();
        break;

        case SDLK_ESCAPE:
        #ifdef PAD_DEBUG
        printf("debug [%d]: ESC (quit lamenes) key (down) press detected\n",debug_cnt);
        #endif
        quit_emulation();
        break;

        case SDLK_F1:
        /* reset */
        reset_emulation();
        break;

        case SDLK_F3:
        /* load state */
        load_state();
        break;

        case SDLK_F6:
        /* save state */
        save_state();
        break;

        case SDLK_F10:
        if(enable_background == 1) {
          enable_background = 0;
        } else {
          enable_background = 1;
        }
        break;

        case SDLK_F11:
        if(enable_sprites == 1) {
          enable_sprites = 0;
        } else {
          enable_sprites = 1;
        }
        break;

        case SDLK_F12:
        if(startdebugger > 0) {
          disassemble = 1;
          hit_break = 1;
          debugger();
        }
        break;

        default:
        break;
      }
    }

    if(event.type == SDL_KEYUP) {
      switch(event.key.keysym.sym){
        case SDLK_DOWN:
        #ifdef PAD_DEBUG
        printf("debug [%d]: DOWN key (up) press detected\n",debug_cnt);
        #endif
        clear_input(1);
        break;

        case SDLK_UP:
        #ifdef PAD_DEBUG
        printf("debug [%d]: UP key (up) press detected\n",debug_cnt);
        #endif
        clear_input(2);
        break;

        case SDLK_LEFT:
        #ifdef PAD_DEBUG
        printf("debug [%d]: LEFT key (up) press detected\n",debug_cnt);
        #endif
        clear_input(3);
        break;

        case SDLK_RIGHT:
        #ifdef PAD_DEBUG
        printf("debug [%d]: RIGHT key (up) press detected\n",debug_cnt);
        #endif
        clear_input(4);
        break;

        case SDLK_LCTRL:
        #ifdef PAD_DEBUG
        printf("debug [%d]: LCTRL (NES -> start) key (up) press detected\n",debug_cnt);
        #endif
        clear_input(5);
        break;

        case SDLK_LSHIFT:
        #ifdef PAD_DEBUG
        printf("debug [%d]: LSHIFT (NES -> select) key (up) press detected\n",debug_cnt);
        #endif
        clear_input(6);
        break;

        case SDLK_x:
        #ifdef PAD_DEBUG
        printf("debug [%d]: Z (NES -> A) key (up) press detected\n",debug_cnt);
        #endif
        clear_input(7);
        break;

        case SDLK_z:
        #ifdef PAD_DEBUG
        printf("debug [%d]: X (NES -> B) key (up) press detected\n",debug_cnt);
        #endif
        clear_input(8);
        break;

        default:
        break;
      }
    }
  }
}
