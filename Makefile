LAMENES		= lamenes.c lame6502/lame6502.c lame6502/disas.c lame6502/debugger.c lib/str_chrchk.c lib/str_cut.c sdl_functions.c romloader.c ppu.c input.c lib/str_replace.c
CC		= gcc
OBJ_FLAG	= -O3 -fomit-frame-pointer -Wall -I/usr/local/include -L/usr/local/lib -L/usr/X11R6/lib `sdl-config --libs` -pthread;

lamenes:	$(LAMENES)
		$(CC) $(LAMENES) $(OBJ_FLAG)
		strip a.out
		mv a.out lamenes

install:	lamenes
		cp lamenes /usr/local/bin

uninstall:
		rm -f /usr/local/bin/lamenes

clean:		$(LAMENES)
		rm -f lamenes lamenes.core core.*
