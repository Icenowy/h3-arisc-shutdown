#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "uart.h"

#ifndef DEBUG

#define puts(v)
#define printf(v...)

#else

void puts(char *str);
void printf(const char* fmt, ...);

#endif

#endif
