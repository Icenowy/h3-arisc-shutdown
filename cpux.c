#include "cpux.h"
#include "io.h"

#define CPUCFG_RST_CPU(cpu) (0x01f01c40 + 0x40 * (cpu))
#define CPUCFG_DBG_CTRL1 0x01f01de4
#define PRCM_CPU_PWROFF 0x01f01500
#define PRCM_CPU_PWR_CLAMP(cpu) (0x01f01540 + 0x4 * (cpu))

void cpux_shutdown(unsigned int cpu)
{
	writel(0, CPUCFG_RST_CPU(cpu));

	clr_bit(cpu, CPUCFG_DBG_CTRL1);

	set_bit(cpu, PRCM_CPU_PWROFF);
	writel(0xff, PRCM_CPU_PWR_CLAMP(cpu));

	set_bit(cpu, CPUCFG_DBG_CTRL1);
}
