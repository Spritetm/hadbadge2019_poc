#include "iomem.h"
#include <stdio.h>

typedef void uart_putchar_t(char c);
typedef int uart_getchar_t();

typedef struct {
	uart_putchar_t *putchar_cb;
	uart_getchar_t *getchar_cb;
	PhysMemoryRange *mem_range;
	int buffered_char;
} peri_uart_t;

peri_uart_t *peri_uart_init(PhysMemoryMap *mmap, uint64_t base_addr, uart_putchar_t *putchar_fn, uart_getchar_t *getchar_fn);
