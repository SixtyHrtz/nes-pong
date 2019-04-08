#include "../libs/neslib.h"
#include "../libs/nesdoug.h"
#include "../nes-st/palette.h"
#include "../nes-st/field.h"
#include "../nes-st/sprites.h"

#pragma bss-name(push, "ZEROPAGE")

#define screen_w 256
#define screen_w_half 128
#define screen_h 224
#define screen_h_half 112
#define row_h_skip 7

struct Platform
{
	unsigned char x;
	unsigned char y;
	unsigned char width;
	unsigned char height;
};

struct Ball
{
	unsigned char x;
	unsigned char y;
	unsigned char width;
	unsigned char height;
	unsigned char x_direction;
	unsigned char y_direction;
};

struct Platform platform_p1 = {
	0,
	screen_h_half - 16 + row_h_skip,
	4 - 1,
	32 - 1};

struct Platform platform_p2 = {
	screen_w - 4,
	screen_h_half - 16 + row_h_skip,
	4 - 1,
	32 - 1};

struct Ball ball = {
	screen_w_half - 4,
	screen_h_half - 4 + row_h_skip,
	8 - 1,
	8 - 1,
	1,
	1};

unsigned char pad1;
unsigned char pad2;
unsigned char endgame = 1;

void movement(void);
void draw(void);
void collision_check(void);

void main(void)
{
	ppu_off();

	pal_bg(palette);
	pal_spr(palette);

	vram_adr(NAMETABLE_A);
	vram_unrle(field);

	ppu_on_all();

	while (1)
	{
		ppu_wait_nmi();

		pad1 = pad_poll(0);
		pad2 = pad_poll(1);

		movement();
		collision_check();
		draw();
	}
}

void movement(void)
{
	if (endgame == 1)
	{
		if (pad1 & PAD_START)
		{
			platform_p1.y = screen_h_half - 16 + row_h_skip;
			platform_p2.y = screen_h_half - 16 + row_h_skip;
			ball.x = screen_w_half;
			ball.y = screen_h_half;
			endgame = 0;
		}
	}
	else
	{
		if (pad1 & PAD_UP)
			platform_p1.y -= 1;
		else if (pad1 & PAD_DOWN)
			platform_p1.y += 1;

		if (pad2 & PAD_UP)
			platform_p2.y -= 1;
		else if (pad2 & PAD_DOWN)
			platform_p2.y += 1;

		ball.x += ball.x_direction;
		ball.y += ball.y_direction;
	}
}

unsigned char sprite_index;

void draw(void)
{
	oam_clear();

	sprite_index = 0;
	sprite_index = oam_meta_spr(platform_p1.x, platform_p1.y, sprite_index, panel);
	sprite_index = oam_meta_spr(platform_p2.x, platform_p2.y, sprite_index, panel);
	sprite_index = oam_spr(ball.x, ball.y, 0x91, 0, sprite_index);
}

unsigned char collision1;
unsigned char collision2;

void collision_check(void)
{
	collision1 = check_collision(&platform_p1, &ball);
	collision2 = check_collision(&platform_p2, &ball);

	if (collision1 || collision2)
	{
		ball.x_direction *= -1;
		*((unsigned char *)0x4000) = 0x0f;
		*((unsigned char *)0x4001) = 0xab;
		*((unsigned char *)0x4003) = 0x01;
	}

	if (ball.y <= 4 + 4)
		ball.y_direction *= -1;
	if (ball.y >= screen_h - 4 - 4)
		ball.y_direction *= -1;

	if (ball.x <= 0)
		endgame = 1;
	if (ball.x >= screen_w - 8)
		endgame = 1;
}
