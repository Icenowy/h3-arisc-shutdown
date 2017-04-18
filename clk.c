#include "io.h"
#include "clk.h"
#include "timer.h"
#include "debug.h"

/* ccm */
#define CCM_BASE	0x01c20000
#define R_PRCM_BASE     0x01f01400

/* clock gating */
#define BUS_CLK_GATING_REG0 		(CCM_BASE + 0x060)
#define BUS_CLK_GATING_REG1 		(CCM_BASE + 0x064)
#define MSGBOX_GATING 			BIT(21)
#define BUS_CLK_GATING_REG2 		(CCM_BASE + 0x068)
#define PIO_GATING 			BIT(5)
#define THS_GATING 			BIT(8)
#define BUS_CLK_GATING_REG3 		(CCM_BASE + 0x06c)
#define UART0_GATING 			BIT(16)
#define I2C0_GATING 			BIT(0)
#define I2C1_GATING 			BIT(1)
#define I2C2_GATING 			BIT(2)
#define BUS_CLK_GATING_REG4 		(CCM_BASE + 0x070)

/* soft reset */
#define BUS_SOFT_RST_REG0 		(CCM_BASE + 0x2c0)
#define BUS_SOFT_RST_REG1 		(CCM_BASE + 0x2c4)
#define MSGBOX_RST 			BIT(21)
#define BUS_SOFT_RST_REG2 		(CCM_BASE + 0x2c8)
#define BUS_SOFT_RST_REG3 		(CCM_BASE + 0x2d0)
#define THS_RST 			BIT(8)
#define BUS_SOFT_RST_REG4 		(CCM_BASE + 0x2d8)
#define UART0_RST 			BIT(16)
#define I2C0_RST 			BIT(0)
#define I2C1_RST 			BIT(1)
#define I2C2_RST 			BIT(2)

/* apb2 */
#define APB2_CFG_REG			(CCM_BASE + 0x058)
#define APB2_CLK_SRC_LOSC		(0x0 << 24)
#define APB2_CLK_SRC_OSC24M		(0x1 << 24)
#define APB2_CLK_SRC_PLL6		(0x2 << 24)
#define APB2_CLK_SRC_MASK		(0x3 << 24)
#define APB2_CLK_RATE_N_1		(0x0 << 16)
#define APB2_CLK_RATE_N_2		(0x1 << 16)
#define APB2_CLK_RATE_N_4		(0x2 << 16)
#define APB2_CLK_RATE_N_8		(0x3 << 16)
#define APB2_CLK_RATE_N_MASK		(3 << 16)
#define APB2_CLK_RATE_M(m)		(((m)-1) << 0)
#define APB2_CLK_RATE_M_MASK            (0x1f << 0)

/* pll6 */
#define PLL6_CTRL_REG			(CCM_BASE + 0x0028)
#define PLL6_CTRL_ENABLE		BIT(31)
#define PLL6_CTRL_LOCK			BIT(28)
#define PLL6_CTRL_BYPASS		BIT(25)
#define PLL6_CTRL_CLK_OUTEN		BIT(24)
#define PLL6_CTRL_24M_OUTEN		BIT(18)

/* ths */
#define THS_CLK_REG                     (CCM_BASE + 0x0074)
#define THS_CLK_SCLK_GATING             BIT(31)
#define THS_CLK_SRC_OSC24M              0
#define THS_CLK_DIV_RATIO_6             0x3
#define THS_CLK_DIV_RATIO_1             0x0

/* pll1 */
#define PLL_CPUX_CTRL_REG               (CCM_BASE)
#define PLL_CPUX_ENABLE                 BIT(31)
#define PLL_CPUX_LOCK                   BIT(28)
#define PLL_CPUX_P(v)                   ((v) << 16)
#define PLL_CPUX_N(v)                   ((v) << 8)
#define PLL_CPUX_K(v)                   ((v) << 4)
#define PLL_CPUX_M(v)                   ((v) << 0)

#define CPUX_AXI_CFG_REG                (CCM_BASE + 0x0050)

