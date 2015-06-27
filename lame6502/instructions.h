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
 * instructions.h - 6502 cpu instruction macros
 */

#define ADC_IM(CYCLES)		{ addr = memory[program_counter]; \
					tmp = accumulator + addr + (carry_flag ? 1 : 0); \
					overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80; \
					carry_flag = tmp > 0xFF; \
					accumulator = tmp & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter++; \
					cycle_count -= CYCLES; \
					break; }

#define ADC_ZP(CYCLES)		{ addr = memory_read(memory[program_counter]); \
					tmp = accumulator + addr + (carry_flag ? 1 : 0); \
					overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80; \
					carry_flag = tmp > 0xFF; \
					accumulator = tmp & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter++; \
					cycle_count -= CYCLES; \
					break; }

#define ADC_ZPIX(CYCLES)	{ addr = memory_read(memory[program_counter] + x_reg); \
					tmp = accumulator + addr + (carry_flag ? 1 : 0); \
					overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80; \
					carry_flag = tmp > 0xFF; \
					accumulator = tmp & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter++; \
					cycle_count -= CYCLES; \
					break; }

#define ADC_A(CYCLES)		{ addr = memory_read((memory[program_counter+1] << 8) | memory[program_counter]); \
					tmp = accumulator + addr + (carry_flag ? 1 : 0); \
					overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80; \
					carry_flag = tmp > 0xFF; \
					accumulator = tmp & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter+=2; \
					cycle_count -= CYCLES; \
					break; }

#define ADC_AIX(CYCLES)		{ addr = memory_read(((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg); \
					tmp = accumulator + addr + (carry_flag ? 1 : 0); \
					overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80; \
					carry_flag = tmp > 0xFF; \
					accumulator = tmp & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter+=2; \
					cycle_count -= CYCLES; \
					break; }

#define ADC_AIY(CYCLES)		{ addr = memory_read(((memory[program_counter+1] << 8) | memory[program_counter]) + y_reg); \
					tmp = accumulator + addr + (carry_flag ? 1 : 0); \
					overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80; \
					carry_flag = tmp > 0xFF; \
					accumulator = tmp & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter+=2; \
					cycle_count -= CYCLES; \
					break; }

#define ADC_IDI(CYCLES)	{ addr = memory_read(memory[program_counter] + x_reg); \
					tmp = (memory[addr + 1] << 8) | memory[addr]; \
					tmp2 = memory_read(tmp); \
					tmp3 = accumulator + tmp2 + (carry_flag ? 1 : 0); \
					overflow_flag = (~(accumulator ^ tmp2)) & (accumulator ^ tmp2) & 0x80; \
					carry_flag = tmp3 > 0xFF; \
					accumulator = tmp3 & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter++; \
					cycle_count -= CYCLES; \
					break; }

#define ADC_INI(CYCLES)	{ addr = memory[program_counter]; \
					tmp = ((memory[addr + 1] << 8) | memory[addr]) + y_reg; \
					tmp2 = memory_read(tmp); \
					tmp3 = accumulator + tmp2 + (carry_flag ? 1 : 0); \
					overflow_flag = (~(accumulator ^ tmp2)) & (accumulator ^ tmp2) & 0x80; \
					carry_flag = tmp3 > 0xFF; \
					accumulator = tmp3 & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter++; \
					cycle_count -= CYCLES; \
					break; }

