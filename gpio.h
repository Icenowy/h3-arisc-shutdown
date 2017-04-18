#ifndef __GPIO_H__
#define __GPIO_H__

#include "io.h"

#define GPIO_BANK_A    0
#define GPIO_BANK_C    2
#define GPIO_BANK_D    3
#define GPIO_BANK_E    4
#define GPIO_BANK_F    5
#define GPIO_BANK_G    6
#define GPIO_BANK_L    11

#define GPIO_FUNC_INPUT         0
#define GPIO_FUNC_OUTPUT        1
#define GPIO_FUNC_BANK_A_UART0	2
#define GPIO_FUNC_BANK_A_I2C0	2
#define GPIO_FUNC_BANK_A_I2C1	3
#define GPIO_FUNC_BANK_E_I2C2	3
#define GPIO_FUNC_BANK_L_I2C3	2

void gpio_init(void);
void gpio_set_pincfg(uint32_t bank, uint32_t pin, uint32_t val);
uint32_t gpio_get_pincfg(uint32_t bank, uint32_t pin);
uint32_t gpio_get_data_addr(uint32_t bank);

#endif
