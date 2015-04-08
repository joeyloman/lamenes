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
 * mmc1.h - NES Mapper 1: MMC1
 */

int mmc1_PRGROM_area_switch;
int mmc1_PRGROM_bank_switch;
int mmc1_CHRROM_bank_switch;

int mmc1_reg0_data = 0;
int mmc1_reg1_data = 0;
int mmc1_reg2_data = 0;
int mmc1_reg3_data = 0;

int mmc1_reg0_bitcount = 0;
int mmc1_reg1_bitcount = 0;
int mmc1_reg2_bitcount = 0;
int mmc1_reg3_bitcount = 0;

void
mmc1_switch_prg(int bank, int pagesize, int area)
{
	int prg_size;
	unsigned int address;

	if(pagesize == 0) {
		prg_size = 32768;
		address = 0x8000;
	} else if(pagesize == 1) {
		prg_size = 16384;

		if(area == 0) {
			address = 0xc000;
		} else if(area == 1) {
			address = 0x8000;
		 } else {
			fprintf(stdout,"[!] error prg area selection incorrect!\n");
			exit(1);
		}
	} else {
		fprintf(stdout,"[!] error prg pagesize incorrect!\n");
		exit(1);
	}

	#ifdef MAPPER_DEBUG
	printf("[%d] prg_switch: reading from %d (offset: %x) into mem address: %x (bank: %d, pagesize: %d, area: %d)\n",
		debug_cnt,16 + (bank * prg_size),16 + (bank *prg_size),address,bank + 1,pagesize,area);
	#endif

	memcpy(memory + address, romcache + 16 + (bank * prg_size), prg_size); 
}

void
mmc1_switch_chr(int bank, int pagesize, int area) {
	int prg_size;
	int chr_size;
	int chr_start;

	unsigned int address;

	prg_size = 16384;

	chr_start = prg_size * PRG;

	if(pagesize == 0) {
		chr_size = 8192;
		address = 0x0000;
	} else if(pagesize == 1) {
		chr_size = 4096;
		if(area == 0) {
			address = 0x0000;
		} else if(area == 1) {
			address = 0x1000;
		 } else {
			fprintf(stdout,"[!] error chr area selection incorrect!\n");
			exit(1);
		}
	} else {
		fprintf(stdout,"[!] error chr pages incorrect!\n");
		exit(1);
	}

	#ifdef MAPPER_DEBUG
	printf("[%d] chr_switch: reading from offset: %x, into mem address: %x (bank: %d, pagesize: %d, area: %d)\n",
		debug_cnt,16 + chr_start + (bank * prg_size),address,bank,pagesize,area);
	#endif

	memcpy(ppu_memory + address, romcache + 16 + chr_start + (bank * chr_size), chr_size); 
}