#define AND_IM(CYCLES)		{ accumulator &= memory[program_counter]; \
					program_counter++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define AND_ZP(CYCLES)		{ addr = memory[program_counter]; \
					accumulator &= memory_read(addr); \
					program_counter ++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define AND_ZPIX(CYCLES)	{ addr = memory[program_counter] + x_reg; \
					accumulator &= memory_read(addr); \
					program_counter ++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define AND_A(CYCLES)		{ addr = (memory[program_counter+1] << 8) | memory[program_counter]; \
					accumulator &= memory_read(addr); \
					program_counter += 2; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define AND_AIX(CYCLES)		{ tmp = (memory[program_counter+1] << 8) | memory[program_counter]; \
					addr = tmp + x_reg; \
					accumulator &= memory_read(addr); \
					program_counter += 2; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define AND_AIY(CYCLES)		{ tmp = (memory[program_counter+1] << 8) | memory[program_counter]; \
					addr = tmp + y_reg; \
					accumulator &= memory_read(addr); \
					program_counter += 2; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define AND_IDI(CYCLES)		{ addr = memory[program_counter] + x_reg; \
					tmp = ((memory[addr + 1] << 8) | memory[addr]); \
					accumulator &= memory_read(tmp); \
					program_counter ++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define AND_INI(CYCLES)		{ addr = memory[program_counter]; \
					tmp = ((memory[addr + 1] << 8) | memory[addr]) + y_reg; \
					accumulator &= memory_read(tmp); \
					program_counter ++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define ARITH_SL_ACC(CYCLES)	{ carry_flag = (carry_flag & 0xfe) | ((accumulator >> 7) & 0x01); \
					accumulator = accumulator << 1; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define ARITH_SL_ZP(CYCLES)	{ tmp = memory[program_counter]; \
					addr = memory_read(tmp); \
					carry_flag = (carry_flag & 0xfe) | ((addr >> 7) & 0x01); \
					addr = addr << 1; \
					write_memory(tmp,addr); \
					sign_flag = addr & 0x80; \
					zero_flag = !(addr); \
					program_counter ++; \
					cycle_count -= CYCLES; \
					break; }

#define ARITH_SL_ZPIX(CYCLES)	{ tmp = memory[program_counter] + x_reg; \
					addr = memory_read(tmp); \
					carry_flag = (carry_flag & 0xfe) | ((addr >> 7) & 0x01); \
					addr = addr << 1; \
					write_memory(tmp,addr); \
					sign_flag = addr & 0x80; \
					zero_flag = !(addr); \
					program_counter ++; \
					cycle_count -= CYCLES; \
					break; }

#define ARITH_SL_A(CYCLES)	{ tmp = (memory[program_counter+1] << 8) | memory[program_counter]; \
					addr = memory_read(tmp); \
					carry_flag = (carry_flag & 0xfe) | ((addr >> 7) & 0x01); \
					addr = addr << 1; \
					write_memory(tmp,addr); \
					sign_flag = addr & 0x80; \
					zero_flag = !(addr); \
					program_counter +=2; \
					cycle_count -= CYCLES; \
					break; }

#define ARITH_SL_AIX(CYCLES)	{ tmp = ((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg; \
					addr = memory_read(tmp); \
					carry_flag = (carry_flag & 0xfe) | ((addr >> 7) & 0x01); \
					addr = addr << 1; \
					write_memory(tmp,addr); \
					sign_flag = addr & 0x80; \
					zero_flag = !(addr); \
					program_counter +=2; \
					cycle_count -= CYCLES; \
					break; }

#define BRANCH_CC(CYCLES)	{ program_counter++; \
					if(!carry_flag) program_counter += (signed char)memory[program_counter - 1]; \
					cycle_count -= CYCLES; \
					break; }

#define BRANCH_CS(CYCLES)	{ program_counter++; \
					if(carry_flag) program_counter += (signed char)memory[program_counter - 1]; \
					cycle_count -= CYCLES; \
					break; }

#define BRANCH_ZS(CYCLES)	{ program_counter++; \
					if(zero_flag) program_counter += (signed char)memory[program_counter - 1]; \
					cycle_count -= CYCLES; \
					break; }

#define BIT_TEST_ZP(CYCLES)	{ addr = memory[program_counter]; \
					tmp = memory_read(addr); \
					tmp2 = tmp & accumulator; \
					sign_flag = tmp & 0x80; \
					overflow_flag = ((tmp & 0x40) != 0); \
					zero_flag = tmp2 ? 0 : 1; \
					program_counter++; \
					cycle_count -= CYCLES; \
					break; }

#define BIT_TEST_A(CYCLES)	{ addr = (memory[program_counter+1] << 8) | memory[program_counter]; \
					tmp = memory_read(addr); \
					tmp2 = tmp & accumulator; \
					sign_flag = tmp & 0x80; \
					overflow_flag = ((tmp & 0x40) != 0); \
					zero_flag = tmp2 ? 0 : 1; \
					program_counter+=2; \
					cycle_count -= CYCLES; \
					break; }

#define BRANCH_RM(CYCLES)	{ program_counter++; \
					if(sign_flag) program_counter += (signed char)memory[program_counter - 1]; \
					cycle_count -= CYCLES; \
					break; }

#define BRANCH_ZR(CYCLES)	{ program_counter++; \
					if(!zero_flag) program_counter += (signed char)memory[program_counter - 1]; \
					cycle_count -= CYCLES; \
					break; }

#define BRANCH_RP(CYCLES)	{ program_counter++; \
					if(!sign_flag) program_counter += (signed char)memory[program_counter - 1]; \
					cycle_count -= CYCLES; \
					break; }

#define BREAK(CYCLES)		{ program_counter ++; \
					break_flag = 1; \
					PUSH_ST((program_counter & 0xFF00) >> 8); \
					PUSH_ST(program_counter & 0xFF); \
					PUSH_ST(GET_SR()); \
					interrupt_flag = 1; \
					program_counter = (memory[0xFFFF] << 8) | memory[0xFFFE]; \
					cycle_count -= CYCLES; \
					break; }

#define BRANCH_OC(CYCLES)	{ program_counter++; \
					if(!overflow_flag) program_counter += (signed char)memory[program_counter - 1]; \
					cycle_count -= CYCLES; \
					break; }

#define BRANCH_OS(CYCLES)	{ program_counter++; \
					if(overflow_flag) program_counter += (signed char)memory[program_counter - 1]; \
					cycle_count -= CYCLES; \
					break; }

#define CLEAR_CF(CYCLES)	{ carry_flag = 0; \
					cycle_count -= CYCLES; break; }

#define CLEAR_DM(CYCLES)	{ decimal_flag = 0; \
					cycle_count -= CYCLES; break; }

#define CLEAR_ID(CYCLES)	{ interrupt_flag = 0; \
					cycle_count -= CYCLES; break; }

#define CLEAR_OF(CYCLES)	{ overflow_flag = 0; \
					cycle_count -= CYCLES; break; }

