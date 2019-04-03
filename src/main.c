#include "../libs/neslib.h"
#include "../libs/nesdoug.h"
#include "../nes-st/palette.h"
#include "../nes-st/field.h"

#pragma bss-name(push, "ZEROPAGE")

void main(void)
{
	ppu_off();
	pal_bg(palette);

	vram_adr(NAMETABLE_A);
	vram_unrle(field);

	ppu_on_all();

	while (1)
	{
	}
}
