/*
 * LameNES - Nintendo Entertainment System (NES) emulator
 *
 * Copyright (c) 2005, Joey Loman, <joey@lamenes.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "lame6502/lame6502.h"
#include "lame6502/disas.h"

#include "system/display.h"

#include "lamenes.h"
#include "ppu.h"
#include "macros.h"
#include "romloader.h"
#include "palette.h"

//#define PPU_DEBUG
//#define PPU_MEM_DEBUG
//#define PPU_SPR_MEM_DEBUG
//#define SCANLINE_DEBUG
//#define SPRITE_RENDER_DEBUG

/* gfx cache -> [hor] [ver] */
unsigned char bgcache[256+8] [256+8];
unsigned char sprcache[256+8] [256+8];

/* ppu control registers */
unsigned int ppu_control1 = 0x00;
unsigned int ppu_control2 = 0x00;

/* used to flip between first and second write (0x2006) */
unsigned int ppu_addr_h = 0x00;

unsigned int ppu_addr = 0x2000;
unsigned int ppu_addr_tmp = 0x2000;

/* used for scrolling techniques */
unsigned int loopyT = 0x00;
unsigned int loopyV = 0x00;
unsigned int loopyX = 0x00;

/* ppu status/temp registers */
unsigned int ppu_status;
unsigned int ppu_status_tmp = 0x00;

unsigned int sprite_address = 0x00;

/* used to flip between first and second write (0x2005) */
unsigned int ppu_bgscr_f = 0x00;

/* used to export the current scanline for the debugger */
int current_scanline;

