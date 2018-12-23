#include <stdio.h>
#include <stdint.h>

#include "iomem.h"
#include "riscv_cpu.h"
#include "mach_defines.h"

#include "peri_uart.h"
#include <sys/select.h>

RISCVCPUState *cpu;
PhysMemoryMap *memmap;
PhysMemoryRange *ram;

static void uart_putchar(char c) {
	putchar(c);
}

static int uart_getchar() {
	fd_set rfds;
	struct timeval to={0};
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	select(1, &rfds, NULL, NULL, &to);
	if (!FD_ISSET(0, &rfds)) return EOF;
	return getchar();
}

void mach_setup(uint8_t *main_mem) {
	//Initialize memory map
	memmap=phys_mem_map_init();
	//Register main RAM
	ram=register_ram_entry(memmap, MACH_RAM_START, MACH_RAM_SIZE, 0);
	ram->phys_mem=main_mem;
	//Register UART
	peri_uart_init(memmap, UART_OFFSET, uart_putchar, uart_getchar);

	//Instantiate CPU
	cpu=riscv_cpu_init(memmap, 32); //init 32-bit machine

	while(1) {
		riscv_cpu_interp(cpu, 1024);
	}
}


int main(int argc, char **argv) {
	if (argc<2) {
		printf("Usage: $s memfile.bin\n", argv[0]);
		exit(0);
	}
	uint8_t *main_mem=malloc(8*1024*1024);
	FILE *f=fopen(argv[1], "rb");
	if (f==NULL) {
		perror(argv[1]);
		exit(1);
	}
	fread(main_mem, 1, 8*1024*1024, f);
	fclose(f);
	mach_setup(main_mem);
}