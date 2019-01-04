#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "loader.h"

//Use the cheat interface to load tilemaps etc

extern volatile uint32_t CHEAT[];
#define CHEATREG(i) CHEAT[i/4]

mach_tilemap_t *load_tilemap(const char *filename, const char *layer) {
	CHEATREG(CHEAT_FN_ADDR_REG)=(uint32_t)filename;
	CHEATREG(CHEAT_LAYERNAME_ADDR_REG)=(uint32_t)layer;
	uint32_t size=CHEATREG(CHEAT_TILEMAP_SIZE_REG);
	mach_tilemap_t *ret=malloc(size);
	if (!ret) return NULL;
	CHEATREG(CHEAT_TILEMAP_READ_ADDR_REG)=(uint32_t)ret;
	return ret;
}

uint8_t *load_tilegfx(const char *filename) {
	CHEATREG(CHEAT_FN_ADDR_REG)=(uint32_t)filename;
	uint32_t size=CHEATREG(CHEAT_TILEGFX_SIZE_REG);
	uint8_t *ret=malloc(size);
	if (!ret) return NULL;
	CHEATREG(CHEAT_TILEGFX_READ_ADDR_REG)=(uint32_t)ret;
	return ret;
}

uint8_t *load_file(const char *filename) {
	CHEATREG(CHEAT_FN_ADDR_REG)=(uint32_t)filename;
	uint32_t size=CHEATREG(CHEAT_RAW_SIZE_REG);
	uint8_t *ret=malloc(size);
	if (!ret) return NULL;
	CHEATREG(CHEAT_RAW_READ_ADDR_REG)=(uint32_t)ret;
	return ret;
}