void
write_ppu_memory(unsigned int address,unsigned char data)
{	
	int i;

	if(address == 0x2000) {
		ppu_addr_tmp = data;

		ppu_control1 = data;

		memory[address] = data;

		loopyT &= 0xf3ff; // (0000110000000000)
		loopyT |= (data & 3) << 10; // (00000011)

		#ifdef PPU_MEM_DEBUG
		if(debug_cnt > show_debug_cnt) {
			printf("[%d] 0x2000 = %x, loopyT: %x\n",debug_cnt,data,loopyT);
		}
		#endif

		return;
        }

	if(address == 0x2001) {
		ppu_addr_tmp = data;

		ppu_control2 = data;
		memory[address] = data;

		#ifdef PPU_MEM_DEBUG
		if(debug_cnt > show_debug_cnt) {
			printf("[%d] 0x2001 = %x, loopyT: %x\n",debug_cnt,data,loopyT);
		}
		#endif

		return;
        }

	/* sprite_memory address register */
	if(address == 0x2003) {
		ppu_addr_tmp = data;

		sprite_address = data;
		memory[address] = data;

		#ifdef PPU_MEM_DEBUG
		if(debug_cnt > show_debug_cnt) {
			printf("[%d] 0x2003 -> sprite_addr = %x, data = %x\n",debug_cnt,sprite_address,data);
		}
		#endif

		return;
	}

	/* sprite_memory i/o register */
	if(address == 0x2004) {
		ppu_addr_tmp = data;

		#ifdef PPU_SPR_MEM_DEBUG
		if(debug_cnt > show_debug_cnt) {
			printf("[%d] 0x2004 -> sprite_addr = %x, data = %x\n",debug_cnt,sprite_address,data);
		}
		#endif

		sprite_memory[sprite_address] = data;
		sprite_address++;

		memory[address] = data;
		return;
	}

	/* vram address register #1 (scrolling) */
	if(address == 0x2005) {
		ppu_addr_tmp = data;

		if(ppu_bgscr_f == 0x00) {
			loopyT &= 0xFFE0; // (0000000000011111)
			loopyT |= (data & 0xF8) >> 3; // (11111000)
			loopyX = data & 0x07; // (00000111)

			#ifdef PPU_MEM_DEBUG
			if(debug_cnt > show_debug_cnt) {
				printf("[%d] 0x2005 first = %x, loopyT: %x\n",debug_cnt,data,loopyT);
			}
			#endif

			ppu_bgscr_f = 0x01;

			memory[address] = data;

			return;
		}

		if(ppu_bgscr_f == 0x01) {
			loopyT &= 0xFC1F; // (0000001111100000)
			loopyT |= (data & 0xF8) << 2; //(0111000000000000)
			loopyT &= 0x8FFF; //(11111000)
			loopyT |= (data & 0x07) << 12; // (00000111)

			#ifdef PPU_MEM_DEBUG
			if(debug_cnt > show_debug_cnt) {
				printf("[%d] 0x2005 second = %x, loopyT: %x\n",debug_cnt,data,loopyT);
			}
			#endif

			ppu_bgscr_f = 0x00;

			memory[address] = data;

			return;
		}
	}

	/* vram address register #2 */
	if(address == 0x2006) {
		ppu_addr_tmp = data;

		/* First write -> Store the high byte 6 bits and clear out the last two */
		if(ppu_addr_h == 0x00) {
			ppu_addr = (data << 8);

			loopyT &= 0x00FF; // (0011111100000000)
			loopyT |= (data & 0x3F) << 8; // (1100000000000000) (00111111)

			#ifdef PPU_MEM_DEBUG
			if(debug_cnt > show_debug_cnt) {
				printf("[%d] 0x2006 first = %x, ppu_addr = %x, loopyT: %x\n",
					debug_cnt,data,ppu_addr,loopyT);
			}
			#endif

			ppu_addr_h = 0x01;

			memory[address] = data;

			return;
		}

		/* Second write -> Store the low byte 8 bits */
		if(ppu_addr_h == 0x01) {
			ppu_addr |= data;

			loopyT &= 0xFF00; // (0000000011111111)
			loopyT |= data; // (11111111)
			loopyV = loopyT; // v=t

			#ifdef PPU_MEM_DEBUG
			if(debug_cnt > show_debug_cnt) {
				printf("[%d] 0x2006 second = %x, ppu_addr = %x, loopyT: %x\n",
					debug_cnt,data,ppu_addr,loopyT);
			}
			#endif

			ppu_addr_h = 0x00;

			memory[address] = data;

			return;
		}
	}

	/* vram i/o register */
	if(address == 0x2007) {
		/* if the vram_write_flag is on, vram writes should ignored */
		if(vram_write_flag)
			return;

		ppu_addr_tmp = data;

		#ifdef PPU_MEM_DEBUG
		if(debug_cnt > show_debug_cnt) {
			printf("[%d] 0x2007 -> writing [%x] to ppu_memory[%x]\n",debug_cnt,data,ppu_addr);
		}
		#endif

		ppu_memory[ppu_addr] = data;

		/* nametable mirroring */
		if((ppu_addr > 0x1999) && (ppu_addr < 0x3000)) {
			if(OS_MIRROR == 1) {
				ppu_memory[ppu_addr + 0x400] = data;
				ppu_memory[ppu_addr + 0x800] = data;
				ppu_memory[ppu_addr + 0x1200] = data;
			} else if(FS_MIRROR == 1) {
				printf("FS_MIRRORING detected! do nothing\n");
			} else {
				if(MIRRORING == 0) {
					/* horizontal */
					ppu_memory[ppu_addr + 0x400] = data;
				} else {
					/* vertical */
					ppu_memory[ppu_addr + 0x800] = data;
				}
			}
		}

		/* palette mirror */
		if(ppu_addr == 0x3f10) {
			ppu_memory[0x3f00] = data;
		}

		ppu_addr_tmp = ppu_addr;

		if(!increment_32) {
			ppu_addr++;
		} else {
			ppu_addr += 0x20;
		}
		memory[address] = data;
		return;
	}

	/* transfer 256 bytes of memory into sprite_memory */
        if(address == 0x4014) {
		for(i = 0; i < 256; i++) {
			#ifdef PPU_SPR_MEM_DEBUG
			if(debug_cnt > show_debug_cnt) {
				printf("[%d] 0x4014 -> transfering data from mem address: 0x%x to spritememory 0x%x [data = %x]\n",
					debug_cnt,0x100 * data + i,data + i,memory[0x100 * data + i]);
			}
			#endif

			sprite_memory[i] = memory[0x100 * data + i];
		}
	}

	return;
}

