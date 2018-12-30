#pragma once

int gfx_init();
void gfx_clear();
void gfx_render_fbmem_scanline(int ypos, const uint8_t *scanline);
int gfx_flip();
