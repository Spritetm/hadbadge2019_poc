#include "iomem.h"
#include <stdio.h>

typedef int buttons_get_t();

typedef struct {
	buttons_get_t *buttons_get_cb;
	PhysMemoryRange *mem_range;
} peri_buttons_t;

peri_buttons_t *peri_buttons_init(PhysMemoryMap *mmap, uint64_t base_addr, buttons_get_t *buttons_get_fn);
