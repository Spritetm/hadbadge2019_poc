#include "gloss/mach_defines.h"
#pragma once

mach_tilemap_t *load_tilemap(const char *filename, const char *layer);
uint8_t *load_tilegfx(const char *filename);
uint8_t *load_file(const char *filename);
