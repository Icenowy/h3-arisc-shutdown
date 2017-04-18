#ifndef __UART_H__
#define __UART_H__

#ifndef DEBUG

#define uart0_init()
#define uart0_putc(v)

#else

void uart0_init(void);
void uart0_putc(char c);

#endif

#endif