/*
 * AR100 clock configuration register:
 * [31:18] Reserved
 * [17:16] Clock source (00: LOSC, 01: HOSC, 10/11: PLL6/PDIV)
 * [15:13] Reserved
 * [12:8]  Post divide (00000: 1 - 11111: 32)
 * [7:6]   Reserved
 * [5:4]   Clock divide ratio (00: 1, 01: 2, 10: 4, 11: 8)
 * [3:0]   Reserved
 */
#define AR100_CLKCFG_REG		(R_PRCM_BASE + 0x000)
#define AR100_CLKCFG_SRC_LOSC		(0 << 16)
#define AR100_CLKCFG_SRC_HOSC		(1 << 16)
#define AR100_CLKCFG_SRC_PLL6		(2 << 16)
#define AR100_CLKCFG_SRC_MASK		(0x3 << 16)
#define AR100_CLKCFG_POSTDIV(x)		(((x) & 0x1f) << 8)
#define AR100_CLKCFG_POSTDIV_MASK	(0x1f << 8)
#define AR100_CLKCFG_DIV(x)		(((x) & 0x3) << 4)
#define AR100_CLKCFG_DIV_MASK		(0x3 << 4)

#define	R_PRCM_CLK_GATING_REG		(R_PRCM_BASE + 0x28) // ???
#define R_PIO_GATING 			BIT(0)

void clk_enable(uint32_t clk)
{
	if (clk & CLK_UART0) {
		readl(BUS_SOFT_RST_REG4) |= UART0_RST;
		readl(BUS_CLK_GATING_REG3) |= UART0_GATING;
	}

	if (clk & CLK_R_PIO) {
		readl(R_PRCM_CLK_GATING_REG) |= R_PIO_GATING;
	}

	if (clk & CLK_THS) {
		readl(BUS_SOFT_RST_REG3) |= THS_RST;
		readl(BUS_CLK_GATING_REG2) |= THS_GATING;
	}

	if (clk & CLK_I2C0) {
		readl(BUS_SOFT_RST_REG4) |= I2C0_RST;
		readl(BUS_CLK_GATING_REG3) |= I2C0_GATING;
	}

	if (clk & CLK_I2C1) {
		readl(BUS_SOFT_RST_REG4) |= I2C1_RST;
		readl(BUS_CLK_GATING_REG3) |= I2C1_GATING;
	}

	if (clk & CLK_I2C2) {
		readl(BUS_SOFT_RST_REG4) |= I2C2_RST;
		readl(BUS_CLK_GATING_REG3) |= I2C2_GATING;
	}

	if (clk & CLK_I2C3) {
		readl(R_PRCM_CLK_GATING_REG) |= BIT(6);
	}
}

void clk_disable(uint32_t clk)
{
	if (clk & CLK_UART0) {
		readl(BUS_SOFT_RST_REG4) &= ~UART0_RST;
		readl(BUS_CLK_GATING_REG3) &= ~UART0_GATING;
	}

	if (clk & CLK_THS) {
		readl(BUS_SOFT_RST_REG3) &= ~THS_RST;
		readl(BUS_CLK_GATING_REG2) &= ~THS_GATING;
	}
}

struct pll1_factors {
	uint8_t n;
	uint8_t k;
	uint8_t m;
	uint8_t p;
};

static void get_cpu_clk_params_calc(unsigned long freq, struct pll1_factors* factors)
{
	unsigned long best_rate = 0;
	unsigned long best_n = 0, best_k = 0, best_m = 0, best_p = 0;
	unsigned long _n, _k, _m, _p;

	for (_k = 1; _k <= 4; _k++) {
		for (_n = 1; _n <= 32; _n++) {
			for (_m = 1; _m <= 4; _m++) {
				for (_p = 0; _p <= (freq < 288000000 ? 2 : 0); _p++) {
					unsigned long tmp_rate;

					tmp_rate = 24000000 * _n * _k / (_m * (1 << _p));

					if (tmp_rate > freq)
						continue;

					if ((freq - tmp_rate) < (freq - best_rate)) {
						best_rate = tmp_rate;
						best_n = _n;
						best_k = _k;
						best_m = _m;
						best_p = _p;
					}
				}
			}
		}
	}

	factors->n = best_n - 1;
	factors->k = best_k - 1;
	factors->m = best_m - 1;
	factors->p = best_p;
}

