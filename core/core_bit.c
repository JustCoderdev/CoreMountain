/* Bit module for JustCoderdev Core library v2
 * */

#include <core.h>

#include <stdio.h>
#include <stdlib.h>

void printc(char c) {
#ifdef DEBUG_ENABLE
	switch(c) {
		case '\0': printf(CSI FG_BLUE    M "\\0" CSI RESET M); return;

		case '\r': printf(CSI FG_MAGENTA M "\\r" CSI RESET M); return;
		case '\n': printf(CSI FG_MAGENTA M "\\n" CSI RESET M "\n"); return;

		case  ' ': printf(CSI BG_CYAN M "."     CSI RESET M); return;
		case '\t': printf(CSI BG_CYAN M "\\t--" CSI RESET M); return;
	}

	if(c < 21 || c > 126) {
		printf(CSI FG_RED M "x" CSI RESET M);
		return;
	}
#endif

	putchar(c);
}

void printb(n8 byte)
{
	printf("%c%c%c%c%c%c%c%c",
	       byte & 0x80 ? '1' : '0',
	       byte & 0x40 ? '1' : '0',
	       byte & 0x20 ? '1' : '0',
	       byte & 0x10 ? '1' : '0',
	       byte & 0x08 ? '1' : '0',
	       byte & 0x04 ? '1' : '0',
	       byte & 0x02 ? '1' : '0',
	       byte & 0x01 ? '1' : '0');
}

void printw(n32 word)
{
	printb((n8)(word >> 24));
	printb((n8)(word >> 16));
	printb((n8)(word >> 8));
	printb((n8)(word));
}

