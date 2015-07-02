#include "system/sleep.h"

#include <stdint.h>

#include <SDL/SDL.h>

void sleep_ms(uint32_t time_ms) {
  SDL_Delay(time_ms);
}
