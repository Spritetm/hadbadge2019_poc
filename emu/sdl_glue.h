#pragma once

#include "mach_defines.h"

int gfx_init();
void gfx_clear();
void gfx_render_fbmem_scanline(int ypos, const uint8_t *scanline);
int gfx_flip();

int gfx_poll_event();
int gfx_get_buttons();




