**********************
*** LameNES README ***
**********************


*************
*** Intro ***
*************

LameNES is a Nintendo Entertainment System (NES) Emulator programmed by Joey Loman.
 
The project goal was to learn how to write an emulator from scratch and to write it in portable C code so
that it compiles on many platforms.

The LameNES 6502 CPU emulator code (Lame6502) is written to be modular so it can be used in other emulators as well.
The separated Lame6502 source code can also be downloaded from the download section.

LameNES also has an excellent interactive debugger in it. It has a lot of features to make it easy to debug
your nescode.

LameNES is written in C with SDL and is availabe in many ports. The source is released under the BSD license.
It compiles without any problems with GNU GCC on most unix platforms and MinGW on windows.
Feel free to port it to other systems and send me them.

To build and install lamenes for your favorite system, check the INSTALL file included in the source code.


******************
*** Disclaimer ***
******************

The author shall not be held responsible for any damage
or loss caused by this software. Use it at your own risk.

LameNES may only be used with ROMs that are legally owned
by the user. The author does not condone piracy.

LameNES may NOT be distributed with ROMs of any kind. All of these
conditions also apply to any derivatives of LameNES.

LameNES was written using 100% legally reverse-engineered information.

Any similarity between the name "LameNES" and the name of any
other piece of software is purely coincidental. Nintendo,
Nintendo Entertainment System and NES are registered trademarks
of Nintendo of America.


***************
*** License ***
***************

LameNES - Nintendo Entertainment System (NES) emulator

Copyright (c) 2005, Joey Loman, <joey@lamenes.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
 3. Neither the name of the author nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.


*************************
*** Released versions ***
*************************

-----------
26-12-2006:
-----------
Public version 0.1:
- Full Super Mario Bros support!


***********************
*** What's emulated ***
***********************

- 6502 CPU
- 2C02 PPU (Pixel Processing Unit)
- NES memory
- Mapper 0 (No mapper -> default NES)
- Mapper 1 (mmc1) (not fully emulated)
- Mapper 2 (unrom)
- Mapper 3 (cnrom)
- Mapper 4 (mmc3) (not fully emulated)
- Controller 1


***************************
*** System requirements ***
***************************

Minimum system:
I don't know, i don't have a machine that needs frameskips.

Recommended system: 
Pentium II or higher (based on the delay on my slowest system).
Graphic card which is capable of displaying al least 256 colors.

Notes:
If your system is too fast you can use the delay function,
if it is too slow you can use the frameskip function.


***********************
*** Getting started ***
***********************

Unix/Linux/BSD: ./lamenes [options] <rom-filename>
 or
Windows: lamenes.exe [options] <rom-filename> 

options:
        -debug                  -> enable debugger (F12)
        -startdebug             -> enable debugger (F12) and start it immediately

        -scale <value>          -> scale the screen
        -fullscreen             -> toggle fullscreen

        -frameskip <value>      -> frameskip (speed up emulation)
        -delay <value>          -> delay (slow down emulation)

        -pal                    -> pal video mode (default)
        -ntsc                   -> ntsc video mode

Note: The debugger cannot be used in Windows if it is compiled using MinGW.
MinGW redirects the console ouput to a txt file (stdout.txt) and causes the
emulator to hang in the debugger.


*******************
*** Keybindings ***
*******************

LameNES uses the folowing keybindings:

-------------
controller 1:
-------------
up      = up arrow
down    = down arrow
left    = left arrow
right   = right arrow
A       = X
B       = Z
start   = <left ctrl>
select  = <left shift>

---------------
extra features:
---------------
reset                            = <F1>
load state                       = <F3>
save state                       = <F6>
pause                            = P
quit                             = Q or <ESC>
show/hide background             = <F10>
show/hide sprites                = <F11>
enter debugger (only if enabled) = <F12>


***************
*** Credits ***
***************

- Daniel Boris for getting me started with his "how to write an emulator" document.
- RTK and CricketNE for the 6502 Instruction Summary.
- Marat Fayzullin for his NES documentation.
- Jeremy Chadwick for his NES documentation.
- Patrick Diskin for his NES documentation.
- Brad Taylor for his PPU documentation.
- Loopy for his scrolling information.
- Cenobyte for the beginning of the PPU/Graphical implementation.
- Firebug for his mapper documentation.
- And all the people i forgot to mention.


Copyright (c) 2005, Joey Loman <joey@lamenes.org> - http://www.lamenes.org
