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

#include <stdio.h>

#include "lamenes.h"

int pad1_DOWN;
int pad1_UP;
int pad1_LEFT;
int pad1_RIGHT;
int pad1_START;
int pad1_SELECT;
int pad1_A;
int pad1_B;

void
set_input(int pad_key)
{
	switch(pad_key) {
		/* pad_down */
		case 1:
		pad1_DOWN = 0x01;
		break;

		/* pad_up */
		case 2:
		pad1_UP = 0x01;
		break;

		/* pad_left */
		case 3:
		pad1_LEFT = 0x01;
		break;

		/* pad_right */
		case 4:
		pad1_RIGHT = 0x01;
		break;

		/* pad_start */
		case 5:
		pad1_START = 0x01;
		break;

		/* pad_select */
		case 6:
		pad1_SELECT = 0x01;
		break;

		/* pad_a */
		case 7:
		pad1_A = 0x01;
		break;

		/* pad_b */
		case 8:
		pad1_B = 0x01;
		break;

		default:
		/* never reached */
		break;
	}
}

void
clear_input(int pad_key)
{
	switch(pad_key) {
		/* pad_down */
		case 1:
		pad1_DOWN = 0x40;
		break;

		/* pad_up */
		case 2:
		pad1_UP = 0x40;
		break;

		/* pad_left */
		case 3:
		pad1_LEFT = 0x40;
		break;

		/* pad_right */
		case 4:
		pad1_RIGHT = 0x40;
		break;

		/* pad_start */
		case 5:
		pad1_START = 0x40;
		break;

		/* pad_select */
		case 6:
		pad1_SELECT = 0x40;
		break;

		/* pad_a */
		case 7:
		pad1_A = 0x40;
		break;

		/* pad_b */
		case 8:
		pad1_B = 0x40;
		break;

		default:
		/* never reached */
		break;
	}
}

void
reset_input()
{
	pad1_DOWN = 0x40;
	pad1_UP = 0x40;
	pad1_LEFT = 0x40;
	pad1_RIGHT = 0x40;
	pad1_START = 0x40;
	pad1_SELECT = 0x40;
	pad1_A = 0x40;
	pad1_B = 0x40;

}
