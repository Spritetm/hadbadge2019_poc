#include <stdint.h>
#include "gloss/mach_defines.h"
#pragma once

void gfx_fb_set_mem(uint8_t *fbmem, int width, int height);
void gfx_fb_set_scroll(int scrollx, int scrolly);
void gfx_bgnd_set_map(int bgndid, mach_tilemap_t *map);
void gfx_bgnd_set_gfx(int bgndid, uint8_t *gfx, int trans_col);
void gfx_bgnd_set_scroll(int bgndid, int scrollx, int scrolly);
int gfx_bgnd_get_h(int bgndid);
int gfx_bgnd_get_w(int bgndid);
void gfx_wait_vbl();
int gfx_get_scanline();
