TARGET := emu
OBJS := cutils.o iomem.o riscv_cpu.o softfp.o peri_uart.o peri_gfx.o sdl_glue.o main.o peri_cheat.o tile_loader.o
CFLAGS := -ggdb -DCONFIG_RISCV_MAX_XLEN=32 -DMAX_XLEN=32 `sdl2-config --cflags` $(shell xml2-config --cflags) 
LDFLAGS := -lm `sdl2-config --libs` $(shell xml2-config --libs) -lgd

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean