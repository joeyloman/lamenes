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

//#define DEBUG
//#define DISASSAMBLE
//#define PAD_DEBUG
//#define MAPPER_DEBUG

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "lame6502/lame6502.h"
#include "lame6502/debugger.h"
#include "lame6502/disas.h"
#include "lame6502/instructions.h"

#include "macros.h"
#include "lamenes.h"
#include "romloader.h"
#include "ppu.h"
#include "input.h"

#include "system/buttons.h"
#include "system/display.h"
#include "system/sleep.h"

#include "lib/str_chrchk.h"
#include "lib/str_replace.h"

/* included mappers */
#include "mappers/mmc1.h"	// 1
#include "mappers/unrom.h"	// 2
#include "mappers/cnrom.h"	// 3
#include "mappers/mmc3.h"	// 4

char romfn[256];

/* cache the rom in memory to access the data quickly */
unsigned char *romcache;

unsigned char *ppu_memory;
unsigned char *sprite_memory;

unsigned int pad1_data;
int pad1_readcount = 0;

int start_int;
int vblank_int;
int vblank_cycle_timeout;
int scanline_refresh;

int ntsc = 0;
int pal = 1;

int CPU_is_running = 1;
int pause_emulation = 0;

int height;
int width;

int sdl_screen_height;
int sdl_screen_width;

int enable_background = 1;
int enable_sprites = 1;

int fullscreen = 0;
int scale = 1;

int frameskip = 0;
int skipframe = 0;

int sdl_delay = 10;

char *savfile;
char *statefile;

long romlen;

FILE *sav_fp;

void
open_sav()
{
	sav_fp = fopen(savfile,"rb");

	if(sav_fp) {
		printf("[*] %s found! loading into sram...\n",savfile);
		fseek(sav_fp,0,SEEK_SET);
		fread(&memory[0x6000],1,8192,sav_fp);

		fclose(sav_fp);
	}
}

void
write_sav()
{
	sav_fp = fopen(savfile,"wb");
	fwrite(&memory[0x6000],1,8192,sav_fp);
	fclose(sav_fp);
}

void
load_state()
{
	int pf;

	FILE *lst_fp;

	fprintf(stdout,"[*] loading state from file: %s\n",statefile);

	lst_fp = fopen(statefile,"rb");

	if(lst_fp) {
		fread(&program_counter,1,2,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,2,SEEK_CUR);
		fread(&stack_pointer,1,1,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,3,SEEK_CUR);
		fread(&status_register,1,1,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,4,SEEK_CUR);
		fread(&x_reg,1,1,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,5,SEEK_CUR);
		fread(&y_reg,1,1,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,6,SEEK_CUR);
		fread(&accumulator,1,1,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,7,SEEK_CUR);
		fread(&pf,1,1,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,8,SEEK_CUR);
		fread(&ppu_control1,1,2,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,10,SEEK_CUR);
		fread(&ppu_control2,1,2,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,13,SEEK_CUR);
		fread(&ppu_addr_h,1,2,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,14,SEEK_CUR);
		fread(&ppu_addr,1,2,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,16,SEEK_CUR);
		fread(&ppu_addr_tmp,1,2,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,18,SEEK_CUR);
		fread(&ppu_status,1,2,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,20,SEEK_CUR);
		fread(&ppu_status_tmp,1,2,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,22,SEEK_CUR);
		fread(&sprite_address,1,2,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,24,SEEK_CUR);
		fread(&ppu_bgscr_f,1,2,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,26,SEEK_CUR);
		fread(&loopyT,1,2,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,28,SEEK_CUR);
		fread(&loopyV,1,2,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,30,SEEK_CUR);
		fread(&loopyX,1,2,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,32,SEEK_CUR);
		fread(&memory[0x0000],1,65536,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,65568,SEEK_CUR);
		fread(&ppu_memory[0x0000],1,16384,lst_fp);

		fseek(lst_fp,0,SEEK_SET);
		fseek(lst_fp,81952,SEEK_CUR);
		fread(&sprite_memory[0x0000],1,256,lst_fp);

		SET_SR(pf)

		fclose(lst_fp);

		fprintf(stdout,"[*] done!\n");
	} else {
		fprintf(stdout,"[!] no save state file found!\n");
	}
}

