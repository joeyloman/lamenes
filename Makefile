LAMENES_SOURCES	= lamenes.c lame6502/lame6502.c lame6502/disas.c lame6502/debugger.c romloader.c ppu.c input.c
LIB_SOURCES = lib/str_chrchk.c lib/str_cut.c lib/str_replace.c
DESKTOP_SOURCES = $(wildcard system/desktop/*.c)
CC = gcc
CFLAGS = -O3 -fomit-frame-pointer -Wall -I/usr/local/include -I. `sdl2-config --cflags`
LDFLAGS = -L/usr/local/lib -L/usr/X11R6/lib `sdl2-config --libs`

lamenes: $(LAMENES_SOURCES) $(LIB_SOURCES) $(DESKTOP_SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LAMENES_SOURCES) $(LIB_SOURCES) $(DESKTOP_SOURCES) -o lamenes

install: lamenes
	cp lamenes /usr/local/bin

uninstall:
	rm -f /usr/local/bin/lamenes

clean: $(LAMENES_SOURCES)
	rm -rf lamenes lamenes.core core.* *.dSYM
