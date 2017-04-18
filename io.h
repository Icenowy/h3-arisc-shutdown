#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>

#define readl(addr)         	(*((volatile uint32_t *)(addr)))
#define writel(v, addr)	    	(readl(addr) = (uint32_t)(v))
#define set_bit(nr, addr)   	(readl(addr) |=  (1u << (nr)))
#define clr_bit(nr, addr)   	(readl(addr) &= ~(1u << (nr)))

#define BIT(nr) 	    	(1u << (nr))
#define MASK(nr) 		((1u << (nr)) - 1)
#define MASK_AT(nr, start) 	(MASK(nr) << (start))

#define GET_BITS_AT(value, nr, start) (((value) >> start) & MASK(nr))
#define SET_BITS_AT(value, nr, start, bits) ((value) &= ~MASK_AT(nr, start), (value) |= ((bits) & MASK(nr)) << start)

#endif