#define COMP_MEM_IM(REG,CYCLES)		{ addr = memory[program_counter]; \
						carry_flag = (REG >= addr) ? 1 : 0; \
						sign_flag = ((signed char)REG < (signed char)addr) ? 1 : 0; \
						zero_flag = (REG == addr) ? 1 : 0; \
						program_counter++; \
						cycle_count -= CYCLES; \
						break; }

#define COMP_MEM_ZP(REG,CYCLES)		{ addr = memory[program_counter]; \
						tmp = memory_read(addr); \
						carry_flag = (REG >= tmp) ? 1 : 0; \
						sign_flag = ((signed char)REG < (signed char)tmp) ? 1 : 0; \
						zero_flag = (REG == tmp) ? 1 : 0; \
						program_counter++; \
						cycle_count -= CYCLES; \
						break; }

#define COMP_MEM_ZPIX(REG,CYCLES)	{ addr = memory[program_counter] + x_reg; \
						tmp = memory_read(addr); \
						carry_flag = (REG >= tmp) ? 1 : 0; \
						sign_flag = ((signed char)REG < (signed char)tmp) ? 1 : 0; \
						zero_flag = (REG == tmp) ? 1 : 0; \
						program_counter++; \
						cycle_count -= CYCLES; \
						break; }

#define COMP_MEM_A(REG,CYCLES)		{ addr = (memory[program_counter+1] << 8) | memory[program_counter]; \
						tmp = memory_read(addr); \
						carry_flag = (REG >= tmp) ? 1 : 0; \
						sign_flag = ((signed char)REG < (signed char)tmp) ? 1 : 0; \
						zero_flag = (REG == tmp) ? 1 : 0; \
						program_counter+=2; \
						cycle_count -= CYCLES; \
						break; }

#define COMP_MEM_AIX(REG,CYCLES)	{ addr = ((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg; \
						tmp = memory_read(addr); \
						carry_flag = (REG >= tmp) ? 1 : 0; \
						sign_flag = ((signed char)REG < (signed char)tmp) ? 1 : 0; \
						zero_flag = (REG == tmp) ? 1 : 0; \
						program_counter+=2; \
						cycle_count -= CYCLES; \
						break; }

#define COMP_MEM_AIY(REG,CYCLES)	{ addr = ((memory[program_counter+1] << 8) | memory[program_counter]) + y_reg; \
						tmp = memory_read(addr); \
						carry_flag = (REG >= tmp) ? 1 : 0; \
						sign_flag = ((signed char)REG < (signed char)tmp) ? 1 : 0; \
						zero_flag = (REG == tmp) ? 1 : 0; \
						program_counter+=2; \
						cycle_count -= CYCLES; \
						break; }

#define COMP_MEM_IDI(REG,CYCLES)	{ addr = memory[program_counter] + x_reg; \
						tmp2 = (memory[addr + 1] << 8) | memory[addr]; \
						tmp = memory_read(tmp2); \
						carry_flag = (REG >= tmp) ? 1 : 0; \
						sign_flag = ((signed char)REG < (signed char)tmp) ? 1 : 0; \
						zero_flag = (REG == tmp) ? 1 : 0; \
						program_counter++; \
						cycle_count -= CYCLES; \
						break; }

#define COMP_MEM_INI(REG,CYCLES)	{ addr = memory[program_counter]; \
						tmp2 = ((memory[addr + 1] << 8) | memory[addr]) + y_reg; \
						tmp = memory_read(tmp2); \
						carry_flag = (REG >= tmp) ? 1 : 0; \
						sign_flag = ((signed char)REG < (signed char)tmp) ? 1 : 0; \
						zero_flag = (REG == tmp) ? 1 : 0; \
						program_counter++; \
						cycle_count -= CYCLES; \
						break; }

#define DECR_MEM_ZP(CYCLES)	{ addr = memory[program_counter]; \
					tmp = memory_read(addr) - 1; \
					write_memory(addr,tmp); \
					sign_flag = memory[addr] & 0x80; \
					zero_flag = !(memory[addr]); \
					program_counter++; \
					cycle_count -= CYCLES; \
					break; }

#define DECR_MEM_ZPIX(CYCLES)	{ addr = memory[program_counter] + x_reg; \
					tmp = memory_read(addr) - 1; \
					write_memory(addr,tmp); \
					sign_flag = memory[addr] & 0x80; \
					zero_flag = !(memory[addr]); \
					program_counter++; \
					cycle_count -= CYCLES; \
					break; }

#define DECR_MEM_A(CYCLES)	{ addr = (memory[program_counter+1] << 8) | memory[program_counter]; \
					tmp = memory_read(addr) - 1; \
					write_memory(addr,tmp); \
					sign_flag = memory[addr] & 0x80; \
					zero_flag = !(memory[addr]); \
					cycle_count -= CYCLES; \
					program_counter+=2; \
					break; }

#define DECR_MEM_AIX(CYCLES)	{ addr = ((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg; \
					tmp = memory_read(addr) - 1; \
					write_memory(addr,tmp); \
					sign_flag = memory[addr] & 0x80; \
					zero_flag = !(memory[addr]); \
					program_counter+=2; \
					cycle_count -= CYCLES; \
					break; }

#define DECR(REG,CYCLES)	{ REG -= 1; \
					sign_flag = REG & 0x80; \
					zero_flag = !(REG); \
					cycle_count -= CYCLES; \
					break; }

