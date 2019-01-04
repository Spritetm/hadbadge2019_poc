#include "iomem.h"
#include <stdio.h>
#include <stdlib.h>
#include "peri_gfx.h"
#include "mach_defines.h"
#include "sdl_glue.h"

#define FBH 240
#define FBW 320

static void gfx_write_fn(void *opaque, uint32_t offset, uint32_t val, int size_log2) {
	peri_gfx_t *g=(peri_gfx_t*)opaque;
	int bgnd_per_idx=(offset-GFX_BGND_OFFSET_BASE)/GFX_BGND_OFFSET_SIZE;
	if (offset==GFX_FB_BASE_ADDR_REG) {
		g->fb_base=val;
	} else if (offset==GFX_FB_PITCH_REG) {
		g->fb_pitch=val;
	} else if (offset==GFX_FB_HEIGHT_REG) {
		g->fb_height=val;
	} else if (offset==GFX_FB_SCROLLX_REG) {
		g->fb_scrollx=val;
	} else if (offset==GFX_FB_SCROLLY_REG) {
		g->fb_scrolly=val;
	} else if (offset==GFX_SPRITE_TILEGFX_ADDR_REG) {
		g->sprite_tile_addr=val;
	} else if (offset==GFX_SPRITE_LIST_ADDR_REG) {
		g->sprite_list_addr=val;
	} else if (offset==GFX_SPRITE_TRANS_COL_REG) {
		g->sprite_transcol=val;
	} else if (bgnd_per_idx>=0 && bgnd_per_idx<GFX_BGND_COUNT) {
		if (offset==GFX_BGND_TILEMAP_ADDR_REG(bgnd_per_idx)) {
			g->bgnd[bgnd_per_idx].map_addr=val;
		} else if (offset==GFX_BGND_TILEGFX_ADDR_REG(bgnd_per_idx)) {
			g->bgnd[bgnd_per_idx].gfx_addr=val;
		} else if (offset==GFX_BGND_WIDTH_REG(bgnd_per_idx)) {
			g->bgnd[bgnd_per_idx].width=val;
		} else if (offset==GFX_BGND_HEIGHT_REG(bgnd_per_idx)) {
			g->bgnd[bgnd_per_idx].height=val;
		} else if (offset==GFX_BGND_SCROLLX_REG(bgnd_per_idx)) {
			g->bgnd[bgnd_per_idx].scrollx=val;
		} else if (offset==GFX_BGND_SCROLLY_REG(bgnd_per_idx)) {
			g->bgnd[bgnd_per_idx].scrolly=val;
		} else if (offset==GFX_BGND_TRANS_COL_REG(bgnd_per_idx)) {
			g->bgnd[bgnd_per_idx].transcol=val;
		} else {
			printf("gfx: bgnd peri %d: write to undefined reg %x\n", bgnd_per_idx, offset);
		}
	} else {
		printf("gfx: Write to undefined reg %x\n", offset);
	}
}

static uint32_t gfx_read_fn(void *opaque, uint32_t offset, int size_log2) {
	peri_gfx_t *g=(peri_gfx_t*)opaque;
	uint32_t ret=0;
	int bgnd_per_idx=(offset-GFX_BGND_OFFSET_BASE)/GFX_BGND_OFFSET_SIZE;
	if (offset==GFX_ST_SCANLINE_REG) {
		ret=g->last_scanline;
	} else if (offset==GFX_ST_FLAG_REG) {
		ret=0;
	} else if (offset==GFX_FB_BASE_ADDR_REG) {
		ret=g->fb_base;
	} else if (offset==GFX_FB_PITCH_REG) {
		ret=g->fb_pitch;
	} else if (offset==GFX_FB_HEIGHT_REG) {
		ret=g->fb_height;
	} else if (offset==GFX_FB_SCROLLX_REG) {
		ret=g->fb_scrollx;
	} else if (offset==GFX_FB_SCROLLY_REG) {
		ret=g->fb_scrolly;
	} else if (offset==GFX_SPRITE_TILEGFX_ADDR_REG) {
		ret=g->sprite_tile_addr;
	} else if (offset==GFX_SPRITE_LIST_ADDR_REG) {
		ret=g->sprite_list_addr;
	} else if (offset==GFX_SPRITE_TRANS_COL_REG) {
		ret=g->sprite_transcol;
	} else if (bgnd_per_idx>=0 && bgnd_per_idx<GFX_BGND_COUNT) {
		if (offset==GFX_BGND_TILEMAP_ADDR_REG(bgnd_per_idx)) {
			ret=g->bgnd[bgnd_per_idx].map_addr;
		} else if (offset==GFX_BGND_TILEGFX_ADDR_REG(bgnd_per_idx)) {
			ret=g->bgnd[bgnd_per_idx].gfx_addr;
		} else if (offset==GFX_BGND_WIDTH_REG(bgnd_per_idx)) {
			ret=g->bgnd[bgnd_per_idx].width;
		} else if (offset==GFX_BGND_HEIGHT_REG(bgnd_per_idx)) {
			ret=g->bgnd[bgnd_per_idx].height;
		} else if (offset==GFX_BGND_SCROLLX_REG(bgnd_per_idx)) {
			ret=g->bgnd[bgnd_per_idx].scrollx;
		} else if (offset==GFX_BGND_SCROLLY_REG(bgnd_per_idx)) {
			ret=g->bgnd[bgnd_per_idx].scrolly;
		} else if (offset==GFX_BGND_TRANS_COL_REG(bgnd_per_idx)) {
			ret=g->bgnd[bgnd_per_idx].transcol;
		} else {
			printf("gfx: bgnd peri %d: read from undefined reg %x\n", bgnd_per_idx, offset);
		}
	} else {
		//todo: readback bgnd regs
		printf("gfx: Read from undefined reg %x\n", offset);
	}
	return ret;
}