void
save_state()
{
	int pf;

	FILE *sst_fp;

	fprintf(stdout,"[*] saving state to file: %s\n",statefile);

	sst_fp = fopen(statefile,"wb");

	pf = ((sign_flag ? 0x80 : 0) |
		(zero_flag ? 0x02 : 0) |
		(carry_flag ? 0x01 : 0) |
		(interrupt_flag ? 0x04 : 0) |
		(decimal_flag ? 0x08 : 0) |
		(overflow_flag ? 0x40 : 0) |
		(break_flag ? 0x10 : 0) | 0x20);

	fwrite(&program_counter,1,2,sst_fp);
	fwrite(&stack_pointer,1,1,sst_fp);
	fwrite(&status_register,1,1,sst_fp);
	fwrite(&x_reg,1,1,sst_fp);
	fwrite(&y_reg,1,1,sst_fp);
	fwrite(&accumulator,1,1,sst_fp);
	fwrite(&pf,1,1,sst_fp);

	fwrite(&ppu_control1,1,2,sst_fp);
	fwrite(&ppu_control2,1,2,sst_fp);
	fwrite(&ppu_addr_h,1,2,sst_fp);
	fwrite(&ppu_addr,1,2,sst_fp);
	fwrite(&ppu_addr_tmp,1,2,sst_fp);
	fwrite(&ppu_status,1,2,sst_fp);
	fwrite(&ppu_status_tmp,1,2,sst_fp);
	fwrite(&sprite_address,1,2,sst_fp);
	fwrite(&ppu_bgscr_f,1,2,sst_fp);

	fwrite(&loopyT,1,2,sst_fp);
	fwrite(&loopyV,1,2,sst_fp);
	fwrite(&loopyX,1,2,sst_fp);

	fwrite(&memory[0x0000],1,65536,sst_fp);
	fwrite(&ppu_memory[0x0000],1,16384,sst_fp);
	fwrite(&sprite_memory[0x0000],1,256,sst_fp);

	fclose(sst_fp);

	fprintf(stdout,"[*] done!\n");
}

/*
 * memory read handler
 */
unsigned char memory_read(unsigned int address) {
	/* this is ram or rom so we can return the address */
	if(address < 0x2000 || address > 0x7FFF)
		return memory[address];

	/* the addresses between 0x2000 and 0x5000 are for input/ouput */
	if(address == 0x2002) {
		ppu_status_tmp = ppu_status;

		/* set ppu_status (D7) to 0 (vblank_on) */
		ppu_status &= 0x7F;
		write_memory(0x2002,ppu_status);

		/* set ppu_status (D6) to 0 (sprite_zero) */
		ppu_status &= 0x1F;
		write_memory(0x2002,ppu_status);

		/* reset VRAM Address Register #1 */
		ppu_bgscr_f = 0x00;

		/* reset VRAM Address Register #2 */
		ppu_addr_h = 0x00;

		/* return bits 7-4 of unmodifyed ppu_status with bits 3-0 of the ppu_addr_tmp */
		return (ppu_status_tmp & 0xE0) | (ppu_addr_tmp & 0x1F);
	}

	if(address == 0x2007) {
		tmp = ppu_addr_tmp;
		ppu_addr_tmp = ppu_addr;

		if(increment_32 == 0) {
			ppu_addr++;
		} else {
			ppu_addr += 0x20;
		}

		return ppu_memory[tmp];
	}

	/* pAPU data (sound) */
	if(address == 0x4015) {
		return memory[address];
	}

	/* joypad1 data */
	if(address == 0x4016) {
		switch(pad1_readcount) {
			case 0:
			memory[address] = pad1_A;
			pad1_readcount++;
			break;

			case 1:
			memory[address] = pad1_B;
			pad1_readcount++;
			break;

			case 2:
			memory[address] = pad1_SELECT;
			pad1_readcount++;
			break;

			case 3:
			memory[address] = pad1_START;
			pad1_readcount++;
			break;

			case 4:
			memory[address] = pad1_UP;
			pad1_readcount++;
			break;

			case 5:
			memory[address] = pad1_DOWN;
			pad1_readcount++;
			break;

			case 6:
			memory[address] = pad1_LEFT;
			pad1_readcount++;
			break;

			case 7:
			memory[address] = pad1_RIGHT;
			pad1_readcount = 0;
			break;
		}

		#ifdef PAD_DEBUG
		printf("debug [%d]: pad1_A -> %d, pad1_B -> %d, pad1_SELECT -> %d, pad1_START -> %d, pad1_UP -> %d, pad1_DOWN -> %d, pad1_LEFT -> %d, pad1_RIGHT -> %d\n",
			debug_cnt,pad1_A,pad1_B,pad1_SELECT,pad1_START,pad1_UP,pad1_DOWN,pad1_LEFT,pad1_RIGHT);

		printf("debug [%d]: pad1_readcount = %d, read joydata = %d [hex: %x]\n",debug_cnt,pad1_readcount,memory[address],memory[address]);
		#endif

		return memory[address];
	}

	if(address == 0x4017) {
		return memory[address];
	}

	return memory[address];
}