#define EXCL_OR_MEM_IM(CYCLES)	{ accumulator ^= memory[program_counter]; \
					program_counter++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define EXCL_OR_MEM_ZP(CYCLES)	{ addr = memory[program_counter]; \
					accumulator ^= memory_read(addr); \
					program_counter ++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define EXCL_OR_MEM_ZPIX(CYCLES)	{ addr = memory[program_counter] + x_reg; \
						accumulator ^= memory_read(addr); \
						program_counter ++; \
						sign_flag = accumulator & 0x80; \
						zero_flag = !(accumulator); \
						cycle_count -= CYCLES; \
						break; }

#define EXCL_OR_MEM_A(CYCLES)	{ addr = (memory[program_counter+1] << 8) | memory[program_counter]; \
					accumulator ^= memory_read(addr); \
					program_counter += 2; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define EXCL_OR_MEM_AIX(CYCLES)	{ addr = ((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg; \
					accumulator ^= memory_read(addr); \
					program_counter += 2; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define EXCL_OR_MEM_AIY(CYCLES)	{ addr = ((memory[program_counter+1] << 8) | memory[program_counter]) + y_reg; \
					accumulator ^= memory_read(addr); \
					program_counter += 2; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define EXCL_OR_MEM_IDI(CYCLES)	{ addr = memory[program_counter] + x_reg; \
					tmp = (memory[addr + 1] << 8) | memory[addr]; \
					accumulator ^= memory_read(tmp); \
					program_counter ++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define EXCL_OR_MEM_INI(CYCLES)	{ addr = memory[program_counter]; \
					tmp = ((memory[addr + 1] << 8) | memory[addr]) + y_reg; \
					accumulator ^= memory_read(tmp); \
					program_counter ++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define INCR_MEM_ZP(CYCLES)	{ addr = memory[program_counter]; \
					tmp = memory_read(addr) + 1; \
					write_memory(addr,tmp); \
					sign_flag = memory[addr] & 0x80; \
					zero_flag = !(memory[addr]); \
					program_counter++; \
					cycle_count -= CYCLES; \
					break; }

#define INCR_MEM_ZPIX(CYCLES)	{ addr = memory[program_counter] + x_reg; \
					tmp = memory_read(addr) + 1; \
					write_memory(addr,tmp); \
					sign_flag = memory[addr] & 0x80; \
					zero_flag = !(memory[addr]); \
					program_counter++; \
					cycle_count -= CYCLES; \
					break; }

#define INCR_MEM_A(CYCLES)	{ addr = (memory[program_counter+1] << 8) | memory[program_counter]; \
					tmp = memory_read(addr) + 1; \
					write_memory(addr,tmp); \
					sign_flag = memory[addr] & 0x80; \
					zero_flag = !(memory[addr]); \
					program_counter+=2; \
					cycle_count -= CYCLES; \
					break; }

#define INCR_MEM_AIX(CYCLES)	{ addr = ((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg; \
					tmp = memory_read(addr) + 1; \
					write_memory(addr,tmp); \
					sign_flag = memory[addr] & 0x80; \
					zero_flag = !(memory[addr]); \
					program_counter+=2; \
					cycle_count -= CYCLES; \
					break; }

#define INCR(REG,CYCLES)	{ REG += 1; \
					sign_flag = REG & 0x80; \
					zero_flag = !(REG); \
					cycle_count -= CYCLES; \
					break; }

/* jump */
#define JMP_A(CYCLES)		{ program_counter = (memory[program_counter+1] << 8) | memory[program_counter]; \
					cycle_count -= CYCLES; \
					break; }

#define JMP_AI(CYCLES)		{ tmp = (memory[program_counter+1] << 8) | memory[program_counter]; \
					tmp2 = memory_read(tmp); \
					tmp = ((memory[program_counter+1] << 8) | memory[program_counter]) + 1; \
					addr = memory_read(tmp); \
					program_counter = (addr << 8) | tmp2; \
					cycle_count -= CYCLES; \
					break; }

/* jump to subroutine */
//if(((program_counter + 2) & 0xFF) == 0) { PUSH_ST(((program_counter + 1) >> 8) + 1); } else { PUSH_ST((program_counter + 1) >> 8); }
#define JSR(CYCLES)		{ PUSH_ST((program_counter + 1) >> 8); \
					PUSH_ST(program_counter + 1); \
					program_counter = (memory[program_counter+1] << 8) | memory[program_counter]; \
					cycle_count -= CYCLES; \
					break; }

#define LOAD_IM(REG, CYCLES)	{ REG = memory[program_counter]; \
					program_counter ++; \
					sign_flag = REG & 0x80; \
					zero_flag = !(REG); \
					cycle_count -= CYCLES; \
					break; }

#define LOAD_ZP(REG, CYCLES)	{ addr = memory[program_counter]; \
					REG = memory_read(addr); \
					program_counter ++; \
					sign_flag = REG & 0x80; \
					zero_flag = !(REG); \
					cycle_count -= CYCLES; \
					break; }

#define LOAD_ZPIX(REG, CYCLES)	{ addr = memory[program_counter] + x_reg; \
					REG = memory_read(addr); \
					program_counter ++; \
					sign_flag = REG & 0x80; \
					zero_flag = !(REG); \
					cycle_count -= CYCLES; \
					break; }

