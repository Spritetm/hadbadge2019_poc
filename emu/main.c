#include <stdio.h>
#include <stdint.h>

#include "iomem.h"
#include "riscv_cpu.h"
#include "mach_defines.h"

#include "peri_uart.h"
#include "peri_gfx.h"
#include "peri_cheat.h"
#include "peri_buttons.h"
#include "sdl_glue.h"

#include <sys/select.h>
#include <sys/time.h>


RISCVCPUState *cpu;
PhysMemoryMap *memmap;
PhysMemoryRange *ram;

peri_gfx_t *peri_gfx;
peri_uart_t *peri_uart;
peri_cheat_t *peri_cheat;
peri_buttons_t *peri_buttons;

static int buttons_get() {
	return gfx_get_buttons();
}

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

//Instructions per second ('clock speed') of the CPU.
#define CPU_IPS (10*1000*1000)
//Fps the GFX periph runs at
#define GFX_FPS 60

#define FIRST_PAGE_SZ 0x1000

void mach_setup(uint8_t *main_mem) {
	//Initialize memory map
	memmap=phys_mem_map_init();
	//Register main RAM
	//Note that we do not map the first 4K, as we want the CPU to error out if something
	//reads/writes/executes there.
	ram=register_ram_entry(memmap, MACH_RAM_START+FIRST_PAGE_SZ, MACH_RAM_SIZE-FIRST_PAGE_SZ, 0);
	ram->phys_mem=main_mem+FIRST_PAGE_SZ;
	//Register UART
	peri_uart=peri_uart_init(memmap, UART_OFFSET, uart_putchar, uart_getchar);
	//Register GFX peripheral
	peri_gfx=peri_gfx_init(memmap, GFX_OFFSET);
	//Register cheat interface peripheral
	peri_cheat=peri_cheat_init(memmap, CHEAT_OFFSET, main_mem);
	//Register buttons peripheral
	peri_buttons=peri_buttons_init(memmap, BUTTON_OFFSET, buttons_get);


	//Instantiate CPU
	cpu=riscv_cpu_init(memmap, 32); //init 32-bit machine

	struct timeval frame_ex_len={0};
	frame_ex_len.tv_usec=1000000/GFX_FPS;
	while(1) {
		struct timeval frame_start, frame_end, frame_len, delay_len;
		gettimeofday(&frame_start, NULL);
		for (int y=0; y<240; y++) {
			//Run CPU for one scanline
			riscv_cpu_interp(cpu, CPU_IPS/(GFX_FPS*240));
			//Render frame
			peri_gfx_render_scanline(y, peri_gfx, main_mem);
		}
		//Get events
		int quit=gfx_poll_event();
		if (quit) break;
		//Cap fps
		gettimeofday(&frame_end, NULL);
		timersub(&frame_end, &frame_start, &frame_len);
		timersub(&frame_ex_len, &frame_len, &delay_len);
		select(0, NULL, NULL, NULL, &delay_len);
	}
}


int main(int argc, char **argv) {
	if (argc<2) {
		printf("Usage: $s memfile.bin\n", argv[0]);
		exit(0);
	}
	uint8_t *main_mem=calloc(1, 8*1024*1024);
	FILE *f=fopen(argv[1], "rb");
	if (f==NULL) {
		perror(argv[1]);
		exit(1);
	}
	fread(main_mem+FIRST_PAGE_SZ, 1, MACH_RAM_SIZE-FIRST_PAGE_SZ, f);
	fclose(f);
	mach_setup(main_mem);
}