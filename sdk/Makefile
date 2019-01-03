TC_PATH := /home/jeroen/hackaday2019/poc/toolchain/toolchain/bin
NEWLIB_PATH := /home/jeroen/hackaday2019/poc/toolchain/toolchain/bin
EMU_PATH := ../../poc/emu

PREFIX := $(TC_PATH)/riscv32-unknown-elf-
CC := $(PREFIX)gcc
AR := $(PREFIX)ar
LD := $(PREFIX)ld
OBJCOPY := $(PREFIX)objcopy
SIZE := $(PREFIX)size

APPNAME := app
TARGET_ELF := $(APPNAME).elf
TARGET_BIN := $(APPNAME).bin
TARGET_MAP := $(APPNAME).map
OBJS := main.o program.o test.o gfx.o
LIBS := gloss/libgloss.a mybasic/libmybasic.a
LDSCRIPT := gloss/ldscript.ld

CFLAGS := -Og -ggdb -Imybasic
LDFLAGS := -Wl,-Bstatic -Wl,-T,$(LDSCRIPT) -Wl,-Map,$(TARGET_MAP) -lgcc -nostartfiles -Lgloss 

export PREFIX CC AR LD OBJCOPY CFLAGS LDFLAGS APPNAME

$(TARGET_BIN): $(TARGET_ELF)
	$(SIZE) $^
	$(OBJCOPY) -O binary $^ $@

$(TARGET_ELF): $(LIBS) $(OBJS) $(LDSCRIPT)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS) -lm

program.o: program.bas
	$(OBJCOPY) -I binary -O elf32-littleriscv -B riscv:rv32 $^ $@

.PHONY: emu
emu: $(TARGET_BIN)
	CC="cc" make -C $(EMU_PATH)
	$(EMU_PATH)/emu $(TARGET_BIN)

.PHONY: clean
clean:
	rm -f $(TARGET_ELF) $(TARGET_BIN) $(OBJS) $(TARGET_MAP)
	$(MAKE) -C gloss clean
	$(MAKE) -C mybasic clean

.PHONY: gloss/libgloss.a
gloss/libgloss.a:
	$(MAKE) -C gloss

.PHONY: mybasic/libmybasic.a
mybasic/libmybasic.a:
	$(MAKE) -C mybasic