#define LOAD_ZPIY(REG, CYCLES)	{ addr = memory[program_counter] + y_reg; \
					REG = memory_read(addr); \
					program_counter ++; \
					sign_flag = REG & 0x80; \
					zero_flag = !(REG); \
					cycle_count -= CYCLES; \
					break; }

#define LOAD_A(REG, CYCLES)	{ addr = (memory[program_counter+1] << 8) | memory[program_counter]; \
					REG = memory_read(addr); \
					program_counter += 2; \
					sign_flag = REG & 0x80; \
					zero_flag = !(REG); \
					cycle_count -= CYCLES; \
					break; }

#define LOAD_AIX(REG, CYCLES)	{ addr = ((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg; \
					REG = memory_read(addr); \
					program_counter += 2; \
					sign_flag = REG & 0x80; \
					zero_flag = !(REG); \
					cycle_count -= CYCLES; \
					break; }

#define LOAD_AIY(REG, CYCLES)	{ addr = ((memory[program_counter+1] << 8) | memory[program_counter]) + y_reg; \
					REG = memory_read(addr); \
					program_counter += 2; \
					sign_flag = REG & 0x80; \
					zero_flag = !(REG); \
					cycle_count -= CYCLES; \
					break; }

#define LOAD_IDI(REG, CYCLES)	{ addr = memory[program_counter] + x_reg; \
					tmp = (memory[addr + 1] << 8) | memory[addr]; \
					REG = memory_read(tmp); \
					program_counter ++; \
					sign_flag = REG & 0x80; \
					zero_flag = !(REG); \
					cycle_count -= CYCLES; \
					break; }

#define LOAD_INI(REG, CYCLES)	{ addr = memory[program_counter]; \
					tmp = ((memory[addr + 1] << 8) | memory[addr]) + y_reg; \
					REG = memory_read(tmp); \
					program_counter ++; \
					sign_flag = REG & 0x80; \
					zero_flag = !(REG); \
					cycle_count -= CYCLES; \
					break; }

#define LOGIC_SHIFT_R_ACC(CYCLES)	{ carry_flag = (carry_flag & 0xfe) | (accumulator & 0x01); \
						accumulator = accumulator >> 1; \
						sign_flag = accumulator & 0x80; \
						zero_flag = !(accumulator); \
						cycle_count -= CYCLES; \
						break; }

#define LOGIC_SHIFT_R_ZP(CYCLES)	{ addr = memory[program_counter]; \
						tmp = memory_read(addr); \
						carry_flag = (carry_flag & 0xfe) | (tmp & 0x01); \
						tmp = tmp >> 1; \
						write_memory(addr,tmp); \
						sign_flag = tmp & 0x80; \
						zero_flag = !(tmp); \
						program_counter ++; \
						cycle_count -= CYCLES; \
						break; }

#define LOGIC_SHIFT_R_ZPIX(CYCLES)	{ addr = memory[program_counter] + x_reg; \
						tmp = memory_read(addr); \
						carry_flag = (carry_flag & 0xfe) | (tmp & 0x01); \
						tmp = tmp >> 1; \
						write_memory(addr,tmp); \
						sign_flag = tmp & 0x80; \
						zero_flag = !(tmp); \
						program_counter ++; \
						cycle_count -= CYCLES; \
						break; }

#define LOGIC_SHIFT_R_A(CYCLES)		{ addr = (memory[program_counter+1] << 8) | memory[program_counter]; \
						tmp = memory_read(addr); \
						carry_flag = (carry_flag & 0xfe) | (tmp & 0x01); \
						tmp = tmp >> 1; \
						write_memory(addr,tmp); \
						sign_flag = tmp & 0x80; \
						zero_flag = !(tmp); \
						program_counter +=2; \
						cycle_count -= CYCLES; \
						break; }

#define LOGIC_SHIFT_R_AIX(CYCLES)	{ addr = ((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg; \
						tmp = memory_read(addr); \
						carry_flag = (carry_flag & 0xfe) | (tmp & 0x01); \
						tmp = tmp >> 1; \
						write_memory(addr,tmp); \
						sign_flag = tmp & 0x80; \
						zero_flag = !(tmp); \
						program_counter +=2; \
						cycle_count -= CYCLES; \
						break; }

#define NOP(CYCLES)		{ cycle_count -= CYCLES; \
					break; }

