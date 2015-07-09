#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  DisplayTypePAL = 0,
  DisplayTypeNTSC,
} DisplayType;

void display_init(uint16_t width, uint16_t height, DisplayType display_type, bool fullscreen);
void display_lock(void);
void display_set_pixel(uint16_t x, uint16_t y, uint8_t nes_color);
void display_update(void);
void display_unlock(void);
