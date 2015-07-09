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
 * opcodes.h - 6502 opcode handler
 */

/* ADC  -  Add to Accumulator with Carry */
case 0x69: if(disassemble == 1)disas("ADC","IM"); ADC_IM(2);
case 0x65: if(disassemble == 1)disas("ADC","ZP"); ADC_ZP(3);
case 0x75: if(disassemble == 1)disas("ADC","ZPIX"); ADC_ZPIX(4);
case 0x6D: if(disassemble == 1)disas("ADC","A"); ADC_A(4);
case 0x7D: if(disassemble == 1)disas("ADC","AIX"); ADC_AIX(5);
case 0x79: if(disassemble == 1)disas("ADC","AIY"); ADC_AIY(4);
case 0x61: if(disassemble == 1)disas("ADC","IDI"); ADC_IDI(6);
case 0x71: if(disassemble == 1)disas("ADC","INI"); ADC_INI(5);

/* AND  -  AND Memory with Accumulator */
case 0x29: if(disassemble == 1)disas("AND","IM"); AND_IM(2);
case 0x25: if(disassemble == 1)disas("AND","ZP"); AND_ZP(3);
case 0x35: if(disassemble == 1)disas("AND","ZPIX"); AND_ZPIX(4);
case 0x2D: if(disassemble == 1)disas("AND","A"); AND_A(4);
case 0x3D: if(disassemble == 1)disas("AND","AIX"); AND_AIX(5);
case 0x39: if(disassemble == 1)disas("AND","AIY"); AND_AIY(4);
case 0x21: if(disassemble == 1)disas("AND","IDI"); AND_IDI(6);
case 0x31: if(disassemble == 1)disas("AND","INI"); AND_INI(5);

/* ASL  -  Arithmatic Shift Left */
case 0x0A: if(disassemble == 1)disas("ASL","ACC"); ARITH_SL_ACC(2);
case 0x06: if(disassemble == 1)disas("ASL","ZP"); ARITH_SL_ZP(5);
case 0x16: if(disassemble == 1)disas("ASL","ZPIX"); ARITH_SL_ZPIX(6);
case 0x0E: if(disassemble == 1)disas("ASL","A"); ARITH_SL_A(6);
case 0x1E: if(disassemble == 1)disas("ASL","AIX"); ARITH_SL_AIX(7);

/* BCC  -  Branch on Carry Clear */
case 0x90: if(disassemble == 1)disas("BCC","aa"); BRANCH_CC(2);

/* BCS  -  Branch on Carry Set */
case 0xB0: if(disassemble == 1)disas("BCS","aa"); BRANCH_CS(2);

/* BEQ  -  Branch Zero Set */
case 0xF0: if(disassemble == 1)disas("BEQ","aa"); BRANCH_ZS(2);

/* note: bit moet 5 instr zijn ipv 2? */
/* BIT  -  Test Bits in Memory with Accumulator */
case 0x24: if(disassemble == 1)disas("BIT","ZP"); BIT_TEST_ZP(3);
case 0x2C: if(disassemble == 1)disas("BIT","A"); BIT_TEST_A(4);

/* BMI  -  Branch on Result Minus */
case 0x30: if(disassemble == 1)disas("BMI","aa"); BRANCH_RM(2);

/* BNE  -  Branch on Z reset */
case 0xD0: if(disassemble == 1)disas("BNE","aa"); BRANCH_ZR(2);

/* BPL  -  Branch on Result Plus (or Positive) */
case 0x10: if(disassemble == 1)disas("BPL","aa"); BRANCH_RP(2);

/* BRA? */

/* BRK  -  Force a Break */
case 0x00: if(disassemble == 1)disas("BRK","NODATA"); BREAK(7);

/* BVC  -  Branch on Overflow Clear */
case 0x50: if(disassemble == 1)disas("BVC","aa"); BRANCH_OC(2);

/* BVS  -  Branch on Overflow Set */
case 0x70: if(disassemble == 1)disas("BVS","aa"); BRANCH_OS(4);