#define OR_MEM_IM(CYCLES)	{ accumulator |= memory[program_counter]; \
					program_counter++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define OR_MEM_ZP(CYCLES)	{ addr = memory[program_counter]; \
					accumulator |= memory_read(addr); \
					program_counter ++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define OR_MEM_ZPIX(CYCLES)	{ addr = memory[program_counter] + x_reg; \
					accumulator |= memory_read(addr); \
					program_counter ++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define OR_MEM_A(CYCLES)	{ addr = (memory[program_counter+1] << 8) | memory[program_counter]; \
					accumulator |= memory_read(addr); \
					program_counter += 2; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define OR_MEM_AIX(CYCLES)	{ addr = ((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg; \
					accumulator |= memory_read(addr); \
					program_counter += 2; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define OR_MEM_AIY(CYCLES)	{ addr = ((memory[program_counter+1] << 8) | memory[program_counter]) + y_reg; \
					accumulator |= memory_read(addr); \
					program_counter += 2; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define OR_MEM_IDI(CYCLES)	{ addr = memory[program_counter] + x_reg; \
					tmp = (memory[addr + 1] << 8) | memory[addr]; \
					accumulator |= memory_read(tmp); \
					program_counter ++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

#define OR_MEM_INI(CYCLES)	{ addr = memory[program_counter]; \
					tmp = ((memory[addr + 1] << 8) | memory[addr]) + y_reg; \
					accumulator |= memory_read(tmp); \
					program_counter ++; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					cycle_count -= CYCLES; \
					break; }

/* push accumulator on stack */
#define PUSH_A(b, CYCLES)	{ write_memory(stack_pointer+0x100,(b)); \
					if(stackdebug == 1) if(debug_cnt > show_debug_cnt) printf("[%d]: push stack = %x\n",debug_cnt-1,memory[stack_pointer+0x100]); \
					stack_pointer--; \
					cycle_count -= CYCLES; \
					break; }

/* pull accumulator off stack */
#define PULL_A(b, CYCLES)	{ stack_pointer++; \
					if(stackdebug == 1) if(debug_cnt > show_debug_cnt) printf("[%d]: pull stack = %x\n",debug_cnt-1,memory[stack_pointer+0x100]); \
					b = memory_read(stack_pointer+0x100); \
					sign_flag = b & 0x80; \
					zero_flag = !(b); \
					cycle_count -= CYCLES; \
					break; }

/* push processor status on stack */
#define PUSH_PS(CYCLES)		{ PUSH_ST(GET_SR()); \
					cycle_count -= CYCLES; \
					break; }

/* pull processor status off stack */
#define PULL_PS(CYCLES)		{ PULL_ST(); \
					addr = memory_read(stack_pointer+0x100); \
					SET_SR(addr); \
					cycle_count -= CYCLES; \
					break; }

#define ROTATE_LEFT_ACC(CYCLES)		{ tmp = carry_flag; \
						carry_flag = (carry_flag & 0xfe) | ((accumulator >> 7) & 0x01); \
						accumulator = (accumulator << 1); \
						accumulator |= tmp; \
						sign_flag = accumulator & 0x80; \
						zero_flag = !(accumulator); \
						cycle_count -= CYCLES; \
						break; }\

#define ROTATE_LEFT_ZP(CYCLES)		{ tmp = carry_flag; \
						tmp2 = memory[program_counter]; \
						addr = memory_read(tmp2); \
						carry_flag = (carry_flag & 0xfe) | ((addr >> 7) & 0x01); \
						addr = (addr << 1); \
						addr |= tmp; \
						write_memory(tmp2, addr); \
						sign_flag = accumulator & 0x80; \
						zero_flag = !(accumulator); \
						program_counter++; \
						cycle_count -= CYCLES; \
						break; }\

#define ROTATE_LEFT_ZPIX(CYCLES)	{ tmp = carry_flag; \
						tmp2 = memory[program_counter] + x_reg; \
						addr = memory_read(tmp2); \
						carry_flag = (carry_flag & 0xfe) | ((addr >> 7) & 0x01); \
						addr = (addr << 1); \
						addr |= tmp; \
						write_memory(tmp2, addr); \
						sign_flag = accumulator & 0x80; \
						zero_flag = !(accumulator); \
						program_counter++; \
						cycle_count -= CYCLES; \
						break; }\

#define ROTATE_LEFT_A(CYCLES)		{ tmp = carry_flag; \
						tmp2 = (memory[program_counter+1] << 8) | memory[program_counter]; \
						addr = memory_read(tmp2); \
						carry_flag = (carry_flag & 0xfe) | ((addr >> 7) & 0x01); \
						addr = (addr << 1); \
						addr |= tmp; \
						write_memory(tmp2, addr); \
						sign_flag = accumulator & 0x80; \
						zero_flag = !(accumulator); \
						program_counter += 2; \
						cycle_count -= CYCLES; \
						break; }\

#define ROTATE_LEFT_AIX(CYCLES)		{ tmp = carry_flag; \
						tmp2 = ((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg; \
						addr = memory_read(tmp2); \
						carry_flag = (carry_flag & 0xfe) | ((addr >> 7) & 0x01); \
						addr = (addr << 1); \
						addr |= tmp; \
						write_memory(tmp2, addr); \
						sign_flag = accumulator & 0x80; \
						zero_flag = !(accumulator); \
						program_counter += 2; \
						cycle_count -= CYCLES; \
						break; }\

#define ROTATE_RIGHT_ACC(CYCLES)	{ tmp = carry_flag; \
						carry_flag = (carry_flag & 0xfe) | (accumulator & 0x01); \
						accumulator = (accumulator >> 1); \
						if(tmp) accumulator |= 0x80; \
						sign_flag = accumulator & 0x80; \
						zero_flag = !(accumulator); \
						cycle_count -= CYCLES; \
						break; }\

#define ROTATE_RIGHT_ZP(CYCLES)		{ tmp = carry_flag; \
						tmp2 = memory[program_counter]; \
						addr = memory_read(tmp2); \
						carry_flag = (carry_flag & 0xfe) | (addr & 0x01); \
						addr = (addr >> 1); \
						if(tmp) addr |= 0x80; \
						write_memory(tmp2, addr); \
						sign_flag = accumulator & 0x80; \
						zero_flag = !(accumulator); \
						program_counter++; \
						cycle_count -= CYCLES; \
						break; }\