peri_gfx_t *peri_gfx_init(PhysMemoryMap *mmap, uint64_t base_addr) {
	peri_gfx_t *g=calloc(sizeof(peri_gfx_t), 1);
	if (!g) return NULL;
	g->mem_range=cpu_register_device(mmap, base_addr, 0x1000, g, gfx_read_fn, gfx_write_fn, DEVIO_SIZE32);
	gfx_init();
	return g;
}

static void gfx_render_fb_scanline(uint8_t *scanline, int ypos, peri_gfx_t *g, const uint8_t *mem) {
	if (!g->fb_base) return;
	ypos=ypos+g->fb_scrolly;
	if (ypos>=g->fb_height) ypos-=g->fb_height;
	int xpos=g->fb_scrollx;
	const uint8_t *p=&mem[g->fb_base+(ypos*g->fb_pitch)];
	for (int i=0; i<FBW; i++) {
		scanline[i]=p[xpos++];
		if (xpos>=g->fb_pitch) xpos=0;
	}
}

static void gfx_render_bgnd_scanline(uint8_t *scanline, int ypos, peri_gfx_bgnd_t *b, const uint8_t *mem) {
	if (!b->gfx_addr || !b->map_addr) return;
	if (!b->height || !b->width) return;

	ypos=ypos+b->scrolly;
	while (ypos>=b->height*8) ypos-=b->height*8;
	int xpos=b->scrollx;
	while (xpos>=b->width*8) xpos-=b->width*8;

	uint16_t *tiledata=(uint16_t*)(&mem[b->map_addr]);
	for (int i=0; i<FBW; i++) {
		int tileidx=(ypos/8)*b->width+(xpos/8);
		int tileno=tiledata[tileidx];
		const uint8_t *tile=&mem[b->gfx_addr+64*tileno];
		uint8_t col=tile[(ypos%8)*8+(xpos%8)];
		if (col!=b->transcol) scanline[i]=col;
		xpos++;
		if (xpos>=b->width*8) xpos-=b->width*8;
	}
}

//ToDo: Relative offset to sprite pos, to easily scroll screen?
static void gfx_render_sprites(uint8_t *scanline, int ypos, peri_gfx_t *g, const uint8_t *mem) {
	if (g->sprite_tile_addr==0 || g->sprite_list_addr==0) return;
	const uint8_t *tiles=&mem[g->sprite_tile_addr];
	mach_sprite_t *sprite=(mach_sprite_t*)&mem[g->sprite_list_addr];
	int sprite_limit=200;
	while (sprite->ypos!=SPRITE_LIST_END_MARKER) {
		int sp_y=ypos-sprite->ypos;
		if (sp_y>=0 && sp_y<8) {
			const uint8_t *gfx=&tiles[sprite->tile]+sp_y*8;
			int p_x=sprite->xpos;
			for (int x=0; x<8; x++) {
				if (p_x>=0 && p_x<320) {
					if (gfx[x]!=g->sprite_transcol) scanline[p_x]=gfx[x];
				}
				p_x++;
			}
		}
		sprite++;
		sprite_limit--;
		if (sprite_limit==0) {
			fprintf(stderr, "Warning: ran into sprite limit, not rendering more sprites!\n");
		}
	}
}

void peri_gfx_render_scanline(int ypos, peri_gfx_t *g, const uint8_t *mem) {
	uint8_t scanline[320];
	if (ypos==0) gfx_clear();
	gfx_render_fb_scanline(scanline, ypos, g, mem);
	for (int i=0; i<GFX_BGND_COUNT; i++) {
		gfx_render_bgnd_scanline(scanline, ypos, &g->bgnd[i], mem);
	}
	gfx_render_sprites(scanline, ypos, g, mem);
	gfx_render_fbmem_scanline(ypos, scanline);
	if (ypos==239) gfx_flip();
	g->last_scanline=ypos;
}
