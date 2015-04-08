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

extern unsigned int ppu_control1;
extern unsigned int ppu_control2;
extern unsigned int ppu_addr;
extern unsigned int ppu_addr_h;
extern unsigned int ppu_addr_tmp;
extern unsigned int ppu_status;
extern unsigned int ppu_status_tmp;
extern unsigned int ppu_bgscr_f;

extern int current_scanline;

extern unsigned int sprite_address;

extern unsigned int loopyT;
extern unsigned int loopyV;
extern unsigned int loopyX;

extern void show_gfxcache();
extern void write_ppu_memory(unsigned int address,unsigned char data);
extern void render_background(int scanline);
extern void render_sprites();
extern void check_sprite_hit(int scanline);
extern void update_screen();
