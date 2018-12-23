#include "iomem.h"
#include <stdio.h>
#include <stdlib.h>
#include "peri_uart.h"
#include "mach_defines.h"

static void uart_write_fn(void *opaque, uint32_t offset, uint32_t val, int size_log2) {
	peri_uart_t *u=(peri_uart_t*)opaque;
	if (offset==UART_DATA_REG) {
		u->putchar_cb(val&0xff);
	} else {
		printf("UART: Write to undefined reg %x\n", offset);
	}
}

static uint32_t uart_read_fn(void *opaque, uint32_t offset, int size_log2) {
	peri_uart_t *u=(peri_uart_t*)opaque;
	uint32_t ret=0;
	if (offset==UART_DATA_REG) {
		if (u->buffered_char==EOF) u->buffered_char=u->getchar_cb();
		ret=u->buffered_char&0xff;
		u->buffered_char=EOF;
	} else if (offset==UART_FLAG_REG) {
		ret|=UART_FLAG_TXDONE;
		if (u->buffered_char==EOF) u->buffered_char=u->getchar_cb();
		if (u->buffered_char!=EOF) ret|=UART_FLAG_RXAV;
	} else {
		printf("UART: Read from undefined reg %x\n", offset);
	}
	return ret;
}

peri_uart_t *peri_uart_init(PhysMemoryMap *mmap, uint64_t base_addr, uart_putchar_t *putchar_fn, uart_getchar_t *getchar_fn) {
	peri_uart_t *u=calloc(sizeof(peri_uart_t), 1);
	if (!u) return NULL;
	u->putchar_cb=putchar_fn;
	u->getchar_cb=getchar_fn;
	u->buffered_char=EOF;
	u->mem_range=cpu_register_device(mmap, base_addr, 0x100, u, uart_read_fn, uart_write_fn, DEVIO_SIZE32);
	return u;
}
