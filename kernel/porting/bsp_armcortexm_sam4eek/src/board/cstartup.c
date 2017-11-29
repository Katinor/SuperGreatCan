/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the bsp_sam3... component of the Ubinos.

  GNU General Public License Usage
  This file may be used under the terms of the GNU
  General Public License version 3.0 as published by the Free Software
  Foundation and appearing in the file license_gpl3.txt included in the
  packaging of this file. Please review the following information to
  ensure the GNU General Public License version 3.0 requirements will be
  met: http://www.gnu.org/copyleft/gpl.html.

  GNU Lesser General Public License Usage
  Alternatively, this file may be used under the terms of the GNU Lesser
  General Public License version 2.1 as published by the Free Software
  Foundation and appearing in the file license_lgpl.txt included in the
  packaging of this file. Please review the following information to
  ensure the GNU Lesser General Public License version 2.1 requirements
  will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.

  Commercial Usage
  Alternatively, licensees holding valid commercial licenses may
  use this file in accordance with the commercial license agreement
  provided with the software or, alternatively, in accordance with the
  terms contained in a written agreement between you and rightful owner.
*/

#include "../../ubiconfig.h"

#include "_bsp_board.h"

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_intr.h"
#include "itf_ubinos/itf/bsp_dtty.h"

#if defined(UBI_COMCONFIG__lib_ubik)
		#include "../../../lib_ubik_armcortexm/src/_ubik_arch.h"
#else

void ubik_irq_handler(void) {
	__asm__ __volatile__ (
		"mrs	r0, ipsr                                    \n\t"
		"bx		%0                                          \n\t"
		: : "p" (bsp_exception_handler)
	);
}

void ubik_swi_handler(void) {
	__asm__ __volatile__ (
		"tst	lr, #0x4                                    \n\t"
		"ite	eq                                          \n\t"
		"mrseq	r1, msp                                     \n\t"
		"mrsne	r1, psp                                     \n\t"
		"ldr	r0, [r1, #24]                               \n\t"
		"ldrb	r1, [r0, #-2]                               \n\t"
		"mrs	r0, ipsr                                    \n\t"
		"bx		%0                                          \n\t"
		: : "p" (bsp_exception_handler)
	);
}

void ubik_psv_handler(void) {
	__asm__ __volatile__ (
		"mrs	r0, ipsr                                    \n\t"
		"bx		%0                                          \n\t"
		: : "p" (bsp_exception_handler)
	);
}

#include "../../itf_ubinos/hdr/ubinos.h"

int mutex_create(mutex_pt * mutex_p) {
	return 0;
}

int mutex_delete(mutex_pt * mutex_p) {
	return 0;
}

int mutex_lock(mutex_pt mutex) {
	return 0;
}

int mutex_unlock(mutex_pt mutex) {
	return 0;
}

int ubik_isrt(void) {
	return 0;
}

#endif /* defined(UBI_COMCONFIG__lib_ubik) */

void _processor_reset(void) __attribute__ ((naked, section(".startup")));

