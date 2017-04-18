#include "io.h"
#include "uart.h"
#include "clk.h"
#include "gpio.h"

#define UART0_BASE	0x01c28000

#define UART0_RBR 	(UART0_BASE + 0x0)    /* receive buffer register */
#define UART0_THR 	(UART0_BASE + 0x0)    /* transmit holding register */
#define UART0_DLL 	(UART0_BASE + 0x0)    /* divisor latch low register */
#define UART0_DLH 	(UART0_BASE + 0x4)    /* divisor latch high register */
#define UART0_IER 	(UART0_BASE + 0x4)    /* interrupt enable reigster */
#define UART0_IIR 	(UART0_BASE + 0x8)    /* interrupt identity register */
#define UART0_FCR 	(UART0_BASE + 0x8)    /* fifo control register */
#define UART0_LCR 	(UART0_BASE + 0xc)    /* line control register */
#define UART0_LSR 	(UART0_BASE + 0x14)   /* line status register */

#define BAUD_115200    	(0xd) /* 24 * 1000 * 1000 / 16 / 115200 = 13 */
#define NO_PARITY      	(0)
#define ONE_STOP_BIT   	(0)
#define DAT_LEN_8_BITS 	(3)
#define LC_8_N_1       	(NO_PARITY << 3 | ONE_STOP_BIT << 2 | DAT_LEN_8_BITS)
#define TX_READY 	(readl(UART0_LSR) & BIT(6))

#ifdef DEBUG

void uart0_init(void)
{
	gpio_init();

	gpio_set_pincfg(GPIO_BANK_A, 4, GPIO_FUNC_BANK_A_UART0);
	gpio_set_pincfg(GPIO_BANK_A, 5, GPIO_FUNC_BANK_A_UART0);

	clk_set_rate(CLK_UART0, 24000000);
	clk_enable(CLK_UART0);

	/* select dll dlh */
	writel(0x80, UART0_LCR);
	/* set baudrate */
	writel(0, UART0_DLH);
	writel(BAUD_115200, UART0_DLL);
	/* set line control */
	writel(LC_8_N_1, UART0_LCR);
}

void uart0_putc(char c)
{
	while (!TX_READY);

	writel(c, UART0_THR);
}

#endif
