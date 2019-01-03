#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "gloss/mach_defines.h"

extern uint32_t GFX[];
#define GFXREG(i) GFX[i/4]

static uint8_t *gfx_fb_mem;
static int gfx_fb_h, gfx_fb_w;

//Note: FB is nice, but how about wraparound when scrolling? Atm, it just grabs whatever mem is outside of the FB...


void gfx_init_fb(int height, int width) {
	
}

void do_test() {
	uint8_t *fb_mem=calloc(320,240);
	assert(fb_mem);
	for (int x=0; x<320*240; x++) {
		fb_mem[x]=x&0xff;
	}
	GFXREG(GFX_FB_PITCH_REG)=320;
	GFXREG(GFX_FB_BASE_ADDR_REG)=(uint32_t)fb_mem;
}