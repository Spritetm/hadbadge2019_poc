#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "driver/loader.h"
#include "driver/gfxdriver.h"
#include "driver/buttons.h"
#include <assert.h>


void do_test() {
	printf("Do_test starting...\n");
	uint8_t *fb_mem=calloc(320,240);
	assert(fb_mem);
	//Fill mem with pattern
	int i=0;
	for (int y=0; y<240; y++) {
		for (int x=0; x<320; x++) {
			fb_mem[i++]=x^y;
		}
	}
	//Set up FB
	gfx_fb_set_mem(fb_mem, 320, 240);

	mach_tilemap_t *map=load_tilemap("tile/level1.tmx", "fgnd");
	uint8_t *gfx=load_tilegfx("tile/beastlands.png");
	gfx_bgnd_set_map(0, map);
	gfx_bgnd_set_gfx(0, gfx, 0x57);

	int  x=0, y=0;
	int tx=0, ty=0;
	while(1) {
		gfx_wait_vbl();
		x++; if (x>=320) x=0;
		y++; if (y>=240) y=0;
		gfx_fb_set_scroll(x, y);

		int b=buttons_get_state();
		if (b&BUTTON_UP) ty--;
		if (b&BUTTON_DOWN) ty++;
		if (b&BUTTON_LEFT) tx--;
		if (b&BUTTON_RIGHT) tx++;
		if (tx>map->w*8) tx=0;
		if (tx<0) tx+=map->w*8;
		if (ty>map->h*8) ty=0;
		if (ty<0) ty+=map->h*8;
		gfx_bgnd_set_scroll(0, tx, ty);
	}
}