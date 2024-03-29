#include <stdint.h>
#pragma once

#define MACH_RAM_START	0
#define MACH_RAM_SIZE	(8*1024*1024)

#define UART_OFFSET		0x10000000
#define UART_DATA_REG	0x0
#define UART_FLAG_REG	0x4
#define UART_FLAG_RXAV		(1<<0)
#define UART_FLAG_TXDONE	(1<<1)

#define GFX_OFFSET 0x10010000
#define GFX_ST_SCANLINE_REG 0x0
#define GFX_ST_FLAG_REG 0x4
#define GFX_ST_FLAG_WORKING (1<<0)
#define GFX_FB_BASE_ADDR_REG 0x10
#define GFX_FB_PITCH_REG 0x14
#define GFX_FB_HEIGHT_REG 0x18
#define GFX_FB_SCROLLX_REG 0x1C
#define GFX_FB_SCROLLY_REG 0x20
#define GFX_SPRITE_TILEGFX_ADDR_REG 0x24
#define GFX_SPRITE_LIST_ADDR_REG 0x28
#define GFX_SPRITE_TRANS_COL_REG 0x2C

#define GFX_BGND_OFFSET_BASE 0x40
#define GFX_BGND_OFFSET_SIZE 0x20
#define GFX_BGND_COUNT 3
#define GFX_BGND_OFFSET(i) (i*GFX_BGND_OFFSET_SIZE+GFX_BGND_OFFSET_BASE)
#define GFX_BGND_TILEMAP_ADDR_REG(i) (GFX_BGND_OFFSET(i)+0x0)
#define GFX_BGND_TILEGFX_ADDR_REG(i) (GFX_BGND_OFFSET(i)+0x4)
#define GFX_BGND_WIDTH_REG(i) (GFX_BGND_OFFSET(i)+0x8)
#define GFX_BGND_HEIGHT_REG(i) (GFX_BGND_OFFSET(i)+0xC)
#define GFX_BGND_SCROLLX_REG(i) (GFX_BGND_OFFSET(i)+0x10)
#define GFX_BGND_SCROLLY_REG(i) (GFX_BGND_OFFSET(i)+0x14)
#define GFX_BGND_TRANS_COL_REG(i) (GFX_BGND_OFFSET(i)+0x18)

//'Cheat' interface; used to ask the emulator to do high-level functions that would
//normally be implemented in a secondary processor or whatever.
#define CHEAT_OFFSET 0x10020000
#define CHEAT_FN_ADDR_REG 0x0
#define CHEAT_TILEGFX_SIZE_REG 0x4
#define CHEAT_TILEGFX_READ_ADDR_REG 0x8
#define CHEAT_LAYERNAME_ADDR_REG 0xC
#define CHEAT_TILEMAP_SIZE_REG 0x10
#define CHEAT_TILEMAP_READ_ADDR_REG 0x14
#define CHEAT_RAW_SIZE_REG 0x18
#define CHEAT_RAW_READ_ADDR_REG 0x1C


#define BUTTON_OFFSET 0x10030000
#define BUTTON_READ_REG 0x0
#define BUTTON_UP (1<<0)
#define BUTTON_DOWN (1<<1)
#define BUTTON_LEFT (1<<2)
#define BUTTON_RIGHT (1<<3)
#define BUTTON_A (1<<4)
#define BUTTON_B (1<<5)
#define BUTTON_SELECT (1<<6)
#define BUTTON_START (1<<7)

typedef struct __attribute__((packed)) {
	uint16_t w;
	uint16_t h;
	uint16_t tiles[];
} mach_tilemap_t;

//special ypos values
#define SPRITE_LIST_END_MARKER -32768
#define SPRITE_HIDE -32767
typedef struct __attribute__((packed)) {
	int16_t xpos;
	int16_t ypos;
	uint16_t tile;
	uint16_t flags; //whatever
} mach_sprite_t;


