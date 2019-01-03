#include "iomem.h"
#include <stdio.h>
#include <stdlib.h>
#include "peri_cheat.h"
#include "mach_defines.h"
#include "tile_loader.h"

static int get_file_size(const char *filename) {
	FILE *f=fopen(filename, "r");
	if (!f) return -1;
	fseek(f, 0, SEEK_END);
	int ret=ftell(f);
	fclose(f);
	return ret;
}

static void load_file(const char *filename, uint8_t *out) {
	FILE *f=fopen(filename, "r");
	if (!f) return;
	fread(out, 1, 8*1024*1024, f);
	fclose(f);
}

static void cheat_write_fn(void *opaque, uint32_t offset, uint32_t val, int size_log2) {
	peri_cheat_t *p=(peri_cheat_t*)opaque;
	if (offset==CHEAT_FN_ADDR_REG) {
		p->filename_addr=val;
	} else if (offset==CHEAT_LAYERNAME_ADDR_REG) {
		p->layer_addr=val;
	} else if (offset==CHEAT_TILEGFX_READ_ADDR_REG) {
		tile_loader_gen_tileset(&p->mem[p->filename_addr], &p->mem[val]);
	} else if (offset==CHEAT_TILEMAP_READ_ADDR_REG) {
		tile_loader_gen_map(&p->mem[p->filename_addr], &p->mem[p->layer_addr], &p->mem[val]);
	} else if (offset==CHEAT_RAW_READ_ADDR_REG) {
		load_file(&p->mem[p->filename_addr], &p->mem[val]);
	} else {
		printf("peri_cheat: Write to undefined reg %x\n", offset);
	}
}


static uint32_t cheat_read_fn(void *opaque, uint32_t offset, int size_log2) {
	peri_cheat_t *p=(peri_cheat_t*)opaque;
	uint32_t ret=0;
	if (offset==CHEAT_FN_ADDR_REG) {
		ret=p->filename_addr;
	} else if (offset==CHEAT_LAYERNAME_ADDR_REG) {
		ret=p->layer_addr;
	} else if (offset==CHEAT_TILEGFX_SIZE_REG) {
		ret=tile_loader_gen_tileset(&p->mem[p->filename_addr], NULL);
		printf("Get size of tileset %s: %d\n", &p->mem[p->filename_addr], ret);
	} else if (offset==CHEAT_TILEMAP_SIZE_REG) {
		ret=tile_loader_gen_map(&p->mem[p->filename_addr], &p->mem[p->layer_addr], NULL);
		printf("Get size of map %s: %d\n", &p->mem[p->filename_addr], ret);
	} else if (offset==CHEAT_RAW_SIZE_REG) {
		ret=get_file_size(&p->mem[p->filename_addr]);
		printf("Get size of file %s: %d\n", &p->mem[p->filename_addr], ret);
	} else {
		printf("Cheat: Read from undefined reg %x\n", offset);
	}
	return ret;
}

peri_cheat_t *peri_cheat_init(PhysMemoryMap *mmap, uint64_t base_addr, uint8_t *mem) {
	peri_cheat_t *p=calloc(sizeof(peri_cheat_t), 1);
	if (!p) return NULL;
	p->mem=mem;
	p->mem_range=cpu_register_device(mmap, base_addr, 0x100, p, cheat_read_fn, cheat_write_fn, DEVIO_SIZE32);
	return p;
}
