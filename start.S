/*
 * (C) Copyright 2013 Stefan Kristiansson <stefan.kristiansson@saunalahti.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <or1k-sprs.h>

#define CLEAR_GPR(gpr) \
        l.or    gpr,r0,r0

#define LOAD_LONG_SYMBOL_TO_GPR(gpr, symbol) \
        l.movhi gpr,hi(symbol) 		;\
        l.ori   gpr,gpr,lo(symbol)

#define LOAD_SHORT_SYMBOL_TO_GPR(gpr, symbol) \
        l.ori   gpr,r0,lo(symbol)

// 
// The H3 SoC has address decode logic for the exception vectors,
// leaving only room for a jump instruction at each exception vector,
// so we can't do the setup here.
// 

#define EXCEPTION_ENTRY_POINT(offset, handler) \
	.org offset 	;\
	l.j handler 	;\
	l.nop

	EXCEPTION_ENTRY_POINT(0x100, reset_handler)
	EXCEPTION_ENTRY_POINT(0x200, bus_error_handler)
	EXCEPTION_ENTRY_POINT(0x300, data_page_fault_handler)
	EXCEPTION_ENTRY_POINT(0x400, insn_page_fault_handler)
	EXCEPTION_ENTRY_POINT(0x500, tick_timer_handler)
	EXCEPTION_ENTRY_POINT(0x600, alignment_handler)
	EXCEPTION_ENTRY_POINT(0x700, illegal_insn_handler)
	EXCEPTION_ENTRY_POINT(0x800, ext_interrupt_handler)
	EXCEPTION_ENTRY_POINT(0x900, dtlb_miss_handler)
	EXCEPTION_ENTRY_POINT(0xa00, itlb_miss_handler)
	EXCEPTION_ENTRY_POINT(0xb00, range_handler)
	EXCEPTION_ENTRY_POINT(0xc00, system_call_handler)
	EXCEPTION_ENTRY_POINT(0xd00, floating_point_handler)
	EXCEPTION_ENTRY_POINT(0xe00, trap_handler)

//
// ARM CPU "boot loader" that resets the OpenRISC core in H3
//

	.org 0x4000
	.align 4

// 
// Exception handlers
// 

#define EXCEPTION_HANDLER(exception_number) \
	l.addi  r1,r1,-256 			;\
	l.sw    4(r1),r3 			;\
	l.addi  r3,r0,exception_number 		;\
	l.j     shared_exception_handler 	;\
        l.sw 	0(r1),r2

bus_error_handler:
	EXCEPTION_HANDLER(2)

tick_timer_handler:
	EXCEPTION_HANDLER(5)

alignment_handler:
	EXCEPTION_HANDLER(6)

illegal_insn_handler:
	EXCEPTION_HANDLER(7)

ext_interrupt_handler:
	EXCEPTION_HANDLER(8)

range_handler:
	EXCEPTION_HANDLER(0xb)

system_call_handler:
	EXCEPTION_HANDLER(0xc)

trap_handler:
	EXCEPTION_HANDLER(0xe)

// exceptions we don't need to differentiate
data_page_fault_handler:
insn_page_fault_handler:
dtlb_miss_handler:
itlb_miss_handler:
floating_point_handler:
	EXCEPTION_HANDLER(0)

shared_exception_handler:
        l.sw 	8(r1),r4
        l.sw 	12(r1),r5
        l.sw 	16(r1),r6
        l.sw 	20(r1),r7
        l.sw 	24(r1),r8
        l.sw 	28(r1),r9
        l.sw 	32(r1),r10
        l.sw 	36(r1),r11
        l.sw 	40(r1),r12
        l.sw 	44(r1),r13
        l.sw 	48(r1),r14
        l.sw 	52(r1),r15
        l.sw 	56(r1),r16
        l.sw 	60(r1),r17
        l.sw 	64(r1),r18
        l.sw 	68(r1),r19
        l.sw 	72(r1),r20
        l.sw 	76(r1),r21
        l.sw 	80(r1),r22
        l.sw 	84(r1),r23
        l.sw 	88(r1),r24
        l.sw 	92(r1),r25
        l.sw 	96(r1),r26
        l.sw 	100(r1),r27
        l.sw 	104(r1),r28
        l.sw 	108(r1),r29
        l.sw 	112(r1),r30
        l.sw 	116(r1),r31

	// call into C to handle exception
	l.mfspr r4,r0,OR1K_SPR_SYS_EPCR_ADDR(0)
	l.addi 	r5,r1,256
	l.jal   handle_exception
	l.nop

	l.lwz 	r2,0(r1)    
	l.lwz 	r3,4(r1)    
	l.lwz 	r4,8(r1)    
	l.lwz 	r5,12(r1)   
	l.lwz 	r6,16(r1)   
	l.lwz 	r7,20(r1)   
	l.lwz 	r8,24(r1)   
	l.lwz 	r9,28(r1)   
	l.lwz 	r10,32(r1)  
	l.lwz 	r11,36(r1)  
	l.lwz 	r12,40(r1)  
	l.lwz 	r13,44(r1)  
	l.lwz 	r14,48(r1)  
	l.lwz 	r15,52(r1)  
	l.lwz 	r16,56(r1)  
	l.lwz 	r17,60(r1)  
	l.lwz 	r18,64(r1)  
	l.lwz 	r19,68(r1)  
	l.lwz 	r20,72(r1)  
	l.lwz 	r21,76(r1)  
	l.lwz 	r22,80(r1)  
	l.lwz 	r23,84(r1)  
	l.lwz 	r24,88(r1)  
	l.lwz 	r25,92(r1)  
	l.lwz 	r26,96(r1)  
	l.lwz 	r27,100(r1) 
	l.lwz 	r28,104(r1) 
	l.lwz 	r29,108(r1) 
	l.lwz 	r30,112(r1) 
	l.lwz 	r31,116(r1) 
	l.addi 	r1,r1,256  
	l.rfe             
	l.nop

//
// _start entry point
//

	.global _start
_start:
reset_handler:
	l.ori   r3,r0,OR1K_SPR_SYS_SR_SM_MASK | OR1K_SPR_SYS_SR_LEE_MASK
	l.mtspr r3,r0,OR1K_SPR_SYS_SR_ADDR
	l.mtspr r0,r0,OR1K_SPR_TICK_TTMR_ADDR

	CLEAR_GPR(r1)
	CLEAR_GPR(r2)
	CLEAR_GPR(r3)
	CLEAR_GPR(r4)
	CLEAR_GPR(r5)
	CLEAR_GPR(r6)
	CLEAR_GPR(r7)
	CLEAR_GPR(r8)
	CLEAR_GPR(r9)
	CLEAR_GPR(r10)
	CLEAR_GPR(r11)
	CLEAR_GPR(r12)
	CLEAR_GPR(r13)
	CLEAR_GPR(r14)
	CLEAR_GPR(r15)
	CLEAR_GPR(r16)
	CLEAR_GPR(r17)
	CLEAR_GPR(r18)
	CLEAR_GPR(r19)
	CLEAR_GPR(r20)
	CLEAR_GPR(r21)
	CLEAR_GPR(r22)
	CLEAR_GPR(r23)
	CLEAR_GPR(r24)
	CLEAR_GPR(r26)
	CLEAR_GPR(r27)
	CLEAR_GPR(r28)
	CLEAR_GPR(r29)
	CLEAR_GPR(r30)
	CLEAR_GPR(r31)

clear_bss:
	LOAD_SHORT_SYMBOL_TO_GPR(r6, __bss_start)
	LOAD_SHORT_SYMBOL_TO_GPR(r7, __bss_end)
1:
	l.sw    (0)(r6),r0
	l.sfltu r6,r7
	l.bf    1b
	l.addi  r6,r6,4

	// set stack top
	l.ori	r1,r0,0xbffc

	// start main
	l.j	main
	l.nop