isr_ft _exception_table[] __attribute__ ((section(".exc_vectors"))) = {
	/* Configure Initial Stack Pointer, using linker-generated symbols */
	(isr_ft) &_stack_top_temp,

    _processor_reset,			/* ResetException       */
    ubik_irq_handler,			/* NMI_Handler,         */
	ubik_irq_handler,			/* HardFault_Handler,   */
	ubik_irq_handler,			/* MemManage_Handler,   */
	ubik_irq_handler,			/* BusFault_Handler,    */
	ubik_irq_handler,			/* UsageFault_Handler,  */
	0, 0, 0, 0,					/* Reserved             */
	ubik_swi_handler,			/* SVC_Handler,         */
	ubik_irq_handler,			/* DebugMon_Handler,    */
	0,                  		/* Reserved             */
	ubik_psv_handler,			/* PendSV_Handler,      */
	ubik_irq_handler,			/* SysTick_Handler,     */

	/* Configurable interrupts  */
	ubik_irq_handler, 			/**<  0 SAM4E16E Supply Controller (SUPC) */
	ubik_irq_handler, 			/**<  1 SAM4E16E Reset Controller (RSTC) */
	ubik_irq_handler, 			/**<  2 SAM4E16E Real Time Clock (RTC) */
	ubik_irq_handler, 			/**<  3 SAM4E16E Real Time Timer (RTT) */
	ubik_irq_handler, 			/**<  4 SAM4E16E Watchdog/Dual Watchdog Timer (WDT) */
	ubik_irq_handler, 			/**<  5 SAM4E16E Power Management Controller (PMC) */
	ubik_irq_handler, 			/**<  6 SAM4E16E Enhanced Embedded Flash Controller (EFC) */
	ubik_irq_handler, 			/**<  7 SAM4E16E UART 0 (UART0) */
	0, 							/**<  8 SAM4E16E Reserved */
	ubik_irq_handler, 			/**<  9 SAM4E16E Parallel I/O Controller A (PIOA) */
	ubik_irq_handler, 			/**< 10 SAM4E16E Parallel I/O Controller B (PIOB) */
	ubik_irq_handler, 			/**< 11 SAM4E16E Parallel I/O Controller C (PIOC) */
	ubik_irq_handler, 			/**< 12 SAM4E16E Parallel I/O Controller D (PIOD) */
	ubik_irq_handler, 			/**< 13 SAM4E16E Parallel I/O Controller E (PIOE) */
	ubik_irq_handler, 			/**< 14 SAM4E16E USART 0 (USART0) */
	ubik_irq_handler, 			/**< 15 SAM4E16E USART 1 (USART1) */
	ubik_irq_handler, 			/**< 16 SAM4E16E Multimedia Card Interface (HSMCI) */
	ubik_irq_handler, 			/**< 17 SAM4E16E Two Wire Interface 0 (TWI0) */
	ubik_irq_handler, 			/**< 18 SAM4E16E Two Wire Interface 1 (TWI1) */
	ubik_irq_handler, 			/**< 19 SAM4E16E Serial Peripheral Interface (SPI) */
	ubik_irq_handler, 			/**< 20 SAM4E16E DMAC (DMAC) */
	ubik_irq_handler, 			/**< 21 SAM4E16E Timer/Counter 0 (TC0) */
	ubik_irq_handler, 			/**< 22 SAM4E16E Timer/Counter 1 (TC1) */
	ubik_irq_handler, 			/**< 23 SAM4E16E Timer/Counter 2 (TC2) */
	ubik_irq_handler, 			/**< 24 SAM4E16E Timer/Counter 3 (TC3) */
	ubik_irq_handler, 			/**< 25 SAM4E16E Timer/Counter 4 (TC4) */
	ubik_irq_handler, 			/**< 26 SAM4E16E Timer/Counter 5 (TC5) */
	ubik_irq_handler, 			/**< 27 SAM4E16E Timer/Counter 6 (TC6) */
	ubik_irq_handler, 			/**< 28 SAM4E16E Timer/Counter 7 (TC7) */
	ubik_irq_handler, 			/**< 29 SAM4E16E Timer/Counter 8 (TC8) */
	ubik_irq_handler, 			/**< 30 SAM4E16E Analog Front End 0 (AFEC0) */
	ubik_irq_handler, 			/**< 31 SAM4E16E Analog Front End 1 (AFEC1) */
	ubik_irq_handler, 			/**< 32 SAM4E16E Digital To Analog Converter (DACC) */
	ubik_irq_handler, 			/**< 33 SAM4E16E Analog Comparator (ACC) */
	ubik_irq_handler, 			/**< 34 SAM4E16E FPU signals : FPIXC, FPOFC, FPUFC, FPIOC, FPDZC, FPIDC, FPIXC (ARM) */
	ubik_irq_handler, 			/**< 35 SAM4E16E USB DEVICE (UDP) */
	ubik_irq_handler, 			/**< 36 SAM4E16E PWM (PWM) */
	ubik_irq_handler, 			/**< 37 SAM4E16E CAN0 (CAN0) */
	ubik_irq_handler, 			/**< 38 SAM4E16E CAN1 (CAN1) */
	ubik_irq_handler, 			/**< 39 SAM4E16E AES (AES) */
	ubik_irq_handler, 			/**< 40 SAM4E16E Reserved */
	ubik_irq_handler, 			/**< 41 SAM4E16E Reserved */
	ubik_irq_handler, 			/**< 42 SAM4E16E Reserved */
	ubik_irq_handler, 			/**< 43 SAM4E16E Reserved */
	ubik_irq_handler, 			/**< 44 SAM4E16E EMAC (GMAC) */
	ubik_irq_handler, 			/**< 45 SAM4E16E UART (UART1) */
	ubik_irq_handler, 			/**< Number of peripheral IDs */
};