/*
 * memory write handler
 */
void
write_memory(unsigned int address,unsigned char data)
{
	/* PPU Status */
	if(address == 0x2002) {
		memory[address] = data;
		return;
	}

	/* PPU Video Memory area */
	if(address > 0x1fff && address < 0x4000) {
		write_ppu_memory(address,data);
		return;
	}

	/* Sprite DMA Register */
	if(address == 0x4014) {
		write_ppu_memory(address,data);
		return;
	}

	/* Joypad 1 */
	if(address == 0x4016) {
		memory[address] = 0x40;

		return;
	}

	/* Joypad 2 */
	if(address == 0x4017) {
		memory[address] = 0x48;
		return;
	}

	/* pAPU Sound Registers */
	if(address > 0x3fff && address < 0x4016) {
		//write_sound(address,data); not emulated yet!
		memory[address] = data;
		return;
	}

	/* SRAM Registers */
	if(address > 0x5fff && address < 0x8000) {
		if(SRAM == 1)
			write_sav();

		memory[address] = data;
		return;
	}

	/* RAM registers */
	if(address < 0x2000) {
		memory[address] = data;
		memory[address+2048] = data; // mirror of 0-800
		memory[address+4096] = data; // mirror of 0-800
		memory[address+6144] = data; // mirror of 0-800
		return;
	}

	if(MAPPER == 1) {
		mmc1_access(address,data);
		return;
	}

	if(MAPPER == 2) {
		unrom_access(address,data);
		return;
	}

	if(MAPPER == 3) {
		cnrom_access(address,data);
		return;
	}

	if(MAPPER == 4) {
		mmc3_access(address,data);
		return;
	}

	if(startdebugger > 0) {
		disassemble = 1;
		hit_break = 1;
		debugger();
	}

	memory[address] = data;
}

void
show_header()
{
	fprintf(stdout,"\n"
	"************************************************************\n"
	"*** LameNES version 0.1 by Joey Loman <joey@lamenes.org> ***\n"
	"************************************************************\n"
	"\n"
	);
}

void
usage(char *pname)
{
	fprintf(stdout,"usage: %s [options] <rom-filename>\n\n"
	"options:\n"
	"\t-debug\t\t\t-> enable debugger (F12)\n"
	"\t-startdebug\t\t-> enable debugger (F12) and start it immediately\n"
	"\n"
	"\t-scale <value>\t\t-> scale the screen\n"
	"\t-fullscreen\t\t-> toggle fullscreen\n"
	"\n"
	"\t-frameskip <value>\t-> frameskip (speed up emulation)\n"
	"\t-delay <value>\t\t-> delay (slow down emulation)\n"
	"\n"
	"\t-pal\t\t\t-> pal video mode (default)\n"
	"\t-ntsc\t\t\t-> ntsc video mode\n\n"
	,pname);

	exit(1);
}

void
start_emulation()
{
	int counter = 0;

	int scanline = 0;


	while(CPU_is_running) {
//printf("[%d] 1 counter = %d / pvi = %d (counter - PAL_VBLANK_INT = %d)\n",debug_cnt,counter,PAL_VBLANK_INT + 341,(counter - (PAL_VBLANK_INT + 341)));
		CPU_execute(start_int);

		/* set ppu_status D7 to 1 and enter vblank */
		ppu_status |= 0x80;
		write_memory(0x2002,ppu_status);

		counter += CPU_execute(12); // needed for some roms

		#ifdef DEBUG
		if(debug_cnt > show_debug_cnt) {
			printf("debug [%d] --- entering VBLANK! ---\n",debug_cnt);
			printf("ppu_status: %x\n",ppu_status);
		}
		#endif

		if(exec_nmi_on_vblank) {
			#ifdef DEBUG
			if(debug_cnt > show_debug_cnt) {
				printf("[%d] vblank = on\n",debug_cnt);
			}
			#endif

			counter += NMI(counter);
		}

		counter += CPU_execute(vblank_cycle_timeout);

		#ifdef DEBUG
		if(debug_cnt > show_debug_cnt) {
			printf("[%d] --- leaving VBLANK! ---\n",debug_cnt);
		}
		#endif

		/* vblank ends (ppu_status D7) is set to 0, sprite_zero (ppu_status D6) is set to 0 */
		ppu_status &= 0x3F;
	
		/* and write to mem */
		write_memory(0x2002,ppu_status);

		loopyV = loopyT;

		if(skipframe > frameskip)
			skipframe = 0;

		display_lock();
		for(scanline = 0; scanline < 240; scanline++) {
			if(!sprite_zero) {
				check_sprite_hit(scanline);
			}

			render_background(scanline);

			counter += CPU_execute(scanline_refresh);

			if(mmc3_irq_enable == 1) {
				if(scanline == mmc3_irq_counter) {
					//printf("[%d] mmc3_irq_counter = %d\n",debug_cnt,mmc3_irq_counter);
					IRQ(counter);
					mmc3_irq_counter--;

					//break;
				}
			}
		}

		render_sprites();
		display_unlock();

		#ifdef DEBUG
		if(debug_cnt > show_debug_cnt) {
			printf("[%d]: *** UPDATING SCREEN ***\n",debug_cnt);
		}
		#endif

		if(skipframe == 0)
			display_update();

		sleep_ms(sdl_delay);

		skipframe++;

		//if(!interrupt_flag) {
		//	counter += IRQ(counter);
		//}

		poll_buttons();
	}
}

