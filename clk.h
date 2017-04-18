#ifndef __CLK_H__
#define __CLK_H__

#define CLK_UART0 	BIT(0)
#define CLK_CPUS 	BIT(1)
#define CLK_R_PIO 	BIT(2)
#define CLK_THS 	BIT(3)
#define CLK_CPUX 	BIT(4)
#define CLK_MSGBOX 	BIT(6)
#define CLK_I2C0 	BIT(7)
#define CLK_I2C1 	BIT(8)
#define CLK_I2C2 	BIT(9)
#define CLK_I2C3 	BIT(10)

void clk_enable(uint32_t clk);
void clk_disable(uint32_t clk);
void clk_set_rate(uint32_t clk, uint32_t rate);
uint32_t clk_get_rate(uint32_t clk);

#endif