#define ROTATE_RIGHT_ZPIX(CYCLES)	{ tmp = carry_flag; \
						tmp2 = memory[program_counter] + x_reg; \
						addr = memory_read(tmp2); \
						carry_flag = (carry_flag & 0xfe) | (addr & 0x01); \
						addr = (addr >> 1); \
						if(tmp) addr |= 0x80; \
						write_memory(tmp2, addr); \
						sign_flag = accumulator & 0x80; \
						zero_flag = !(accumulator); \
						program_counter++; \
						cycle_count -= CYCLES; \
						break; }\

#define ROTATE_RIGHT_A(CYCLES)		{ tmp = carry_flag; \
						tmp2 = (memory[program_counter+1] << 8) | memory[program_counter]; \
						addr = memory_read(tmp2); \
						carry_flag = (carry_flag & 0xfe) | (addr & 0x01); \
						addr = (addr >> 1); \
						if(tmp) addr |= 0x80; \
						write_memory(tmp2, addr); \
						sign_flag = accumulator & 0x80; \
						zero_flag = !(accumulator); \
						program_counter += 2; \
						cycle_count -= CYCLES; \
						break; }\

#define ROTATE_RIGHT_AIX(CYCLES)	{ tmp = carry_flag; \
						tmp2 = ((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg; \
						addr = memory_read(tmp2); \
						carry_flag = (carry_flag & 0xfe) | (addr & 0x01); \
						addr = (addr >> 1); \
						if(tmp) addr |= 0x80; \
						write_memory(tmp2, addr); \
						sign_flag = accumulator & 0x80; \
						zero_flag = !(accumulator); \
						program_counter += 2; \
						cycle_count -= CYCLES; \
						break; }\

#define RET_INT(CYCLES)		{ PULL_ST(); \
					SET_SR(addr); \
					PULL_ST(); \
					program_counter = addr; \
					PULL_ST(); \
					program_counter += (addr << 8); \
					cycle_count -= CYCLES; \
					break; }

#define RET_SUB(CYCLES)		{ PULL_ST(); \
                                        program_counter = addr + 1; \
                                        PULL_ST(); \
                                        program_counter += (addr << 8); \
					cycle_count -= CYCLES; \
					break; }

#define SET_C_FLAG(CYCLES)	{ carry_flag = 1; \
					cycle_count -= CYCLES; \
					break; }

#define SET_D_MODE(CYCLES)	{ decimal_flag = 1; \
					cycle_count -= CYCLES; \
					break; }

#define SET_INT_DIS(CYCLES)	{ interrupt_flag = 1; \
					cycle_count -= CYCLES; \
					break; }

#define STORE_ZP(REG, CYCLES)    { addr = memory[program_counter]; \
					write_memory(addr, REG); \
					program_counter ++; \
					cycle_count -= CYCLES; \
					break; }

#define STORE_ZPIX(REG, CYCLES)	{ addr = memory[program_counter] + x_reg; \
					write_memory(addr, REG); \
					program_counter ++; \
					cycle_count -= CYCLES; \
					break; }

#define STORE_ZPIY(REG, CYCLES)	{ addr = memory[program_counter] + y_reg; \
					write_memory(addr, REG); \
					program_counter ++; \
					cycle_count -= CYCLES; \
					break; }

#define STORE_A(REG, CYCLES) { addr = (memory[program_counter+1] << 8) | memory[program_counter]; \
					write_memory(addr, REG); \
					program_counter += 2; \
					cycle_count -= CYCLES; \
					break; }

#define STORE_AIX(REG, CYCLES)	{ addr = ((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg; \
					write_memory(addr, REG); \
					program_counter += 2; \
					cycle_count -= CYCLES; \
					break; }

#define STORE_AIY(REG, CYCLES)	{ addr = ((memory[program_counter+1] << 8) | memory[program_counter]) + y_reg; \
					write_memory(addr, REG); \
					program_counter += 2; \
					cycle_count -= CYCLES; \
					break; }

#define STORE_IDI(REG, CYCLES)	{ addr = memory[program_counter] + x_reg; \
					tmp = (memory[addr + 1] << 8) | memory[addr]; \
					write_memory(tmp, REG); \
					program_counter ++; \
					cycle_count -= CYCLES; \
					break; }

#define STORE_INI(REG, CYCLES)	{ addr = memory[program_counter]; \
					tmp = ((memory[addr + 1] << 8) | memory[addr]) + y_reg; \
					write_memory(tmp, REG); \
					program_counter ++; \
					cycle_count -= CYCLES; \
					break; }

#define SUB_ACC_IM(CYCLES)	{ addr = memory[program_counter]; \
					tmp = accumulator - addr - (carry_flag ? 0 : 1); \
					overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80; \
					carry_flag = tmp <= 0xFF; \
					accumulator = tmp & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter ++; \
					cycle_count -= CYCLES; \
					break; }

#define SUB_ACC_ZP(CYCLES)	{ addr = memory_read(memory[program_counter]); \
					tmp = accumulator - addr - (carry_flag ? 0 : 1); \
					overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80; \
					carry_flag = tmp <= 0xFF; \
					accumulator = tmp & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter ++; \
					cycle_count -= CYCLES; \
					break; }

