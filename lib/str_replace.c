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
 * str_replace.c - string replace function
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *str_replace(const char *str, const char *old, const char *new)
{
	char *ret, *r;

	const char *p, *q;

	size_t len_str = strlen(str);
	size_t len_old = strlen(old);
	size_t len_new = strlen(new);
	size_t count;

	for(count = 0, p = str; (p = strstr(p, old)); p += len_old)
		count++;

	ret = malloc(count * (len_new - len_old) + len_str + 1);

	if(!ret)
		return NULL;

	for(r = ret, p = str; (q = strstr(p, old)); p = q + len_old) {
		count = q - p;
		memcpy(r, p, count);
		r += count;
		strcpy(r, new);
		r += len_new;
	}

	strcpy(r, p);

	return ret;
}
