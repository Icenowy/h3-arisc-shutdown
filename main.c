#include <or1k-support.h>
#include <or1k-sprs.h>
#include "io.h"
#include "gpio.h"
#include "debug.h"
#include "clk.h"
#include "timer.h"

void enable_caches(void)
{
	unsigned addr;

	for (addr = 0; addr < 16 * 1024 + 32 * 1024; addr += 16)
		or1k_icache_flush(addr);

	or1k_icache_enable();
}

void soc_reset(void)
{
	writel(1, 0x01c20cb4);
	writel(1, 0x01c20cb8);
	writel((0xa57 << 1) | 0x01, 0x01c20cb0);
}

void arisc_reset(void)
{
	asm("l.j _start");
	asm("l.nop");
}

void handle_exception(uint32_t type, uint32_t pc, uint32_t sp)
{
	if (type == 8) {
		printf("interrupt\n");
	} else if (type == 5) {
		printf("timer\n");
	} else {
		printf("exception %u at pc=%x sp=%x\nrestarting...\n\n", type, pc, sp);
		arisc_reset();
	}
}

int main(void)
{
	int button_pressed_count = 0;
	enable_caches();
	gpio_init();
	uart0_init();
	clk_set_rate(CLK_CPUS, 24000000);

	puts("\nOpenRISC FW 1.0\n");

	gpio_set_pincfg(GPIO_BANK_L, 3, GPIO_FUNC_INPUT);

	while (1) {
		delay_us(100000); // 100ms

		if (!(readl(gpio_get_data_addr(GPIO_BANK_L)) & BIT(3))) {
			button_pressed_count++;
		} else {
			button_pressed_count = 0;
		}

		if (button_pressed_count > 30)
		{
			printf("Perfoming system reset.\n");
			gpio_set_pincfg(GPIO_BANK_L, 10, GPIO_FUNC_OUTPUT);
			set_bit(10, gpio_get_data_addr(GPIO_BANK_L));
			soc_reset();
			delay_us(1000000);
			printf("SoC not reseted.\n");
		}
	}
}