void clk_set_rate(uint32_t clk, uint32_t rate)
{
	uint32_t reg;

	if (clk & CLK_CPUS) {
		if (rate == 32768) {
			// Switch AR100 to LOSC
			reg = readl(AR100_CLKCFG_REG);
			reg &= ~AR100_CLKCFG_SRC_MASK;
			reg |= AR100_CLKCFG_SRC_LOSC;
			writel(reg, AR100_CLKCFG_REG);
		} else if (rate == 24000000) {
			// Switch AR100 to osc24M
			reg = readl(AR100_CLKCFG_REG);
			reg &= ~AR100_CLKCFG_SRC_MASK;
			reg |= AR100_CLKCFG_SRC_HOSC;
			writel(reg, AR100_CLKCFG_REG);
		}
	} else if (clk & CLK_UART0) {
		if (rate == 24000000) {
			writel(APB2_CLK_SRC_OSC24M |
			       APB2_CLK_RATE_N_1 |
			       APB2_CLK_RATE_M(1),
			       APB2_CFG_REG);
		}
	} else if (clk & CLK_THS) {
		if (rate == 4000000) {
			writel(THS_CLK_SCLK_GATING | THS_CLK_SRC_OSC24M | THS_CLK_DIV_RATIO_6, THS_CLK_REG);
			writel(THS_CLK_SCLK_GATING | THS_CLK_SRC_OSC24M | THS_CLK_DIV_RATIO_1, THS_CLK_REG);
		}
	} else if (clk & CLK_CPUX) {
		struct pll1_factors f;

		if (rate < 60000000) {
			rate = 60000000;
		}
		
		get_cpu_clk_params_calc(rate, &f);

		uint32_t p = f.p, m = f.m, k = f.k, n = f.n;
		uint32_t real_rate = 24000000;
		real_rate *= n + 1;
		real_rate *= k + 1;
		real_rate /= 1 << p;
		real_rate /= m + 1;
		printf("N=%u K=%u M=%u P=%u   rate=%u => %u MHz\n", n, k, m, p, rate / 1000000, real_rate / 1000000);

		reg = readl(CPUX_AXI_CFG_REG);
		SET_BITS_AT(reg, 2, 16, 1);
		writel(reg, CPUX_AXI_CFG_REG);

		delay_us(300);

		reg = readl(PLL_CPUX_CTRL_REG);
		SET_BITS_AT(reg, 1, 31, 0);
		writel(reg, PLL_CPUX_CTRL_REG);
		delay_us(200);

		reg = readl(PLL_CPUX_CTRL_REG);

		SET_BITS_AT(reg, 2, 16, p);
		SET_BITS_AT(reg, 2, 0, m);
		SET_BITS_AT(reg, 5, 8, n);
		SET_BITS_AT(reg, 2, 4, k);

		writel(reg, PLL_CPUX_CTRL_REG);

		delay_us(300);

		reg = readl(PLL_CPUX_CTRL_REG);
		SET_BITS_AT(reg, 1, 31, 1);
		writel(reg, PLL_CPUX_CTRL_REG);
		delay_us(200);

		reg = readl(CPUX_AXI_CFG_REG);
		SET_BITS_AT(reg, 2, 16, 2);
		writel(reg, CPUX_AXI_CFG_REG);
	}
}

uint32_t clk_get_rate(uint32_t clk)
{
	uint32_t rate, reg;

	if (clk & CLK_CPUX) {
		reg = readl(PLL_CPUX_CTRL_REG);

		uint32_t p = GET_BITS_AT(reg, 2, 16),
			 m = GET_BITS_AT(reg, 2, 0),
			 n = GET_BITS_AT(reg, 5, 8),
                         k = GET_BITS_AT(reg, 2, 4);

		rate = 24000000;
		rate *= n + 1;
		rate *= k + 1;
		rate /= 1 << p;
		rate /= m + 1;

		return rate;
	}

	return 0;
}
