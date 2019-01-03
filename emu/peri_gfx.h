#include "iomem.h"
#include <stdio.h>
#include "mach_defines.h"

#pragma once

typedef struct {
	uint32_t map_addr;
	uint32_t gfx_addr;
	uint32_t width;
	uint32_t height;
	uint32_t scrollx;
	uint32_t scrolly;
	uint32_t transcol;
} peri_gfx_bgnd_t;

typedef struct {
	PhysMemoryRange *mem_range;
	int last_scanline;
	uint32_t fb_base;
	uint32_t fb_pitch;
	uint32_t fb_height;
	uint32_t fb_scrollx;
	uint32_t fb_scrolly;
	peri_gfx_bgnd_t bgnd[GFX_BGND_COUNT];
} peri_gfx_t;

peri_gfx_t *peri_gfx_init(PhysMemoryMap *mmap, uint64_t base_addr);
void peri_gfx_render_scanline(int ypos, peri_gfx_t *gfx, const uint8_t *mem);