/* CLC  -  Clear Carry Flag */
case 0x18: if(disassemble == 1)disas("CLC","NODATA"); CLEAR_CF(2);

/* CLD  -  Clear Decimal Mode */
case 0xD8: if(disassemble == 1)disas("CLD","NODATA"); CLEAR_DM(2);

/* CLI  -  Clear Interrupt Disable */
case 0x58: if(disassemble == 1)disas("CLI","NODATA"); CLEAR_ID(2);

/* CLV  -  Clear Overflow Flag */
case 0xB8: if(disassemble == 1)disas("CLV","NODATA"); CLEAR_OF(2);

/* CMP  -  Compare Memory and Accumulator */
case 0xC9: if(disassemble == 1)disas("CMP","IM"); COMP_MEM_IM(accumulator,2);
case 0xC5: if(disassemble == 1)disas("CMP","ZP"); COMP_MEM_ZP(accumulator,3);
case 0xD5: if(disassemble == 1)disas("CMP","ZPIX"); COMP_MEM_ZPIX(accumulator,4);
case 0xCD: if(disassemble == 1)disas("CMP","A"); COMP_MEM_A(accumulator,4);
case 0xDD: if(disassemble == 1)disas("CMP","AIX"); COMP_MEM_AIX(accumulator,5);
case 0xD9: if(disassemble == 1)disas("CMP","AIY"); COMP_MEM_AIY(accumulator,4);
case 0xC1: if(disassemble == 1)disas("CMP","IDI"); COMP_MEM_IDI(accumulator,6);
case 0xD1: if(disassemble == 1)disas("CMP","INI"); COMP_MEM_INI(accumulator,6);

/* CPX  -  Compare Memory and X register */
case 0xE0: if(disassemble == 1)disas("CPX","IM"); COMP_MEM_IM(x_reg,2);
case 0xE4: if(disassemble == 1)disas("CPX","ZP"); COMP_MEM_ZP(x_reg,3);
case 0xEC: if(disassemble == 1)disas("CPX","A"); COMP_MEM_A(x_reg,4);

/* CPY  -  Compare Memory and Y register */
case 0xC0: if(disassemble == 1)disas("CPY","IM"); COMP_MEM_IM(y_reg,2);
case 0xC4: if(disassemble == 1)disas("CPY","ZP"); COMP_MEM_ZP(y_reg,3);
case 0xCC: if(disassemble == 1)disas("CPY","A"); COMP_MEM_A(y_reg,4);

/* DEA?? */

/* DEC  -  Decrement Memory by One */
case 0xC6: if(disassemble == 1)disas("DEC","ZP"); DECR_MEM_ZP(5);
case 0xD6: if(disassemble == 1)disas("DEC","ZPIX"); DECR_MEM_ZPIX(6);
case 0xCE: if(disassemble == 1)disas("DEC","A"); DECR_MEM_A(6);
case 0xDE: if(disassemble == 1)disas("DEC","AIX"); DECR_MEM_AIX(7);

/* DEX  -  Decrement X */
case 0xCA: if(disassemble == 1)disas("DEX","NODATA"); DECR(x_reg,2);

/* DEY  -  Decrement Y */
case 0x88: if(disassemble == 1)disas("DEY","NODATA"); DECR(y_reg,2);

/* EOR  -  Exclusive-OR Memory with Accumulator */
case 0x49: if(disassemble == 1)disas("EOR","IM"); EXCL_OR_MEM_IM(2);
case 0x45: if(disassemble == 1)disas("EOR","ZP"); EXCL_OR_MEM_ZP(3);
case 0x55: if(disassemble == 1)disas("EOR","ZPIX"); EXCL_OR_MEM_ZPIX(4);
case 0x4D: if(disassemble == 1)disas("EOR","A"); EXCL_OR_MEM_A(6);
case 0x5D: if(disassemble == 1)disas("EOR","AIX"); EXCL_OR_MEM_AIX(5);
case 0x59: if(disassemble == 1)disas("EOR","AIY"); EXCL_OR_MEM_AIY(4);
case 0x41: if(disassemble == 1)disas("EOR","IDI"); EXCL_OR_MEM_IDI(6);
case 0x51: if(disassemble == 1)disas("EOR","INI"); EXCL_OR_MEM_INI(5);

