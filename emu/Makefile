TARGET := emu
OBJS := cutils.o iomem.o riscv_cpu.o softfp.o peri_uart.o main.o
LDFLAGS := -lm
CFLAGS := -ggdb -DCONFIG_RISCV_MAX_XLEN=32 -DMAX_XLEN=32

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean