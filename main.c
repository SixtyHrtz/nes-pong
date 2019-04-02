#include "libs/neslib.h"
#include "libs/nesdoug.h"

#define BLACK 0x0f
#define DK_GY 0x00
#define LT_GY 0x10
#define WHITE 0x30

#pragma bss - name(push, "ZEROPAGE")

const unsigned char text[] = "Hello World!";

const unsigned char palette[] = {
	BLACK, DK_GY, LT_GY, WHITE,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0};

void main(void)
{
	ppu_off();
	pal_bg(palette);

	vram_adr(NTADR_A(10, 14));
	vram_write(text, sizeof(text));

	ppu_on_all();

	while (1)
	{
	}
}
