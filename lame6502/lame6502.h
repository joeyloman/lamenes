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
 * lame6502.h - 6502 cpu functions and registers
 */

extern unsigned char *memory;

extern unsigned int program_counter;

extern unsigned char stack_pointer;
extern unsigned char status_register;
extern unsigned char x_reg;
extern unsigned char y_reg;
extern unsigned char accumulator;

extern unsigned int addr;
extern unsigned int tmp, tmp2, tmp3;

extern int zero_flag;
extern int sign_flag;
extern int overflow_flag;
extern int break_flag;
extern int decimal_flag;
extern int interrupt_flag;
extern int carry_flag;

extern int disassemble;
extern int hit_break;
extern int startdebugger;

extern int breakpoint;
extern int stop_at_debug_cnt;
extern int stackdebug;

//extern unsigned char memory_read(unsigned int address);
//extern void write_memory(unsigned int address,unsigned char data);

extern int IRQ(int cycles);
extern int NMI(int cycles);
extern void CPU_reset(void);
extern int CPU_execute(int cycles);