void
reset_emulation()
{
	printf("[*] resetting emulation...\n");

	if(load_rom(romfn) == 1) {
		free(sprite_memory);
		free(ppu_memory);
		free(memory);
		free(romcache);
		exit(1);
	}

	if(MAPPER == 4)
		mmc3_reset();

	CPU_reset();

	reset_input();

	start_emulation();
}

void
quit_emulation()
{
	/* free all memory */
	free(sprite_memory);
	free(ppu_memory);
	free(memory);
	free(romcache);

	printf("[!] quiting LameNES!\n\n");

	exit(0);
}

int main(int argc, char *argv[]) {
	int chr_check_result;

	int i;

	/* cpu speed */
	int NTSC_SPEED = 1789725;
	int PAL_SPEED = 1773447;

	/* vblank int */
	int NTSC_VBLANK_INT = NTSC_SPEED / 60;
	int PAL_VBLANK_INT = PAL_SPEED / 50;

	/* scanline refresh (hblank)*/
	int NTSC_SCANLINE_REFRESH = NTSC_VBLANK_INT / 261;
	int PAL_SCANLINE_REFRESH = PAL_VBLANK_INT / 313;

	/* vblank int cycle timeout */
	int NTSC_VBLANK_CYCLE_TIMEOUT = (261-224) * NTSC_VBLANK_INT / 261;
	int PAL_VBLANK_CYCLE_TIMEOUT = (313-240) * PAL_VBLANK_INT / 313;

	show_header();

	if(argc < 2) {
		usage(argv[0]);
	}

	for(i = 0; i < argc; i++) {
		if(i == 0) {
			/* do nothing */
		} else if(i == argc - 1) {
			snprintf(romfn,sizeof(romfn),"%s",argv[i]);
		} else if(!strcmp(argv[i],"-debug")) {
			startdebugger = 1;
		} else if(!strcmp(argv[i],"-startdebug")) {
			startdebugger = 2;
		} else if(!strcmp(argv[i],"-frameskip")) {
			chr_check_result = str_chrchk(argv[i+1],"0123456789");

			if((chr_check_result > 0) && (strlen(argv[i+1]) == chr_check_result)) {
				frameskip = atoi(argv[i+1]);

				i++;
			} else {
				printf("[!] error: frameskip option must be a nummeric value!\n\n");

				usage(argv[0]);
			}
		} else if(!strcmp(argv[i],"-scale")) {
			chr_check_result = str_chrchk(argv[i+1],"0123456789");

			if((chr_check_result > 0) && (strlen(argv[i+1]) == chr_check_result)) {
				scale = atoi(argv[i+1]);

				i++;
			} else {
				printf("[!] error: scale option must be a nummeric value!\n\n");

				usage(argv[0]);
			}
		} else if(!strcmp(argv[i],"-fullscreen")) {
			fullscreen = 1;
		} else if(!strcmp(argv[i],"-delay")) {
			chr_check_result = str_chrchk(argv[i+1],"0123456789");

			if((chr_check_result > 0) && (strlen(argv[i+1]) == chr_check_result)) {
				sdl_delay = atoi(argv[i+1]);

				i++;
			} else {
				printf("[!] error: delay option must be a nummeric value!\n\n");

				usage(argv[0]);
			}
		} else if(!strcmp(argv[i],"-pal")) {
			pal = 1;
			ntsc = 0;
		} else if(!strcmp(argv[i],"-ntsc")) {
			pal = 0;
			ntsc = 1;
		} else {
			printf("[!] error: unknown option %s\n\n",argv[i]);

			usage(argv[0]);
		}
	}

	/* 64k main memory */
	memory = (unsigned char *)malloc(65536);

	/* 16k video memory */
	ppu_memory = (unsigned char *)malloc(16384);

	/* 256b sprite memory */
	sprite_memory = (unsigned char *)malloc(256);

	if(analyze_header(romfn) == 1) {
		free(sprite_memory);
		free(ppu_memory);
		free(memory);
		exit(1);
	}

	/* rom cache memory */
	romcache = (unsigned char *)malloc(romlen);

	printf("[*] PRG = %x, CHR = %x, OS_MIRROR = %d, FS_MIRROR = %d, TRAINER = %d"
		", SRAM = %d, MIRRORING = %d\n",
		PRG,CHR,OS_MIRROR, FS_MIRROR,TRAINER,SRAM,MIRRORING);

	printf("[*] mapper: %d found!\n",MAPPER);

	if(load_rom(romfn) == 1) {
		free(sprite_memory);
		free(ppu_memory);
		free(memory);
		free(romcache);
		exit(1);
	}

	if(MAPPER == 4)
		mmc3_reset();

	statefile = str_replace(romfn,".nes",".sst");
	savfile = str_replace(romfn,".nes",".sav");

	if(SRAM == 1) {
		open_sav();
	}

	if(pal == 1) {
		height = 240;
		width = 256;
	}

	if(ntsc == 1) {
		height = 224;
		width = 256;
	}

	sdl_screen_height = height * scale;
	sdl_screen_width = width * scale;

	if(pal == 1) {
		printf("[*] PAL_SPEED: %d\n",PAL_SPEED);
		printf("[*] PAL_VBLANK_INT: %d\n",PAL_VBLANK_INT);
		printf("[*] PAL_SCANLINE_REFRESH: %d\n",PAL_SCANLINE_REFRESH);
		printf("[*] PAL_VBLANK_CYCLE_TIMEOUT: %d\n",PAL_VBLANK_CYCLE_TIMEOUT);
		printf("[*] height * PAL_SCANLINE_REFRESH: %d\n",(height * PAL_SCANLINE_REFRESH) + PAL_VBLANK_CYCLE_TIMEOUT + 341);
	}

	if(ntsc == 1) {
		printf("[*] NTSC_SPEED: %d\n",NTSC_SPEED);
		printf("[*] NTSC_VBLANK_INT: %d\n",NTSC_VBLANK_INT);
		printf("[*] NTSC_SCANLINE_REFRESH: %d\n",NTSC_SCANLINE_REFRESH);
		printf("[*] NTSC_VBLANK_CYCLE_TIMEOUT: %d\n",NTSC_VBLANK_CYCLE_TIMEOUT);
		printf("[*] height * NTSC_SCANLINE_REFRESH: %d\n",(height * NTSC_SCANLINE_REFRESH) + NTSC_VBLANK_CYCLE_TIMEOUT + 341);
	}

	printf("[*] setting screen resolution to: %dx%d\n",sdl_screen_width,sdl_screen_height);

	DisplayType display_type = DisplayTypePAL;
	if (ntsc == 1) {
		display_type = DisplayTypeNTSC;
	}

	display_init(sdl_screen_width, sdl_screen_height, display_type, fullscreen == 1);

	printf("[*] resetting cpu...\n");

	/*
	 * first reset the cpu at poweron
	 */
	CPU_reset();

	/*
	 * reset joystick
	 */
	reset_input();

	printf("[*] LameNES starting emulation!\n");

	if(startdebugger == 2) {
		disassemble = 1;
		hit_break = 1;
		debugger();
	}

	if(pal == 1) {
		start_int = 341;
		vblank_int = PAL_VBLANK_INT;
		vblank_cycle_timeout = PAL_VBLANK_CYCLE_TIMEOUT;
		scanline_refresh = PAL_SCANLINE_REFRESH;
	}

	if(ntsc == 1) {
		start_int = 325;
		vblank_int = NTSC_VBLANK_INT;
		vblank_cycle_timeout = NTSC_VBLANK_CYCLE_TIMEOUT;
		scanline_refresh = NTSC_SCANLINE_REFRESH;
	}

	while(1) {
		if(!pause_emulation)
			start_emulation();

		poll_buttons();
	}

	/* never reached */
	return(0);
}