#define SUB_ACC_ZPIX(CYCLES)	{ addr = memory_read(memory[program_counter] + x_reg); \
					tmp = accumulator - addr - (carry_flag ? 0 : 1); \
					overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80; \
					carry_flag = tmp <= 0xFF; \
					accumulator = tmp & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter ++; \
					cycle_count -= CYCLES; \
					break; }

#define SUB_ACC_A(CYCLES)	{ addr = memory_read((memory[program_counter+1] << 8) | memory[program_counter]); \
					tmp = accumulator - addr - (carry_flag ? 0 : 1); \
					overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80; \
					carry_flag = tmp <= 0xFF; \
					accumulator = tmp & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter +=2; \
					cycle_count -= CYCLES; \
					break; }

#define SUB_ACC_AIX(CYCLES)	{ addr = memory_read(((memory[program_counter+1] << 8) | memory[program_counter]) + x_reg); \
					tmp = accumulator - addr - (carry_flag ? 0 : 1); \
					overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80; \
					carry_flag = tmp <= 0xFF; \
					accumulator = tmp & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter +=2; \
					cycle_count -= CYCLES; \
					break; }

#define SUB_ACC_AIY(CYCLES)	{ addr = memory_read(((memory[program_counter+1] << 8) | memory[program_counter]) + y_reg); \
					tmp = accumulator - addr - (carry_flag ? 0 : 1); \
					overflow_flag = (~(accumulator ^ addr)) & (accumulator ^ addr) & 0x80; \
					carry_flag = tmp <= 0xFF; \
					accumulator = tmp & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter +=2; \
					cycle_count -= CYCLES; \
					break; }

#define SUB_ACC_IDI(CYCLES)	{ addr = memory[program_counter] + x_reg; \
					tmp = (memory[addr + 1] << 8) | memory[addr]; \
					tmp2 = memory_read(tmp); \
					tmp3 = accumulator - tmp2 - (carry_flag ? 0 : 1); \
					overflow_flag = (~(accumulator ^ tmp2)) & (accumulator ^ tmp2) & 0x80; \
					carry_flag = tmp3 <= 0xFF; \
					accumulator = tmp3 & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter ++; \
					cycle_count -= CYCLES; \
					break; }

#define SUB_ACC_INI(CYCLES)	{ addr = memory[program_counter]; \
					tmp = ((memory[addr + 1] << 8) | memory[addr]) + y_reg; \
					tmp2 = memory_read(tmp); \
					tmp3 = accumulator - tmp2 - (carry_flag ? 0 : 1); \
					overflow_flag = (~(accumulator ^ tmp2)) & (accumulator ^ tmp2) & 0x80; \
					carry_flag = tmp3 <= 0xFF; \
					accumulator = tmp3 & 0xFF; \
					sign_flag = accumulator & 0x80; \
					zero_flag = !(accumulator); \
					program_counter ++; \
					cycle_count -= CYCLES; \
					break; }

#define TRANSFER_REG(REG1,REG2,CYCLES)	{ REG2 = REG1; \
						sign_flag = REG2 & 0x80; \
						zero_flag = !(REG2); \
						cycle_count -= CYCLES; \
						break; }

#define TRANSFER_STACK_FROM(REG,CYCLES)	{ REG = stack_pointer; \
						sign_flag = REG & 0x80; \
						zero_flag = !(REG); \
						cycle_count -= CYCLES; \
						break; }

#define TRANSFER_STACK_TO(REG,CYCLES)	{ stack_pointer = (REG + 0x100); \
						sign_flag = REG & 0x80; \
						zero_flag = !(REG); \
						cycle_count -= CYCLES; \
						break; }

/* stack push */
#define PUSH_ST(b)		{ write_memory(stack_pointer+0x100,(b)); \
					if(stackdebug == 1) if(debug_cnt > show_debug_cnt) printf("[%d]: push stack = %x\n",debug_cnt-1,memory[stack_pointer+0x100]); \
					stack_pointer--; }

/* stack pull */
#define PULL_ST()		{ stack_pointer++; \
					if(stackdebug == 1) if(debug_cnt > show_debug_cnt) printf("[%d]: pull stack = %x\n",debug_cnt-1,memory[stack_pointer+0x100]); \
					addr=memory_read(stack_pointer+0x100); }

/* get the cpu flags */
#define GET_SR()		{ ((sign_flag ? 0x80 : 0) |\
					(zero_flag ? 0x02 : 0) |\
					(carry_flag ? 0x01 : 0) |\
					(interrupt_flag ? 0x04 : 0) |\
					(decimal_flag ? 0x08 : 0) |\
					(overflow_flag ? 0x40 : 0) |\
					(break_flag ? 0x10 : 0) | 0x20); }

/* set the cpu flags */
#define SET_SR(b)		{ ((sign_flag = b & 0x80) |\
					(zero_flag = b & 0x02) |\
					(carry_flag = b & 0x01) |\
					(interrupt_flag = b & 0x04) |\
					(decimal_flag = b & 0x08) |\
					(overflow_flag = b & 0x40) |\
					(break_flag = (b & 0x10) | 0x20)); }
