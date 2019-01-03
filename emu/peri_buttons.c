#include "iomem.h"
#include <stdio.h>
#include <stdlib.h>
#include "peri_buttons.h"
#include "mach_defines.h"

static void button_write_fn(void *opaque, uint32_t offset, uint32_t val, int size_log2) {
	peri_buttons_t *b=(peri_buttons_t*)opaque;
	printf("Buttons: Write to undefined reg %x\n", offset);
}

static uint32_t button_read_fn(void *opaque, uint32_t offset, int size_log2) {
	peri_buttons_t *b=(peri_buttons_t*)opaque;
	uint32_t ret=0;
	if (offset==BUTTON_READ_REG) {
		ret=b->buttons_get_cb();
	} else {
		printf("Buttons: Read from undefined reg %x\n", offset);
	}
	return ret;
}

peri_buttons_t *peri_buttons_init(PhysMemoryMap *mmap, uint64_t base_addr, buttons_get_t *buttons_get_fn) {
	peri_buttons_t *b=calloc(sizeof(peri_buttons_t), 1);
	if (!b) return NULL;
	b->buttons_get_cb=buttons_get_fn;
	b->mem_range=cpu_register_device(mmap, base_addr, 0x100, b, button_read_fn, button_write_fn, DEVIO_SIZE32);
	return b;
}