/* INC  -  Increment Memory by one */
case 0xE6: if(disassemble == 1)disas("INC","ZP"); INCR_MEM_ZP(5);
case 0xF6: if(disassemble == 1)disas("INC","ZPIX"); INCR_MEM_ZPIX(6);
case 0xEE: if(disassemble == 1)disas("INC","A"); INCR_MEM_A(6);
case 0xFE: if(disassemble == 1)disas("INC","AIX"); INCR_MEM_AIX(7);

/* INX  -  Increment X by one */
case 0xE8: if(disassemble == 1)disas("INX","NODATA"); INCR(x_reg,2);

/* INY  -  Increment Y by one */
case 0xC8: if(disassemble == 1)disas("INY","NODATA"); INCR(y_reg,2);

/* mis nog 1 JMP instructie */
/* JMP - Jump */
case 0x4c: if(disassemble == 1)disas("JMP","A"); JMP_A(3);
case 0x6c: if(disassemble == 1)disas("JMP","AI"); JMP_AI(5);

/* JSR - Jump to subroutine */
case 0x20: if(disassemble == 1)disas("JSR","A"); JSR(6);

/* LDA - Load Accumulator with memory */
case 0xA9: if(disassemble == 1)disas("LDA","IM"); LOAD_IM(accumulator,2);
case 0xA5: if(disassemble == 1)disas("LDA","ZP"); LOAD_ZP(accumulator,3);
case 0xB5: if(disassemble == 1)disas("LDA","ZPIX"); LOAD_ZPIX(accumulator,4);
case 0xAD: if(disassemble == 1)disas("LDA","A"); LOAD_A(accumulator,4);
case 0xBD: if(disassemble == 1)disas("LDA","AIX"); LOAD_AIX(accumulator,4);
case 0xB9: if(disassemble == 1)disas("LDA","AIY"); LOAD_AIY(accumulator,4);
case 0xA1: if(disassemble == 1)disas("LDA","IDI"); LOAD_IDI(accumulator,6);
case 0xB1: if(disassemble == 1)disas("LDA","INI"); LOAD_INI(accumulator,5);

/* LDX - Load X with Memory */
case 0xA2: if(disassemble == 1)disas("LDX","IM"); LOAD_IM(x_reg,2);
case 0xA6: if(disassemble == 1)disas("LDX","ZP"); LOAD_ZP(x_reg,3);
case 0xB6: if(disassemble == 1)disas("LDX","ZPIY"); LOAD_ZPIY(x_reg,4);
case 0xAE: if(disassemble == 1)disas("LDX","A"); LOAD_A(x_reg,4);
case 0xBE: if(disassemble == 1)disas("LDX","AIY"); LOAD_AIY(x_reg,4);

/* LDY - Load Y with Memory */
case 0xA0: if(disassemble == 1)disas("LDY","IM"); LOAD_IM(y_reg,2);
case 0xA4: if(disassemble == 1)disas("LDY","ZP"); LOAD_ZP(y_reg,3);
case 0xB4: if(disassemble == 1)disas("LDY","ZPIX"); LOAD_ZPIX(y_reg,4);
case 0xAC: if(disassemble == 1)disas("LDY","A"); LOAD_A(y_reg,4);
case 0xBC: if(disassemble == 1)disas("LDY","AIX"); LOAD_AIX(y_reg,4);

/* LSR  -  Logical Shift Right */
case 0x4A: if(disassemble == 1)disas("LSR","ACC"); LOGIC_SHIFT_R_ACC(2);
case 0x46: if(disassemble == 1)disas("LSR","ZP"); LOGIC_SHIFT_R_ZP(5);
case 0x56: if(disassemble == 1)disas("LSR","ZPIX"); LOGIC_SHIFT_R_ZPIX(6);
case 0x4E: if(disassemble == 1)disas("LSR","A"); LOGIC_SHIFT_R_A(6);
case 0x5E: if(disassemble == 1)disas("LSR","AIX"); LOGIC_SHIFT_R_AIX(7);

