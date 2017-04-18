#include <or1k-support.h>
#include <or1k-sprs.h>
#include "io.h"
#include "gpio.h"
#include "debug.h"
#include "clk.h"
#include "timer.h"
#include "fwcfg.h"
#include "cpux.h"

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

#ifdef DEBUG
void dump_config()
{
	printf("Firmware config:\n");
	printf("==============================================\n");
	if (fwcfg.button_gpio_pl != -1)
		printf("Button GPIO: PL%d\n", fwcfg.button_gpio_pl);
	if (fwcfg.led_gpio_pl != -1)
		printf("LED GPIO: PL%d\n", fwcfg.led_gpio_pl);
	if (fwcfg.button_press_time != -1)
		printf("Button press timeout is %ds\n",
		       fwcfg.button_press_time);
	if (fwcfg.button_pressed_voltage != -1)
		printf("When the button is pressed the GPIO status is %s\n",
		       fwcfg.button_pressed_voltage ? "high" : "low");
	printf("==============================================\n");
}
#else
inline void dump_config {}
#endif

int main(void)
{
	int button_pressed_count = 0;
	unsigned int val;
	enable_caches();
	gpio_init();
#ifdef DEBUG
	uart0_init();
#endif
	clk_set_rate(CLK_CPUS, 24000000);

	puts("\nOpenRISC FW 1.0\n");

	printf("Shutting down CPUX cores: ");
	for (int i = 0; i < 4; i++) {
		cpux_shutdown(i);
		printf("%d ", i);
	};
	printf("\n");

	dump_config();

	if (fwcfg.button_gpio_pl != -1)
		gpio_set_pincfg(GPIO_BANK_L, fwcfg.button_gpio_pl,
				GPIO_FUNC_INPUT);

	while (1) {
		delay_us(10000); /* 10ms */

		if (fwcfg.button_gpio_pl != -1 &&
		    fwcfg.button_pressed_voltage != -1) {
			val = readl(gpio_get_data_addr(GPIO_BANK_L));
			if (fwcfg.button_pressed_voltage) {
				if (val & (1 << fwcfg.button_gpio_pl))
					button_pressed_count++;
				else
					button_pressed_count = 0;
			} else {
				if (val & (1 << fwcfg.button_gpio_pl))
					button_pressed_count = 0;
				else
					button_pressed_count++;
			}
		}

		if (fwcfg.button_press_time != -1 &&
		    button_pressed_count > (fwcfg.button_press_time * 100))
		{
			printf("Perfoming system reset.\n");

			if (fwcfg.led_gpio_pl != -1) {
				gpio_set_pincfg(GPIO_BANK_L, fwcfg.led_gpio_pl,
						GPIO_FUNC_OUTPUT);
				set_bit(fwcfg.led_gpio_pl,
					gpio_get_data_addr(GPIO_BANK_L));
			}

			soc_reset();
			delay_us(1000000); /* 1s, the system should reset in 0.5s */
			printf("SoC not reseted.\n");
		}
	}
}
