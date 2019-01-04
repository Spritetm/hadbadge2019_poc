#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "gloss/mach_defines.h"
#include "loader.h"
#include <assert.h>

extern volatile uint32_t GFX[];
#define GFXREG(i) GFX[i/4]

void gfx_fb_set_mem(uint8_t *fbmem, int width, int height) {
	GFXREG(GFX_FB_PITCH_REG)=width;
	GFXREG(GFX_FB_HEIGHT_REG)=height;
	GFXREG(GFX_FB_BASE_ADDR_REG)=(uint32_t)fbmem;
}

void gfx_fb_set_scroll(int scrollx, int scrolly) {
	GFXREG(GFX_FB_SCROLLX_REG)=scrollx;
	GFXREG(GFX_FB_SCROLLY_REG)=scrolly;
}

void gfx_bgnd_set_map(int bgndid, mach_tilemap_t *map) {
	assert(bgndid<GFX_BGND_COUNT);
	GFXREG(GFX_BGND_WIDTH_REG(bgndid))=map->w;
	GFXREG(GFX_BGND_HEIGHT_REG(bgndid))=map->h;
	GFXREG(GFX_BGND_TILEMAP_ADDR_REG(bgndid))=(uint32_t)map->tiles;
}



int gfx_bgnd_get_w(int bgndid) {
	assert(bgndid<GFX_BGND_COUNT);
	return GFXREG(GFX_BGND_WIDTH_REG(bgndid));
}

int gfx_bgnd_get_h(int bgndid) {
	assert(bgndid<GFX_BGND_COUNT);
	return GFXREG(GFX_BGND_HEIGHT_REG(bgndid));
}

void gfx_bgnd_set_gfx(int bgndid, uint8_t *gfx, int trans_col) {
	assert(bgndid<GFX_BGND_COUNT);
	GFXREG(GFX_BGND_TILEGFX_ADDR_REG(bgndid))=(uint32_t)gfx;
	GFXREG(GFX_BGND_TRANS_COL_REG(bgndid))=(uint32_t)trans_col;
}

void gfx_bgnd_set_scroll(int bgndid, int scrollx, int scrolly) {
	GFXREG(GFX_BGND_SCROLLX_REG(bgndid))=scrollx;
	GFXREG(GFX_BGND_SCROLLY_REG(bgndid))=scrolly;
}

void gfx_wait_vbl() {
	//Hacky: wait for vbl
	while (GFXREG(GFX_ST_SCANLINE_REG)!=238) ;
	while (GFXREG(GFX_ST_SCANLINE_REG)!=239) ;
}

int gfx_get_scanline() {
	return GFXREG(GFX_ST_SCANLINE_REG);
}

void gfx_sprite_set_mem(mach_sprite_t *sprites, uint8_t *gfx, int trans_col) {
	GFXREG(GFX_SPRITE_TILEGFX_ADDR_REG)=(uint32_t)gfx;
	GFXREG(GFX_SPRITE_LIST_ADDR_REG)=(uint32_t)sprites;
	GFXREG(GFX_SPRITE_TRANS_COL_REG)=(uint32_t)trans_col;
}

mach_sprite_t *gfx_get_sprites() {
	return (mach_sprite_t*)(GFXREG(GFX_SPRITE_LIST_ADDR_REG));
}
