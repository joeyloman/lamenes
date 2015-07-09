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

/*
 * debugger.c - LameNES internal debugger
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "lame6502.h"
#include "disas.h"
#include "instructions.h"

#include "../lamenes.h"
#include "../ppu.h"
#include "../input.h"
#include "../macros.h"

#include "../lib/str_cut.h"
#include "../lib/str_chrchk.h"

void
dump_mem(char *dump_fn)
{
	FILE *dump_fp;

	dump_fp=fopen(dump_fn,"wb");
	fwrite(&memory[0x0000],1,65536,dump_fp);
	fclose(dump_fp);
}

void
dump_ppu_mem(char *ppu_dump_fn)
{
	FILE *dump_fp;

	dump_fp=fopen(ppu_dump_fn,"wb");
	fwrite(&ppu_memory[0x0000],1,16384,dump_fp);
	fclose(dump_fp);
}

void
dump_sprite_mem(char *sprite_dump_fn)
{
	FILE *dump_fp;

	dump_fp=fopen(sprite_dump_fn,"wb");
	fwrite(&sprite_memory[0x0000],1,256,dump_fp);
	fclose(dump_fp);
}

void
dump_stack(char *stack_dump_fn)
{
	FILE *dump_fp;

	dump_fp=fopen(stack_dump_fn,"wb");
	fwrite(&memory[0x0100],1,256,dump_fp);
	fclose(dump_fp);
}

void
dump_ppu_nam(char *ppu_dump_fn)
{
	FILE *dump_fp;

	dump_fp=fopen(ppu_dump_fn,"wb");
	fwrite(&ppu_memory[0x2000],1,4096,dump_fp);
	fclose(dump_fp);
}

void
dump_ppu_palette(char *ppu_dump_fn)
{
	FILE *dump_fp;

	dump_fp=fopen(ppu_dump_fn,"wb");
	fwrite(&ppu_memory[0x3F00],1,32,dump_fp);
	fclose(dump_fp);
}

void
dump_nesram(char *dump_fn)
{
	FILE *dump_fp;

	dump_fp=fopen(dump_fn,"wb");
	fwrite(&memory[0x0000],1,2048,dump_fp);
	fclose(dump_fp);
}

void
dump_ppuram(char *dump_fn)
{
	FILE *dump_fp;

	dump_fp=fopen(dump_fn,"wb");
	fwrite(&ppu_memory[0x0000],1,8192,dump_fp);
	fclose(dump_fp);
}

void
show_mem(char *option, unsigned int address)
{
	int c = 16;
	int i = 0;
	int j = 0;
	int k = 0;

	if(!strcmp(option,"main")) {
		if((0xffff - address) < 16) {
			c = 0x10000 - address;
		}

		for(i = 0; i < 16; i++) {
			if(address+i+k > 0xffff)
				return;

			printf("%.4x:",address+k);

			for(j = 0; j < c; j++) {
				printf(" %.2x",memory[address+j+k]);
			}

			printf(" | ");

			for(j = 0; j < c; j++) {
				if(isprint(memory[address+j+k])) {
					printf("%c", memory[address+j+k]);
				} else {
					printf(".");
				}
			}

			printf("\n");

			k+=16;
		}
	} else if(!strcmp(option,"ppu")) {
		if((0x3fff - address) < 16) {
			c = 0x4000 - address;
		}

		for(i = 0; i < 16; i++) {
			if(address+i+k > 0x3fff)
				return;

			printf("%.4x:",address+k);

			for(j = 0; j < c; j++) {
				printf(" %.2x",ppu_memory[address+j+k]);
			}

			printf(" | ");

			for(j = 0; j < c; j++) {
				if(isprint(ppu_memory[address+j+k])) {
					printf("%c", ppu_memory[address+j+k]);
				} else {
					printf(".");
				}
			}

			printf("\n");

			k+=16;
		}
	} else if(!strcmp(option,"sprite")) {
		if((0xff - address) < 16) {
			c = 0x100 - address;
		}

		for(i = 0; i < 16; i++) {
			if(address+i+k > 0xff)
				return;

			printf("%.4x:",address+k);

			for(j = 0; j < c; j++) {
				printf(" %.2x",sprite_memory[address+j+k]);
			}

			printf(" | ");

			for(j = 0; j < c; j++) {
				if(isprint(sprite_memory[address+j+k])) {
					printf("%c", sprite_memory[address+j+k]);
				} else {
					printf(".");
				}
			}

			printf("\n");

			k+=16;
		}
	} else {
		return;
	}
}

void
show_sprite_attribs()
{
	int i;

	for(i = 0; i < 64; i++) {
		if((sprite_memory[i * 4 + 1] != 0) && (sprite_memory[i * 4] < height)) {
			printf("sprite %d: [y: %d] [x: %d] [pattern: %x] [attrib: %x]\n",
				i,sprite_memory[i * 4],sprite_memory[i * 4 + 3], 
				sprite_memory[i * 4 + 1],sprite_memory[i * 4 + 2]);
		}
	}
}

void
debugger()
{
	char cmd[8];
	char subcmd[256];

	char *subopt1;
	char *subopt2;

	int chr_check_result;
	int sublen;
	int opt = 0;
	int menu = 1;

	while(menu) {
		memset(cmd,0,sizeof(cmd));
		memset(subcmd,0,sizeof(subcmd));
		printf("\n");
		printf("LameNES debugger command>");

		if(fgets(cmd, 5,stdin) != NULL) {
			if(cmd[0] != '\0' && cmd[strlen(cmd) - 1] == '\n')
				cmd[strlen(cmd) - 1] = '\0';
		}

		switch(cmd[0]) {
			case 'b':
			printf("\nenter breakpoint: 0x");

			if(fgets(subcmd, 7,stdin) != NULL) {
				if(subcmd[0] != '\0' && subcmd[strlen(subcmd) - 1] == '\n')
					subcmd[strlen(subcmd) - 1] = '\0';
			}

			sublen = strlen(subcmd);

			if(sublen > 5) {
				printf("error: address range must be between 0x0000 and 0xFFFF\n");
			} else {
				chr_check_result = str_chrchk(subcmd,"0123456789abcdef");
				if((chr_check_result > 0) && (strlen(subcmd) == chr_check_result)) {
					breakpoint = strtoul(subcmd, NULL, 16);
				} else {
					printf("error: unknown memory address given!\n");
				}
			}
			break;

			case 'c':
			printf("continue with emulation!\n");
			hit_break = 0;
			disassemble = 0;
			menu = 0;
			break;

			case 'd':
			if(stackdebug == 1) {
				printf("stack debugging disabled!\n");
				stackdebug = 0;
			} else {
				printf("stack debugging enabled!\n");
				stackdebug = 1;
			}
			break;

			case 'f':
			printf("[%d] ppu_control1: %x\n",debug_cnt,ppu_control1);
			printf("[%d] ppu_control2: %x\n",debug_cnt,ppu_control2);
			printf("[%d] ppu_status: %x\n",debug_cnt,ppu_status);
			printf("[%d] ppu_status_ret: %x\n",debug_cnt,(ppu_status & 0xE0) | (ppu_addr_tmp & 0x1F));

			printf("[%d] current scanline: %d\n",debug_cnt,current_scanline);
			printf("[%d] current nametable: %x\n",debug_cnt,0x2000 + (loopyV & 0x0fff));
			printf("[%d] loopyT: %x\n",debug_cnt,loopyT);
			printf("[%d] loopyV: %x\n",debug_cnt,loopyV);
			printf("[%d] loopyX: %x\n",debug_cnt,loopyX);

			printf("[%d] exec_nmi_on_vblank: %x\n",debug_cnt,exec_nmi_on_vblank);
			printf("[%d] sprite_16: %d\n",debug_cnt,sprite_16);
			printf("[%d] background_addr_hi: %x\n",debug_cnt,background_addr_hi);
			printf("[%d] sprite_addr_hi: %d\n",debug_cnt,sprite_addr_hi);
			printf("[%d] increment_32: %d\n",debug_cnt,increment_32);

			printf("[%d] sprite_on: %d\n",debug_cnt,sprite_on);
			printf("[%d] background_on: %d\n",debug_cnt,background_on);
			printf("[%d] sprite_clipping_off: %d\n",debug_cnt,sprite_clipping_off);
			printf("[%d] background_clipping_off: %d\n",debug_cnt,background_clipping_off);
			printf("[%d] monochrome_on: %d\n",debug_cnt,monochrome_on);

			printf("[%d] vblank_on: %d\n",debug_cnt,vblank_on);
			printf("[%d] sprite_zero: %d\n",debug_cnt,sprite_zero);
			printf("[%d] scanline_sprite_count: %d\n",debug_cnt,scanline_sprite_count);
			printf("[%d] vram_write_flag: %d\n",debug_cnt,vram_write_flag);
			break;

			case 'h':
			printf("------------------------------\n");
			printf("LameNES debugger command list:\n");
			printf("------------------------------\n");
			printf("(b)reakpoint -> set breakpoint\n");
			printf("(c)ontinue -> continue emulation\n");
			printf("(d)ebug stack operations -> enable/disable stack debugging\n");
			printf("(i)nstruction counter break -> set instruction counter breakpoint\n");
			printf("(f)lags states -> show all states of current flags\n");
			printf("(h)elp -> this screen\n");
			printf("(l)oad state -> loads emulation state\n");
			printf("(m)emorydump -> show or dump the complete nes memory\n");
			printf("(p)pu status -> show ppu status\n");
			printf("(q)uit -> quit LameNES\n");
			printf("(s)ave state -> saves emulation state\n");
			printf("(u)pdate controller io -> send joypad1 io\n");
			printf("(anything else) -> executes the next instruction\n");
			break;

			case 'i':
			printf("\nenter instruction counter breakpoint: ");

			if(fgets(subcmd, 255,stdin) != NULL) {
				if(subcmd[0] != '\0' && subcmd[strlen(subcmd) - 1] == '\n')
					subcmd[strlen(subcmd) - 1] = '\0';
			}

			sublen = strlen(subcmd);

			if(sublen > 255) {
				printf("error: counter too large!\n");
			} else {
				stop_at_debug_cnt = atoi(subcmd);
			}
			break;

			case 'l':
			load_state();
			break;

			case 'm':
			printf("memory options:\n"
			"m <memory address> -> display main ram from given address + next 100 bytes\n"
			"p <memory address> -> display ppu ram from given address + next 100 bytes\n"
			"s <memory address> -> display sprite ram from given address + next 100 bytes\n"
			"a -> dump all memory registers to bin files\n"
			"enter option: "
			);

			if(fgets(subcmd, 255,stdin) != NULL) {
				if(subcmd[0] != '\0' && subcmd[strlen(subcmd) - 1] == '\n')
					subcmd[strlen(subcmd) - 1] = '\0';
			}

			sublen = strlen(subcmd);

			if(sublen > 10) {
				printf("error: option string too large\n");
				break;
			}

			switch(subcmd[0]) {
				case 'a':
				printf("dumping main memory to: lamenes_nesmem.bin\n");
				dump_mem("lamenes_nesmem.bin");

				printf("dumping ppu memory to: lamenes_ppumem.bin\n");
				dump_ppu_mem("lamenes_ppumem.bin");

				printf("dumping sprite memory to: lamenes_spritemem.bin\n");
				dump_sprite_mem("lamenes_spritemem.bin");

				printf("dumping nametables to: lamenes_nametables.bin\n");
				dump_ppu_nam("lamenes_nametables.bin");

				printf("dumping palette to: lamenes_palette.bin\n");
				dump_ppu_palette("lamenes_palette.bin");

				printf("dumping main ram to: lamenes_nesram.bin\n");
				dump_nesram("lamenes_nesram.bin");

				printf("dumping ppu ram to: lamenes_ppuram.bin\n");
				dump_ppuram("lamenes_ppuram.bin");

				printf("dumping stack to: lamenes_stackdump.bin\n");
				dump_stack("lamenes_stackdump.bin");
				break;

				case 'm':
				subopt1 = str_cut(subcmd," ");
				subopt2 = str_cut(NULL," ");

				chr_check_result = str_chrchk(subopt2,"0123456789abcdef");

				if((chr_check_result > 0) && (strlen(subopt2) == chr_check_result)) {
					opt = strtoul(subopt2, NULL, 16);
					show_mem("main",opt);
				} else {
					printf("error: unknown memory address given!\n");
				}
				break;

				case 'p':
				subopt1 = str_cut(subcmd," ");
				subopt2 = str_cut(NULL," ");

				chr_check_result = str_chrchk(subopt2,"0123456789abcdef");

				if((chr_check_result > 0) && (strlen(subopt2) == chr_check_result)) {
					opt = strtoul(subopt2, NULL, 16);

					show_mem("ppu",opt);
				} else {
					printf("error: unknown memory address given!\n");
				}
				break;

				case 's':
				subopt1 = str_cut(subcmd," ");
				subopt2 = str_cut(NULL," ");

				chr_check_result = str_chrchk(subopt2,"0123456789abcdef");

				if((chr_check_result > 0) && (strlen(subopt2) == chr_check_result)) {
					opt = strtoul(subopt2, NULL, 16);

					show_mem("sprite",opt);
				} else {
					printf("error: unknown memory address given!\n");
				}
				break;

				default:
				break;
			}
			break;

			case 'p':
			printf("ppu status options:\n"
			"(s)prite attributes -> display current sprite attributes\n"
			"enter option: "
			);

			if(fgets(subcmd, 255,stdin) != NULL) {
				if(subcmd[0] != '\0' && subcmd[strlen(subcmd) - 1] == '\n')
					subcmd[strlen(subcmd) - 1] = '\0';
			}

			sublen = strlen(subcmd);

			if(sublen > 10) {
				printf("error: option string too large\n");
				break;
			}

			switch(subcmd[0]) {
				case 's':
				show_sprite_attribs();
				break;

				default:
				break;
			}
			break;

			case 'q':
			quit_emulation();
			break;

			case 's':
			save_state();
			break;

			case 'u':
			printf("\nchoose the following joypad1 oi command to send:\n"
			"1 -> down\n"
			"2 -> up\n"
			"3 -> left\n"
			"4 -> right\n"
			"5 -> start\n"
			"6 -> select\n"
			"7 -> A\n"
			"8 -> B\n"
			"enter selection: ");

			if(fgets(subcmd, 2,stdin) != NULL) {
				if(subcmd[0] != '\0' && subcmd[strlen(subcmd) - 1] == '\n')
					subcmd[strlen(subcmd) - 1] = '\0';
			}

			sublen = strlen(subcmd);

			if(sublen > 2) {
				printf("error: too many arguments!\n");
				break;
			}

			chr_check_result = str_chrchk(subcmd,"12345678");

			if((chr_check_result > 0) && (strlen(subcmd) == chr_check_result)) {
				opt = atoi(subcmd);

				switch(opt) {
					/* down */
					case 1:
					printf("sending joypad1 [down] to io.\n");
					set_input(1);
					break;

					/* up */
					case 2:
					printf("sending joypad1 [up] to io.\n");
					set_input(2);
					break;

					/* left */
					case 3:
					printf("sending joypd1 [left] to io.\n");
					set_input(3);
					break;

					/* right */
					case 4:
					printf("sending joypad1 [right] to io.\n");
					set_input(4);
					break;

					/* start */
					case 5:
					printf("sending joypad1 [start] to io.\n");
					set_input(5);
					break;

					/* select */
					case 6:
					printf("sending joypad1 [select] to io.\n");
					set_input(6);
					break;

					/* A */
					case 7:
					printf("sending joypad1 [A] to io.\n");
					set_input(7);
					break;

					/* B */
					case 8:
					printf("sending joypad1 [B] to io.\n");
					set_input(8);
					break;

					default:
					printf("error: unknown option!\n");
					break;
				}
			} else {
				printf("error: wrong argument!\n");
			}
			break;

			case 'v':
			printf("interrupt vectors:\n\n");
			printf("INIT [0xfffc] -> 0x%x\n",(memory[0xfffd] << 8) | memory[0xfffc]);
			printf("IRQ [0xfffe] -> 0x%x\n",(memory[0xffff] << 8) | memory[0xfffe]);
			printf("NMI [0xfffa] -> 0x%x\n",(memory[0xfffb] << 8) | memory[0xfffa]);
			break;

			default:
			printf("executing next instruction!\n");
			menu = 0;
			break;
		}
	}
}
