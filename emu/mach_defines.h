

#define MACH_RAM_START	0x1000
#define MACH_RAM_SIZE	(8*1024*1024-0x1000)

#define UART_OFFSET		0x10000000
#define UART_DATA_REG	0x0
#define UART_FLAG_REG	0x4
#define UART_FLAG_RXAV		(1<<0)
#define UART_FLAG_TXDONE	(1<<1)