void
render_background(int scanline)
{
	int tile_count;

	int i;

	/* scale x/y */
	int s_x;
	int s_y;

	int nt_addr;
	int at_addr;

	int x_scroll;
	int y_scroll;

	int pt_addr;

	int attribs;

	unsigned char bit1[8];
	unsigned char bit2[8];
	unsigned char tile[8];

	current_scanline = scanline;

	#ifdef SCANLINE_DEBUG
	if(debug_cnt > show_debug_cnt) {
		printf("[%d] --- start scanline: %d (%x) ---\n",debug_cnt,scanline,scanline);
	}
	#endif

	/* loopy scanline start -> v:0000010000011111=t:0000010000011111 | v=t */
	loopyV &= 0xfbe0;
	loopyV |= (loopyT & 0x041f);

	x_scroll = (loopyV & 0x1f);
	y_scroll = (loopyV & 0x03e0) >> 5;

	nt_addr = 0x2000 + (loopyV & 0x0fff);
	at_addr = 0x2000 + (loopyV & 0x0c00) + 0x03c0 + ((y_scroll & 0xfffc) << 1) + (x_scroll >> 2);

	if((y_scroll & 0x0002) == 0) {
		if((x_scroll & 0x0002) == 0) {
			attribs = (ppu_memory[at_addr] & 0x03) << 2;
		} else {
			attribs = (ppu_memory[at_addr] & 0x0C);
		}
	} else {
		if((x_scroll & 0x0002) == 0) {
			attribs = (ppu_memory[at_addr] & 0x30) >> 2;
		} else {
			attribs = (ppu_memory[at_addr] & 0xC0) >> 4;
		}
	}

	#ifdef SCANLINE_DEBUG
	if(debug_cnt > show_debug_cnt) {
		printf("[%d] nt_addr: %x, loopyT: %x, loopyV: %x, loopyX: %x\n",debug_cnt,nt_addr,loopyT,loopyV,loopyX);
	}
	#endif

	/* draw 33 tiles in a scanline (32 + 1 for scrolling) */
	for(tile_count = 0; tile_count < 33; tile_count++) {
		/* nt_data (ppu_memory[nt_addr]) * 16 = pattern table address */
		pt_addr = (ppu_memory[nt_addr] << 4) + ((loopyV & 0x7000) >> 12);

		/* check if the pattern address needs to be high */
		if(background_addr_hi)
			pt_addr+=0x1000;

		/* fetch bits from pattern table */
		for(i = 7; i >= 0; i--) {
			bit1[7 - i] = ((ppu_memory[pt_addr] >> i) & 0x01) ? 1 : 0;
			bit2[7 - i] = ((ppu_memory[pt_addr + 8] >> i) & 0x01) ? 1 : 0;
		}

		/* merge bits */
		for(i = 0; i < 8; i++) {
			if((bit1[i] == 0) &&  (bit2[i] == 0)) {
				tile[i] = 0;
			} else if((bit1[i] == 1) &&  (bit2[i] == 0)) {
				tile[i] = 1;
			} else if((bit1[i] == 0) &&  (bit2[i] == 1)) {
				tile[i] = 2;
			} else if((bit1[i] == 1) &&  (bit2[i] == 1)) {
				tile[i] = 3;
			}
		}

		/* merge colour */
		for(i = 7; i >= 0; i--) {
			/* pixel transparency check */
			if(tile[7 - i] != 0) {
				tile[7 - i] += attribs;
			}
		}

		if((tile_count == 0) && (loopyX != 0)) {
			for(i = 0; i < 8 - loopyX; i++) {
				/* cache pixel */
				bgcache[(tile_count << 3) + i] [scanline] = tile[loopyX + i];

				/* draw pixel */
				if((enable_background == 1) && (background_on) && (skipframe == 0)) {
					if(scale > 1) {
						for(s_y = 0; s_y < scale; s_y++) {
							for(s_x = 0; s_x < scale; s_x++) {
								if(ntsc == 1) {
									if(scanline > 7) {
										display_set_pixel(((tile_count << 3) + i) * scale + s_x, (scanline - 8) * scale + s_y, ppu_memory[0x3f00 + (tile[loopyX + i])]);
									}
								} else {
									display_set_pixel(((tile_count << 3) + i) * scale + s_x, scanline * scale + s_y, ppu_memory[0x3f00 + (tile[loopyX + i])]);
								}
							}
						}
					} else {
						if(ntsc == 1) {
							if(scanline > 7) {
								display_set_pixel((tile_count << 3) + i, scanline - 8, ppu_memory[0x3f00 + (tile[loopyX + i])]);
							}
						} else {
							display_set_pixel((tile_count << 3) + i, scanline, ppu_memory[0x3f00 + (tile[loopyX + i])]);
						}
					}
				}
			}
		} else if((tile_count == 32) && (loopyX != 0)) {
			for(i = 0; i < loopyX; i++) {
				/* cache pixel */
				bgcache[(tile_count << 3) + i - loopyX] [scanline] = tile[i];

				/* draw pixel */
				if((enable_background == 1) && (background_on) && (skipframe == 0)) {
					if(scale > 1) {
						for(s_y = 0; s_y < scale; s_y++) {
							for(s_x = 0; s_x < scale; s_x++) {
								if(ntsc == 1) {
									if(scanline > 7) {
										display_set_pixel(((tile_count << 3) + i - loopyX) * scale + s_x, (scanline - 8) * scale + s_y, ppu_memory[0x3f00 + (tile[i])]);
									}
								} else {
									display_set_pixel(((tile_count << 3) + i - loopyX) * scale + s_x, scanline * scale + s_y, ppu_memory[0x3f00 + (tile[i])]);
								}
							}
						}
					} else {
						if(ntsc == 1) {
							if(scanline > 7) {
								display_set_pixel((tile_count << 3) + i - loopyX, scanline - 8, ppu_memory[0x3f00 + (tile[i])]);
							}
						} else {
							display_set_pixel((tile_count << 3) + i - loopyX, scanline, ppu_memory[0x3f00 + (tile[i])]);
						}
					}
				}
			}
		} else {
			for(i = 0; i < 8; i++) {
				/* cache pixel */
				bgcache[(tile_count << 3) + i - loopyX] [scanline] = tile[i];

				/* draw pixel */
				if((enable_background == 1) && (background_on) && (skipframe == 0)) {
					if(scale > 1) {
						for(s_y = 0; s_y < scale; s_y++) {
							for(s_x = 0; s_x < scale; s_x++) {
								if(ntsc == 1) {
									if(scanline > 7) {
										display_set_pixel(((tile_count << 3) + i - loopyX) * scale + s_x, (scanline - 8) * scale + s_y, ppu_memory[0x3f00 + (tile[i])]);
									}
								} else {
									display_set_pixel(((tile_count << 3) + i - loopyX) * scale + s_x, scanline * scale + s_y, ppu_memory[0x3f00 + (tile[i])]);
								}
							}
						}
					} else {
						if(ntsc == 1) {
							if(scanline > 7) {
								display_set_pixel((tile_count << 3) + i - loopyX, scanline - 8, ppu_memory[0x3f00 + (tile[i])]);
							}
						} else {
							display_set_pixel((tile_count << 3) + i - loopyX, scanline, ppu_memory[0x3f00 + (tile[i])]);
						}
					}
				}
			}
		}

		nt_addr++;
		x_scroll++;

		/* boundary check */
		/* dual-tile */
		if((x_scroll & 0x0001) == 0) {
			/* quad-tile */
			if((x_scroll & 0x0003) == 0) {
				/* check if we crossed a nametable */
				if((x_scroll & 0x1F) == 0) {
					/* switch name/attrib tables */
					nt_addr ^= 0x0400;
					at_addr ^= 0x0400;
					nt_addr -= 0x0020;
					at_addr -= 0x0008;
					x_scroll -= 0x0020;
				}

				at_addr++;
			}

			if((y_scroll & 0x0002) == 0) {
				if((x_scroll & 0x0002) == 0) {
					attribs = ((ppu_memory[at_addr]) & 0x03) << 2;
				} else {
					attribs = ((ppu_memory[at_addr]) & 0x0C);
				}
			} else {
				if((x_scroll & 0x0002) == 0) {
					attribs = ((ppu_memory[at_addr]) & 0x30) >> 2;
				} else {
					attribs = ((ppu_memory[at_addr]) & 0xC0) >> 4;
				}
			}

		}
	}

	/* subtile y_offset == 7 */
	if((loopyV & 0x7000) == 0x7000) {
		/* subtile y_offset = 0 */
		loopyV &= 0x8fff;

		/* nametable line == 29 */
		if((loopyV & 0x03e0) == 0x03a0) {
			/* switch nametables (bit 11) */
			loopyV ^= 0x0800;

			/* name table line = 0 */
			loopyV &= 0xfc1f;
		} else {
			/* nametable line == 31 */
			if((loopyV & 0x03e0) == 0x03e0) {
				/* name table line = 0 */
				loopyV &= 0xfc1f;
			} else {
          			loopyV += 0x0020;
			}
		}
	} else {
		/* next subtile y_offset */
		loopyV += 0x1000;
	}

	#ifdef SCANLINE_DEBUG
	if(debug_cnt > show_debug_cnt) {
		printf("[%d] --- end scanline: %d (%x) ---\n",debug_cnt,scanline,scanline);
	}
	#endif
}

