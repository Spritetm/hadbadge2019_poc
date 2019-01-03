#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "gloss/mach_defines.h"

#include "gfx.h"

extern volatile uint32_t GFX[];
#define GFXREG(i) GFX[i/4]

void do_test() {
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
	GFXREG(GFX_FB_PITCH_REG)=320;
	GFXREG(GFX_FB_HEIGHT_REG)=240;
	GFXREG(GFX_FB_BASE_ADDR_REG)=(uint32_t)fb_mem;

	GFXREG(GFX_BGND_WIDTH_REG(0))=map_level1_fgnd.w;
	GFXREG(GFX_BGND_HEIGHT_REG(0))=map_level1_fgnd.h;
	GFXREG(GFX_BGND_TILEGFX_ADDR_REG(0))=(uint32_t*)map_level1_fgnd.gfx->tile;
	GFXREG(GFX_BGND_TRANS_COL_REG(0))=map_level1_fgnd.gfx->trans_col;
	GFXREG(GFX_BGND_TILEMAP_ADDR_REG(0))=(uint32_t*)map_level1_fgnd.tiles;

	GFXREG(GFX_BGND_SCROLLX_REG(0))=0;
	GFXREG(GFX_BGND_SCROLLY_REG(0))=0;

	int  x=0, y=0;
	int tx=0, ty=0;
	while(1) {
		//Hacky: wait for vbl
		while (GFXREG(GFX_ST_SCANLINE_REG)!=238) ;
		while (GFXREG(GFX_ST_SCANLINE_REG)!=239) ;
		x++;
		if (x>=320) x=0;
		y++;
		if (y>=240) y=0;
		GFXREG(GFX_FB_SCROLLX_REG)=x;
		GFXREG(GFX_FB_SCROLLY_REG)=y;

		tx++; if (tx>map_level1_fgnd.w*8) tx=0;
//		ty++; if (ty>map_level1_fgnd.h*8) ty=0;

		GFXREG(GFX_BGND_SCROLLX_REG(0))=tx;
		GFXREG(GFX_BGND_SCROLLY_REG(0))=ty;

	}
}