void _processor_reset(void) {
	unsigned int value;

	/* Disable interrupt */
	ARM_INTERRUPT_DISABLE();

	/* Disable peripheral clock */
	PMC->PMC_WPMR  = 0x504D4300;
	PMC->PMC_PCDR0 = 0xFFFFFFFF;
	PMC->PMC_PCDR1 = 0xFFFFFFFF;
	ARM_DATASYNC();

	/* Set stack pointer to temporal stack top */
	__asm__ __volatile__ (
		"mov	sp, %0												\n\t"
		"sub	sp, sp, #32											\n\t"	// for local variable
		: : "p" (&_stack_top_temp)
	);

	/* Initiate hardwares */
	lowlevel_init();

	/* Set stack pointer to stack top */
	__asm__ __volatile__ (
		"mov	sp, %0												\n\t"
		"sub	sp, sp, #32											\n\t"	// for local variable
		: : "p" (&_stack_top)
	);

	/* set control register to use msp and privilege in thread mode */
	__asm__ __volatile__ (
		"push		{r0}											\n\t"
		"mrs		r0, control										\n\t"
		"and		r0, r0, #0xFFFFFFFC								\n\t"
		"msr		control, r0										\n\t"
		"pop		{r0}											\n\t"
	);

	/* Set interrupt priority, endian, group, ... */
	value  = SCB->AIRCR;
	value &= ~((0xFFFF << 16) | (0x01 << 15) | (0x07 << 8));
	value |=  ((0x05FA << 16) | (0x00 << 15) | (0x07 << 8) | (0x01 << 1));
	SCB->AIRCR = value;
	ARM_DATASYNC();

	SCB->SHP[ 4 - 4] 		= ((0x00                    ) & 0xFF);	/* MemManageFault */
	SCB->SHP[ 5 - 4] 		= ((0x00                    ) & 0xFF);	/* BusFault */
	SCB->SHP[ 6 - 4] 		= ((0x00                    ) & 0xFF);	/* UseFault */
	SCB->SHP[11 - 4] 		= ((NVIC_PRIO_HIGHEST + 0x00) & 0xFF);	/* SVCall */
	SCB->SHP[12 - 4] 		= ((0x00                    ) & 0xFF);	/* DebugMon */
	SCB->SHP[14 - 4] 		= ((0x00                    ) & 0xFF);	/* PendSV: Must be the highest priority */
	SCB->SHP[15 - 4] 		= ((NVIC_PRIO_HIGHEST + 0x20) & 0xFF);	/* SysTick */
	for (value = 0; value < (EXCEPTION_TYPE_IRQ_END - EXCEPTION_TYPE_IRQ_START + 1); value++) {
	    NVIC->IP[value] 	= ((NVIC_PRIO_MIDDLE  + 0x00) & 0xFF);
	}
	ARM_DATASYNC();

#if (1 == UBI_SOCHECK)
	/* save current (svc) stack limit at _svc_stacklimit */
	value = (unsigned int) &_stack_top - (SVC_STACK_SIZE - SOCHECK_MARGIN - ARM_CONTEXT_SIZE);
	_svc_stacklimit = (unsigned int *) value;
#if (2 != UBI_SOCHECK_TYPE)
	/* save current (svc) stack limit at _socheck_stacklimit */
	_socheck_stacklimit	= (unsigned int *) value;
#else
	/* save current (svc) stack limit at r10 */
	__asm__ __volatile__ (
		"mov	r10, %0                                      \n\t"
		: : "r" (value)
	);
#endif
#endif
#if (1 == UBI_SUCHECK)
	/* save current (svc) stack top max at _svc_stacktop_max */
	_svc_stacktop_max		= &_stack_top;
	/* save current (svc) stack top max at _sucheck_stacktop_max */
	_sucheck_stacktop_max	= &_stack_top;
#endif

	/* ASPEN = 0 , LSPEN = 0
	 * none of the interrupt or exception handlers use the FPU
	 * no lazy stacking
	 * */
	FPU->FPCCR &= 0x3FFFFFFF;

	/* Branch to main function */
	main();

	/* Infinite loop */
	for (;;);
}

