#include <stdarg.h>
#include <stdint.h>
#include "uart.h"

void puts(char *s)
{
	while(*s) {
		if (*s == '\n')
			uart0_putc('\r');
		uart0_putc(*s++);
	}
}

#define BUF_LEN 12
void put_uint(unsigned int value)
{
	char buf[BUF_LEN];
	char *p = &buf[BUF_LEN - 1];

	*p = '\0';

	if (!value)
		*--p = '0';

	while (value) {
		*--p = '0' + value % 10;
		value /= 10;
	}

	puts(p);
}

void put_uint_div_by_1000(unsigned int value)
{
	if (value >= 1000)
		put_uint(value / 1000);
	else
		uart0_putc('0');

	uart0_putc('.');
	put_uint(value % 1000);
}

static const char hexstr[] = "0123456789abcdef";

static void put_hex(uint32_t hex)
{
	int i;

	for (i = 0; i < 8; i++) {
		uart0_putc(hexstr[(hex & 0xf0000000) >> 28]);
		hex <<= 4;
	}
}

void printf(const char* fmt, ...)
{
	va_list ap;
	const char* p = fmt;

	va_start(ap, fmt);

	while (1) {
		if (*p == '%') {
			p++;

			switch (*p) {
				case 's':
					puts(va_arg(ap, char*));
					break;
				case 'd':
				case 'u':
					put_uint(va_arg(ap, uint32_t));
					break;
				case 'f':
					put_uint_div_by_1000(va_arg(ap, uint32_t));
					break;
				case 'x':
					put_hex(va_arg(ap, uint32_t));
					break;
				case '%':
					uart0_putc(*p);
					break;
				case '\0':
					goto end;
				default:
					uart0_putc('%');
					uart0_putc(*p);
			}
		} else if (*p == '\n') {
			uart0_putc('\r');
			uart0_putc(*p);
		} else if (*p == '\0') {
			goto end;
		} else {
			uart0_putc(*p);
		}

		p++;
	}

end:
	va_end(ap);
}