/* NOP - No Operation (79 instructies?) */
case 0xEA: if(disassemble == 1)disas("NOP","NODATA"); NOP(2);

/* ORA  -  OR Memory with Accumulator */
case 0x09: if(disassemble == 1)disas("ORA","IM"); OR_MEM_IM(2);
case 0x05: if(disassemble == 1)disas("ORA","ZP"); OR_MEM_ZP(3);
case 0x15: if(disassemble == 1)disas("ORA","ZPIX"); OR_MEM_ZPIX(4);
case 0x0D: if(disassemble == 1)disas("ORA","A"); OR_MEM_A(4);
case 0x1D: if(disassemble == 1)disas("ORA","AIX"); OR_MEM_AIX(5);
case 0x19: if(disassemble == 1)disas("ORA","AIY"); OR_MEM_AIY(4);
case 0x01: if(disassemble == 1)disas("ORA","IDI"); OR_MEM_IDI(6);
case 0x11: if(disassemble == 1)disas("ORA","INI"); OR_MEM_INI(5);

/* PHA  -  Push Accumulator on Stack */
case 0x48: if(disassemble == 1)disas("PHA","NODATA"); PUSH_A(accumulator,3);

/* PHP  -  Push Processor Status on Stack */
case 0x08: if(disassemble == 1)disas("PHP","NODATA"); PUSH_PS(3);

/* PHX? */

/* PHY? */

/* PLA  -  Pull Accumulator from Stack */
case 0x68: if(disassemble == 1)disas("PLA","NODATA"); PULL_A(accumulator,4);

/* PLP  -  Pull Processor Status from Stack */
case 0x28: if(disassemble == 1)disas("PLP","NODATA"); PULL_PS(4);

/* PLX? */

/* PLY? */

/* ROL  -  Rotate Left */
case 0x2A: if(disassemble == 1)disas("ROL","ACC"); ROTATE_LEFT_ACC(2);
case 0x26: if(disassemble == 1)disas("ROL","ZP"); ROTATE_LEFT_ZP(5);
case 0x36: if(disassemble == 1)disas("ROL","ZPIX"); ROTATE_LEFT_ZPIX(6);
case 0x2E: if(disassemble == 1)disas("ROL","A"); ROTATE_LEFT_A(6);
case 0x3E: if(disassemble == 1)disas("ROL","AIX"); ROTATE_LEFT_AIX(7);

/* ROR  -  Rotate Right */
case 0x6A: if(disassemble == 1)disas("ROR","ACC"); ROTATE_RIGHT_ACC(2);
case 0x66: if(disassemble == 1)disas("ROR","ZP"); ROTATE_RIGHT_ZP(5);
case 0x76: if(disassemble == 1)disas("ROR","ZPIX"); ROTATE_RIGHT_ZPIX(6);
case 0x6E: if(disassemble == 1)disas("ROR","A"); ROTATE_RIGHT_A(6);
case 0x7E: if(disassemble == 1)disas("ROR","AIX"); ROTATE_RIGHT_AIX(7);

/* RTI  -  Return from Interrupt */
case 0x40: if(disassemble == 1)disas("RTI","NODATA"); RET_INT(4);

/* RTS  -  Return from Subroutine */
case 0x60: if(disassemble == 1)disas("RTS","NODATA"); RET_SUB(4);

