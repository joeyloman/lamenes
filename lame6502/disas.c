/*
 * lame6502 - a portable 6502 emulator
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
 * disas.c - 6502 disassembler
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lame6502.h"

int debug_cnt = 0;
int show_debug_cnt = 0;

void
disas(char *name, char *type)
{
	if(debug_cnt > show_debug_cnt) {
		printf("[%d] A:%x, P:%x, X:%x, Y:%x, S:0x0%x, addr:%x\n",
			debug_cnt-1,accumulator,status_register,x_reg,y_reg,stack_pointer+0x100,addr);

		printf("[%d] Z:%d, N:%d, O:%d, B:%d, D:%d, I:%d, C:%d\n",
			debug_cnt-1,zero_flag,sign_flag,overflow_flag,break_flag,decimal_flag,interrupt_flag,carry_flag);
	}

	if(debug_cnt > show_debug_cnt) {
		if(!strcmp(type,"NODATA")) {
			printf("[%d] executing instruction at offset 0x%x: [0x%x - %s]\n",debug_cnt-1,program_counter-1,
				memory[program_counter-1],name);

			return;
		}

		if(!strcmp(type,"ACC")) {
			printf("[%d] executing instruction at offset 0x%x: [0x%x - %s]\tA\n",debug_cnt-1,program_counter-1,
				memory[program_counter-1],name);

			return;
		}

		if(!strcmp(type,"aa")) {
			printf("[%d] executing instruction at offset 0x%x: [0x%x - %s]\t%x (%x + %d)\n",
				debug_cnt-1,program_counter-1,memory[program_counter-1],name,
				(program_counter + (signed char)memory[program_counter]) + 1,program_counter + 1,
				(signed char)memory[program_counter]);
		}

		if(!strcmp(type,"IM")) {
			printf("[%d] executing instruction at offset 0x%x: [0x%x - %s]\t#%x\n",debug_cnt-1,program_counter-1,
				memory[program_counter-1],name,memory[program_counter]);

			return;
		}

		if(!strcmp(type,"ZP")) {
			printf("[%d] executing instruction at offset 0x%x: [0x%x - %s]\t%x [mem value: %x]\n",debug_cnt-1,
				program_counter-1, memory[program_counter-1],name,memory[program_counter],
				memory[memory[program_counter]]);

			return;
		}

		if(!strcmp(type,"ZPIX")) {
			printf("[%d] executing instruction at offset 0x%x: [0x%x - %s]\t%x, X [mem value: %x]\n",debug_cnt-1,
				program_counter-1,memory[program_counter-1],name,memory[program_counter],
				memory[memory[program_counter] + x_reg]);

			return;
		}

		if(!strcmp(type,"ZPIY")) {
			printf("[%d] executing instruction at offset 0x%x: [0x%x - %s]\t%x, Y [mem value: %x]\n",debug_cnt-1,
				program_counter-1,memory[program_counter-1],name,memory[program_counter],
				memory[memory[program_counter] + y_reg]);

			return;
		}

		if(!strcmp(type,"A")) {
			printf("[%d] executing instruction at offset 0x%x: [0x%x - %s]\t%x [mem value: %x]\n",debug_cnt-1,
				program_counter-1,memory[program_counter-1],name,
				(memory[program_counter+1] << 8) | memory[program_counter],
				memory[(memory[program_counter+1] << 8) | memory[program_counter]]);

			return;
		}

		if(!strcmp(type,"AI")) {
			printf("[%d] executing instruction at offset 0x%x: [0x%x - %s]\t(%x)\n",debug_cnt-1,program_counter-1,
				memory[program_counter-1],name,(memory[program_counter+1] << 8) | memory[program_counter]);

			return;
		}

		if(!strcmp(type,"AIX")) {
			printf("[%d] executing instruction at offset 0x%x: [0x%x - %s]\t%x, X [mem value: %x]\n",debug_cnt-1,
				program_counter-1,memory[program_counter-1],name,
				(memory[program_counter+1] << 8) | memory[program_counter],
				memory[((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg]);

			return;
		}

		if(!strcmp(type,"AIY")) {
			printf("[%d] executing instruction at offset 0x%x: [0x%x - %s]\t%x, Y [mem value: %x]\n",debug_cnt-1,
				program_counter-1,memory[program_counter-1],name,
				(memory[program_counter+1] << 8) | memory[program_counter],
				memory[((memory[program_counter+1] << 8) | memory[program_counter]) + y_reg]);
			return;
		}

		if(!strcmp(type,"IDI")) {
			printf("[%d] executing instruction at offset 0x%x: [0x%x - %s]\t(%x, X) [mem location: %x]\n",debug_cnt-1,
				program_counter-1,memory[program_counter-1],name,memory[program_counter],
				(memory[memory[program_counter] + x_reg + 1] << 8) | memory[memory[program_counter] + x_reg]);

			return;
		}

		if(!strcmp(type,"INI")) {
			printf("[%d] executing instruction at offset 0x%x: [0x%x - %s]\t(%x), Y [mem location: %x]\n",debug_cnt-1,
				program_counter-1,memory[program_counter-1],name,memory[program_counter],
				((memory[memory[program_counter] + 1] << 8) | memory[memory[program_counter]]) + y_reg);

			return;
		}
	}

	return;
}
