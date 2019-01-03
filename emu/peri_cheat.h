#include "iomem.h"
#include <stdint.h>

#pragma once

typedef struct {
	uint8_t *mem;
	PhysMemoryRange *mem_range;
	uint32_t filename_addr;
	uint32_t layer_addr;
} peri_cheat_t;

peri_cheat_t *peri_cheat_init(PhysMemoryMap *mmap, uint64_t base_addr, uint8_t *mem);