/* SBC  -  Subtract from Accumulator with Carry (IDI_ZP?) */
case 0xE9: if(disassemble == 1)disas("SBC","IM"); SUB_ACC_IM(2);
case 0xE5: if(disassemble == 1)disas("SBC","ZP"); SUB_ACC_ZP(3);
case 0xF5: if(disassemble == 1)disas("SBC","ZPIX"); SUB_ACC_ZPIX(4);
case 0xED: if(disassemble == 1)disas("SBC","A"); SUB_ACC_A(4);
case 0xFD: if(disassemble == 1)disas("SBC","AIX"); SUB_ACC_AIX(5);
case 0xF9: if(disassemble == 1)disas("SBC","AIY"); SUB_ACC_AIY(4);
case 0xE1: if(disassemble == 1)disas("SBC","IDI"); SUB_ACC_IDI(6);
case 0xF1: if(disassemble == 1)disas("SBC","INI"); SUB_ACC_INI(5);

/* SEC  -  Set Carry Flag */
case 0x38: if(disassemble == 1)disas("SEC","NODATA"); SET_C_FLAG(2);

/* SED  -  Set Decimal Mode */
case 0xF8: if(disassemble == 1)disas("SED","NODATA"); SET_D_MODE(2);

/* SEI - Set Interrupt Disable */
case 0x78: if(disassemble == 1)disas("SEI","NODATA"); SET_INT_DIS(2);

/* STA - Store Accumulator in Memory (IDI_ZP?) */
case 0x85: if(disassemble == 1)disas("STA","ZP"); STORE_ZP(accumulator,3);
case 0x95: if(disassemble == 1)disas("STA","ZPIX"); STORE_ZPIX(accumulator,4);
case 0x8D: if(disassemble == 1)disas("STA","A"); STORE_A(accumulator,4);
case 0x9D: if(disassemble == 1)disas("STA","AIX"); STORE_AIX(accumulator,5);
case 0x99: if(disassemble == 1)disas("STA","AIY"); STORE_AIY(accumulator,5);
case 0x81: if(disassemble == 1)disas("STA","IDI"); STORE_IDI(accumulator,6);
case 0x91: if(disassemble == 1)disas("STA","INI"); STORE_INI(accumulator,6);

/* STX - Store X in Memory */
case 0x86: if(disassemble == 1)disas("STX","ZP"); STORE_ZP(x_reg,3);
case 0x96: if(disassemble == 1)disas("STX","ZPIY"); STORE_ZPIY(x_reg,4);
case 0x8E: if(disassemble == 1)disas("STX","A"); STORE_A(x_reg,4);

/* STY - Store Y in Memory */
case 0x84: if(disassemble == 1)disas("STY","ZP"); STORE_ZP(y_reg,3);
case 0x94: if(disassemble == 1)disas("STY","ZPIX"); STORE_ZPIX(y_reg,4);
case 0x8C: if(disassemble == 1)disas("STY","A"); STORE_A(y_reg,4);

/* STZ? */

/* TAX  -  Transfer Accumulator to X */
case 0xAA: if(disassemble == 1)disas("TAX","NODATA"); TRANSFER_REG(accumulator,x_reg,2);

/* TAY  -  Transfer Accumulator to Y */
case 0xA8: if(disassemble == 1)disas("TAY","NODATA"); TRANSFER_REG(accumulator,y_reg,2);

/* TRB? */

/* TSB? */

/* TSX  -  Transfer Stack to X */
case 0xBA: if(disassemble == 1)disas("TSX","NODATA"); TRANSFER_STACK_FROM(x_reg,2);

/* TXA  -  Transfer X to Accumulator */
case 0x8A: if(disassemble == 1)disas("TXA","NODATA"); TRANSFER_REG(x_reg,accumulator,2);

/* TXS  -  Transfer X to Stack */
case 0x9A: if(disassemble == 1)disas("TXS","NODATA"); TRANSFER_STACK_TO(x_reg,2);

/* TYA  -  Transfer Y to Accumulator */
case 0x98: if(disassemble == 1)disas("TYA","NODATA"); TRANSFER_REG(y_reg,accumulator,2);

/* Unrecognized instructions */
default:
printf("[%d]: error -> unrecognized instruction at offset 0x%x! opcode -> [%x]\n",debug_cnt-1,program_counter -1,opcode);

/* used in LameNES */
if(startdebugger > 0) {
	disassemble = 1;
	hit_break = 1;
	debugger();
}
break;