void
render_sprite(int y, int x, int pattern_number, int attribs, int spr_nr)
{
	#ifdef SPRITE_RENDER_DEBUG
	if(debug_cnt > show_debug_cnt) {
		printf("[%d] (spritedebug [%d]): hor = %x, ver = %x, pattern_number = %d, attribs = %d\n", 
			debug_cnt,spr_nr,x,y,pattern_number,attribs);
	}
	#endif

	int color_bit1;
	int color_bit2;
	int disp_spr_back;
	int flip_spr_hor;
	int flip_spr_ver;

	color_bit1 = attribs & 0x01;
	color_bit2 = attribs & 0x02;
	disp_spr_back = attribs & 0x20;
	flip_spr_hor = attribs & 0x40;
	flip_spr_ver = attribs & 0x80;

	int i;
	int j;

	/* scale x/y */
	int s_x;
	int s_y;

	int spr_start;
	int sprite_pattern_table;

	unsigned char bit1[8] [16];
	unsigned char bit2[8] [16];
	unsigned char sprite[8] [16];

	#ifdef SPRITE_RENDER_DEBUG
	if(debug_cnt > show_debug_cnt) {
		printf("[%d] (spritedebug [%d]): attribs [%x] -> color_bit1 [%x] -> color_bit2 [%x] -> disp_spr_back [%d] -> flip_spr_hor [%d] -> flip_spr_ver [%d]\n",
			debug_cnt,spr_nr,attribs,color_bit1,color_bit2,disp_spr_back,flip_spr_hor,flip_spr_ver);
	}
	#endif

	if(!sprite_addr_hi) {
		sprite_pattern_table=0x0000;
	} else {
		sprite_pattern_table=0x1000;
	}

	/* - pattern_number * 16 */
	spr_start = sprite_pattern_table + ((pattern_number << 3) << 1);

	#ifdef SPRITE_RENDER_DEBUG
	if(debug_cnt > show_debug_cnt) {
		printf("[%d] (spritedebug [%d]): pattern_number = %d [hex %x], sprite_patterntable start addr = %x, ppu mem value = %x\n",
			debug_cnt,spr_nr, pattern_number, pattern_number, sprite_pattern_table + (pattern_number * 16), ppu_memory[sprite_pattern_table + (pattern_number * 16)]);
	}
	#endif

	if(!sprite_16) {
		/* 8 x 8 sprites */
		/* fetch bits */
		if((!flip_spr_hor) && (!flip_spr_ver)) {
			for(i = 7; i >= 0; i--) {
				for(j = 0; j < 8; j++) {
					bit1[7 - i] [j] = ((ppu_memory[spr_start + j] >> i) & 0x01) ? 1 : 0;
					bit2[7 - i] [j] = ((ppu_memory[spr_start + 8 + j] >> i) & 0x01) ? 1 : 0;
				}
			}
		} else if((flip_spr_hor) && (!flip_spr_ver)) {
			for(i = 0; i < 8; i++) {
				for(j = 0; j < 8; j++) {
					bit1[i] [j] = ((ppu_memory[spr_start + j] >> i) & 0x01) ? 1 : 0;
					bit2[i] [j] = ((ppu_memory[spr_start + 8 + j] >> i) & 0x01) ? 1 : 0;
				}
			}
		} else if((!flip_spr_hor) && (flip_spr_ver)) {
			for(i = 7; i >= 0; i--) {
				for(j = 7; j >= 0; j--) {
					bit1[7 - i] [7 - j] = ((ppu_memory[spr_start + j] >> i) & 0x01) ? 1 : 0;
					bit2[7 - i] [7 - j] = ((ppu_memory[spr_start + 8 + j] >> i) & 0x01) ? 1 : 0;
				}
			}
		} else if((flip_spr_hor) && (flip_spr_ver)) {
			for(i = 0; i < 8; i++) {
				for(j = 7; j >= 0; j--) {
					bit1[i] [7 - j] = ((ppu_memory[spr_start + j] >> i) & 0x01) ? 1 : 0;
					bit2[i] [7 - j] = ((ppu_memory[spr_start + 8 + j] >> i) & 0x01) ? 1 : 0;
				}
			}
		}

		/* merge bits */
		for(i = 0; i < 8; i++) {
			for(j = 0; j < 8; j++) {
				if((bit1[i] [j] == 0) &&  (bit2[i] [j] == 0)) {
					sprite[i] [j] = 0;
				} else if((bit1[i] [j] == 1) &&  (bit2[i] [j] == 0)) {
					sprite[i] [j] = 1;
				} else if((bit1[i] [j] == 0) &&  (bit2[i] [j] == 1)) {
					sprite[i] [j] = 2;
				} else if((bit1[i] [j] == 1) &&  (bit2[i] [j] == 1)) {
					sprite[i] [j] = 3;
				}
			}
		}	

		/* add sprite attribute colors */
		if((!flip_spr_hor) && (!flip_spr_ver)) {
			for(i = 7; i >= 0; i--) {
				for(j = 0; j < 8; j++) {
					if(sprite[7 - i] [j] != 0) {
						if(sprite[7 - i] [j] != 0)
							sprite[7 - i] [j] += ((attribs & 0x03) << 0x02);
					}
				}
			}
		} else if((flip_spr_hor) && (!flip_spr_ver)) {
			for(i = 0; i < 8; i++) {
				for(j = 0; j < 8; j++) {
					if(sprite[i] [j] != 0) {
						if(sprite[i] [j] != 0)
							sprite[i] [j] += ((attribs & 0x03) << 0x02);
					}
				}
			}
		} else if((!flip_spr_hor) && (flip_spr_ver)) {
			for(i = 7; i >= 0; i--) {
				for(j = 7; j >= 0; j--) {
					if(sprite[7 - i] [7 - j] != 0) {
						if(sprite[7 - i] [7 - j] != 0)
							sprite[7 - i] [7 - j] += ((attribs & 0x03) << 0x02);
					}
				}
			}
		} else if((flip_spr_hor) && (flip_spr_ver)) {
			for(i = 0; i < 8; i++) {
				for(j = 7; j >= 0; j--) {
					if(sprite[i] [7 - j] != 0) {
						if(sprite[i] [7 - j] != 0)
							sprite[i] [7 - j] += ((attribs & 0x03) << 0x02);
					}
				}
			}
		}

		for(i = 0; i < 8; i++) {
			for(j = 0; j < 8; j++) {
				/* cache pixel for sprite zero detection */
				if(spr_nr == 0)
					sprcache[x + i] [y + j] = sprite[i] [j];

				if(sprite[i] [j] != 0) {
					/* sprite priority check */
					if(!disp_spr_back) {
						if((enable_sprites == 1) && (sprite_on) && (skipframe == 0)) {
							/* draw pixel */
							if(scale > 1) {
								for(s_y = 0; s_y < scale; s_y++) {
									for(s_x = 0; s_x < scale; s_x++) {
										if(ntsc == 1) {
											if(y + j > 7) {
												display_set_pixel((x + i) * scale + s_x, ((y - 8) + j) * scale + s_y, ppu_memory[0x3f10 + (sprite[i] [j])]);
											}
										} else {
											display_set_pixel((x + i) * scale + s_x, (y + j) * scale + s_y, ppu_memory[0x3f10 + (sprite[i] [j])]);
										}
									}
								}
							} else {
								if(ntsc == 1) {
									if(y + j > 7) {
										display_set_pixel(x + i, (y - 8) + j, ppu_memory[0x3f10 + (sprite[i] [j])]);
									}
								} else {
									display_set_pixel(x + i, y + j, ppu_memory[0x3f10 + (sprite[i] [j])]);
								}
							}
						}
					} else {
						if((enable_sprites == 1) && (sprite_on) && (skipframe == 0)) {
							/* draw the sprite pixel if the background pixel is transparent (0) */
							if(bgcache[x+i] [y+j] == 0) {
								/* draw pixel */
								if(scale > 1) {
									for(s_y = 0; s_y < scale; s_y++) {
										for(s_x = 0; s_x < scale; s_x++) {
											if(ntsc == 1) {
												if(y + j > 7) {
													display_set_pixel((x + i) * scale + s_x, ((y - 8) + j) * scale + s_y, ppu_memory[0x3f10 + (sprite[i] [j])]);
												}
											} else {
												display_set_pixel((x + i) * scale + s_x, (y + j) * scale + s_y, ppu_memory[0x3f10 + (sprite[i] [j])]);
											}
										}
									}
								} else {
									if(ntsc == 1) {
										if(y + j > 7) {
											display_set_pixel(x + i, (y - 8) + j, ppu_memory[0x3f10 + (sprite[i] [j])]);
										}
									} else {
										display_set_pixel(x + i, y + j, ppu_memory[0x3f10 + (sprite[i] [j])]);
									}
								}
							}
						}
					}
				}

				#ifdef SPRITE_RENDER_DEBUG
				if(debug_cnt > show_debug_cnt) {
					printf("[%d] (spritedebug [%d]): drawing pixel on place [%d] on scanline [%d] with nes color [%x] on ppu_mem [%x]\n",
					debug_cnt,spr_nr,y + i, x + j,ppu_memory[0x3f10 + (sprite[i] [j])],0x3f10 + (sprite[i] [j]));
				}
				#endif
			}
		}
	} else {
		/* 8 x 16 sprites */
		/* fetch bits */
		if((!flip_spr_hor) && (!flip_spr_ver)) {
			for(i = 7; i >= 0; i--) {
				for(j = 0; j < 16; j++) {
					bit1[7 - i] [j] = ((ppu_memory[spr_start + j] >> i) & 0x01) ? 1 : 0;
					bit2[7 - i] [j] = ((ppu_memory[spr_start + 8 + j] >> i) & 0x01) ? 1 : 0;
				}
			}
		} else if((flip_spr_hor) && (!flip_spr_ver)) {
			for(i = 0; i < 8; i++) {
				for(j = 0; j < 16; j++) {
					bit1[i] [j] = ((ppu_memory[spr_start + j] >> i) & 0x01) ? 1 : 0;
					bit2[i] [j] = ((ppu_memory[spr_start + 8 + j] >> i) & 0x01) ? 1 : 0;
				}
			}
		} else if((!flip_spr_hor) && (flip_spr_ver)) {
			for(i = 7; i >= 0; i--) {
				for(j = 15; j >= 0; j--) {
					bit1[7 - i] [7 - j] = ((ppu_memory[spr_start + j] >> i) & 0x01) ? 1 : 0;
					bit2[7 - i] [7 - j] = ((ppu_memory[spr_start + 8 + j] >> i) & 0x01) ? 1 : 0;
				}
			}
		} else if((flip_spr_hor) && (flip_spr_ver)) {
			for(i = 0; i < 8; i++) {
				for(j = 15; j >= 0; j--) {
					bit1[i] [7 - j] = ((ppu_memory[spr_start + j] >> i) & 0x01) ? 1 : 0;
					bit2[i] [7 - j] = ((ppu_memory[spr_start + 8 + j] >> i) & 0x01) ? 1 : 0;
				}
			}
		}

		/* merge bits */
		for(i = 0; i < 8; i++) {
			for(j = 0; j < 16; j++) {
				if((bit1[i] [j] == 0) &&  (bit2[i] [j] == 0)) {
					sprite[i] [j] = 0;
				} else if((bit1[i] [j] == 1) &&  (bit2[i] [j] == 0)) {
					sprite[i] [j] = 1;
				} else if((bit1[i] [j] == 0) &&  (bit2[i] [j] == 1)) {
					sprite[i] [j] = 2;
				} else if((bit1[i] [j] == 1) &&  (bit2[i] [j] == 1)) {
					sprite[i] [j] = 3;
				}
			}
		}	

		/* add sprite attribute colors */
		if((!flip_spr_hor) && (!flip_spr_ver)) {
			for(i = 7; i >= 0; i--) {
				for(j = 0; j < 16; j++) {
					if(sprite[7 - i] [j] != 0) {
						if(sprite[7 - i] [j] != 0)
							sprite[7 - i] [j] += ((attribs & 0x03) << 0x02);
					}
				}
			}
		} else if((flip_spr_hor) && (!flip_spr_ver)) {
			for(i = 0; i < 8; i++) {
				for(j = 0; j < 16; j++) {
					if(sprite[i] [j] != 0) {
						if(sprite[i] [j] != 0)
							sprite[i] [j] += ((attribs & 0x03) << 0x02);
					}
				}
			}
		} else if((!flip_spr_hor) && (flip_spr_ver)) {
			for(i = 7; i >= 0; i--) {
				for(j = 15; j >= 0; j--) {
					if(sprite[7 - i] [15 - j] != 0) {
						if(sprite[7 - i] [15 - j] != 0)
							sprite[7 - i] [15 - j] += ((attribs & 0x03) << 0x02);
					}
				}
			}
		} else if((flip_spr_hor) && (flip_spr_ver)) {
			for(i = 0; i < 8; i++) {
				for(j = 15; j >= 0; j--) {
					if(sprite[i] [15 - j] != 0) {
						if(sprite[i] [15 - j] != 0)
							sprite[i] [15 - j] += ((attribs & 0x03) << 0x02);
					}
				}
			}
		}

		for(i = 0; i < 8; i++) {
			for(j = 0; j < 16; j++) {
				/* cache pixel for sprite zero detection */
				if(spr_nr == 0)
					sprcache[x + i] [y + j] = sprite[i] [j];

				if(sprite[i] [j] != 0) {
					/* sprite priority check */
					if(!disp_spr_back) {
						if((enable_sprites == 1) && (sprite_on) && (skipframe == 0)) {
							/* draw pixel */
							if(scale > 1) {
								for(s_y = 0; s_y < scale; s_y++) {
									for(s_x = 0; s_x < scale; s_x++) {
										if(ntsc == 1) {
											if(y + j > 7) {
												display_set_pixel((x + i) * scale + s_x, ((y - 8) + j) * scale + s_y, ppu_memory[0x3f10 + (sprite[i] [j])]);
											}
										} else {
											display_set_pixel((x + i) * scale + s_x, (y + j) * scale + s_y, ppu_memory[0x3f10 + (sprite[i] [j])]);
										}
									}
								}
							} else {
								if(ntsc == 1) {
									if(y + j > 7) {
										display_set_pixel(x + i, (y - 8) + j, ppu_memory[0x3f10 + (sprite[i] [j])]);
									}
								} else {
									display_set_pixel(x + i, y + j, ppu_memory[0x3f10 + (sprite[i] [j])]);
								}
							}
						}
					} else {
						/* draw the sprite pixel if the background pixel is transparent (0) */
						if(bgcache[x+i] [y+j] == 0) {
							if((enable_sprites == 1) && (sprite_on) && (skipframe == 0)) {
								/* draw pixel */
								if(scale > 1) {
									for(s_y = 0; s_y < scale; s_y++) {
										for(s_x = 0; s_x < scale; s_x++) {
											if(ntsc == 1) {
												if(y + j > 7) {
													display_set_pixel((x + i) * scale + s_x, ((y - 8) + j) * scale + s_y, ppu_memory[0x3f10 + (sprite[i] [j])]);
												}
											} else {
												display_set_pixel((x + i) * scale + s_x, (y + j) * scale + s_y, ppu_memory[0x3f10 + (sprite[i] [j])]);
											}
										}
									}
								} else {
									if(ntsc == 1) {
										if(y + j > 7) {
											display_set_pixel(x + i, (y - 8) + j, ppu_memory[0x3f10 + (sprite[i] [j])]);
										}
									} else {
										display_set_pixel(x + i, y + j, ppu_memory[0x3f10 + (sprite[i] [j])]);
									}
								}
							}
						}
					}
				}

				#ifdef SPRITE_RENDER_DEBUG
				if(debug_cnt > show_debug_cnt) {
					printf("debug [%d] (spritedebug [%d]): drawing pixel on place [%d] on scanline [%d] with nes color [%x] on ppu_mem [%x]\n",
						debug_cnt,spr_nr,y + i, x + j,ppu_memory[0x3f10 + (sprite[i] [j])],0x3f10 + (sprite[i] [j]));
				}
				#endif
			}
		}
	}
}

void
check_sprite_hit(int scanline)
{
	/* sprite zero detection */
	int i;

	for(i = 0; i < width; i++) {
		if((bgcache[i] [scanline - 1] > 0x00) && (sprcache[i] [scanline - 1] > 0x00)) {
			#ifdef PPU_DEBUG
			printf("debug [%d]: sprite zero found at x:%d, y:%d\n",
				debug_cnt,i,scanline-1);
			#endif

			/* set the sprite zero flag */
			ppu_status |= 0x40;
		}
	}
}

void
render_sprites()
{
	int i = 0;

	/* clear sprite cache */
	memset(sprcache,0x00,sizeof(sprcache));

	/* fetch all 64 sprites out of the sprite memory 4 bytes at a time and render the sprite */
	/* sprites are drawn in priority from sprite 64 to 0 */
	for(i = 63; i >= 0; i--) {
		render_sprite(sprite_memory[i * 4],sprite_memory[i * 4 + 3],sprite_memory[i * 4 + 1],sprite_memory[i * 4 + 2],i);
	}
}