void
mmc1_access(unsigned int address,unsigned char data)
{
	#ifdef MAPPER_DEBUG
	printf("[%d] mmc1 access: 0x%x\n",debug_cnt,address);
	#endif

	if(address > 0x7fff && address < 0xa000) {
		if(data & 0x80) {
			/* reset register */
			mmc1_reg0_data = 0;
			mmc1_reg0_bitcount = 0;
		} else {
			mmc1_reg0_data |= data;
			mmc1_reg0_bitcount++;
		}

		if(mmc1_reg0_bitcount == 5) {
			/* set horizontal/vertical mirroring */
			if(mmc1_reg0_data & 0x01) {
				/* set to horizontal */
				MIRRORING = 0;
			} else {
				/* set to vertical */
				MIRRORING = 1;
			}

			/* set h/v or one screen mirroring */
			if(mmc1_reg0_data & 0x02) {
				OS_MIRROR = 1;
			} else {
				OS_MIRROR = 0;
			}

			/* switch the low/high prg rom area */
			if(mmc1_reg0_data & 0x04) {
				/* low 0x8000 */
				mmc1_PRGROM_area_switch = 1;
			} else {
				/* high 0xc000 */
				mmc1_PRGROM_area_switch = 0;
			}

			/* select prg rom bank size */
			if(mmc1_reg0_data & 0x08) {
				/* 16kb */
				mmc1_PRGROM_bank_switch = 1;
			} else {
				/* 32kb */
				mmc1_PRGROM_bank_switch = 0;
			}

			/* select chr rom bank size */
			if(mmc1_reg0_data & 0x10) {
				/* 4kb */
				mmc1_CHRROM_bank_switch = 1;
			} else {
				/* 8kb */
				mmc1_CHRROM_bank_switch = 0;
			}

			mmc1_reg0_data = 0;
			mmc1_reg0_bitcount = 0;

			#ifdef MAPPER_DEBUG
			printf("[%d] MIRRORING -> %d, OS_MIRROR -> %d, mmc1_PRGROM_area_switch -> %d, mmc1_PRGROM_bank_switch -> %d,mmc1_CHRROM_bank_switch -> %d\n",
				debug_cnt,MIRRORING,OS_MIRROR,mmc1_PRGROM_area_switch,mmc1_PRGROM_bank_switch,mmc1_CHRROM_bank_switch);
			#endif
		}

	}

	if(address > 0x9fff && address < 0xc000) {
		if(data & 0x80) {
			/* reset register */
			mmc1_reg1_data = 0;
			mmc1_reg1_bitcount = 0;
		} else {
			mmc1_reg1_bitcount++;
			mmc1_reg1_data |= (data & 0x01) << mmc1_reg1_bitcount;

			#ifdef MAPPER_DEBUG
			printf("[%d] mmc1_reg1_bitcount = %d, mmc1_reg1_data = %x\n",
				debug_cnt,mmc1_reg1_bitcount,mmc1_reg1_data);
			#endif
		}

		if(mmc1_reg1_bitcount == 5) {
			if(mmc1_reg1_data != 0x00) {
				mmc1_switch_chr(mmc1_reg1_data >> 1, mmc1_CHRROM_bank_switch, 0);
			}

			mmc1_reg1_data = 0;
			mmc1_reg1_bitcount = 0;
		}
	}

	if(address > 0xbfff && address < 0xe000) {
		if(data & 0x80) {
			/* reset register */
			mmc1_reg2_data = 0;
			mmc1_reg2_bitcount = 0;
		} else {
			mmc1_reg2_bitcount++;
			mmc1_reg2_data |= (data & 0x01) << mmc1_reg2_bitcount;

			#ifdef MAPPER_DEBUG
			printf("[%d] mmc1_reg2_bitcount = %d, mmc1_reg2_data = %x\n",
				debug_cnt,mmc1_reg2_bitcount,mmc1_reg2_data);
			#endif
		}

		if(mmc1_reg2_bitcount == 5) {
			if(mmc1_reg2_data != 0x00) {
				mmc1_switch_chr(mmc1_reg2_data >> 1, mmc1_CHRROM_bank_switch, 1);
			}

			mmc1_reg2_data = 0;
			mmc1_reg2_bitcount = 0;
		}
	}

	if(address > 0xdfff && address < 0x10000) {
		if(data & 0x80) {
			/* reset register */
			mmc1_reg3_data = 0;
			mmc1_reg3_bitcount = 0;
		} else {
			mmc1_reg3_bitcount++;
			mmc1_reg3_data |= (data & 0x01) << mmc1_reg3_bitcount;

			#ifdef MAPPER_DEBUG
			printf("[%d] mmc1_reg3_bitcount = %d, mmc1_reg3_data = %x\n",
				debug_cnt,mmc1_reg3_bitcount,mmc1_reg3_data);
			#endif
		}

		if(mmc1_reg3_bitcount == 5) {
			mmc1_switch_prg(mmc1_reg3_data >> 1, mmc1_PRGROM_bank_switch, mmc1_PRGROM_area_switch);

			mmc1_reg3_data = 0;
			mmc1_reg3_bitcount = 0;
		}
	}
}
