TARGET := emu
OBJS := cutils.o iomem.o riscv_cpu.o softfp.o peri_uart.o peri_gfx.o sdl_glue.o main.o
LDFLAGS := -lm `sdl2-config --libs`
CFLAGS := -ggdb -DCONFIG_RISCV_MAX_XLEN=32 -DMAX_XLEN=32 `sdl2-config --cflags`

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean