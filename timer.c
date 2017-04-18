#include <or1k-sprs.h>
#include <or1k-support.h>
#include "timer.h"

void delay_ticks(uint32_t ticks)
{
	or1k_mtspr(OR1K_SPR_TICK_TTCR_ADDR, 0);
	or1k_mtspr(OR1K_SPR_TICK_TTMR_ADDR, OR1K_SPR_TICK_TTMR_MODE_SET(0, OR1K_SPR_TICK_TTMR_MODE_CONTINUE));

	while (or1k_mfspr(OR1K_SPR_TICK_TTCR_ADDR) < ticks);
}

void timer_start(void)
{
	or1k_mtspr(OR1K_SPR_TICK_TTMR_ADDR, OR1K_SPR_TICK_TTMR_MODE_SET(0, OR1K_SPR_TICK_TTMR_MODE_CONTINUE));
	or1k_mtspr(OR1K_SPR_TICK_TTCR_ADDR, 0);
}

uint32_t timer_stop(void)
{
	uint32_t val = or1k_mfspr(OR1K_SPR_TICK_TTCR_ADDR);

	or1k_mtspr(OR1K_SPR_TICK_TTMR_ADDR, 0);

	return val;
}
