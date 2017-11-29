/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the bsp_sam... component of the Ubinos.

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

#include <stdio.h>

#if ( defined(__thumb__) && (UBI_CPU_TYPE__CORTEX_M3 != UBI_CPU_TYPE && UBI_CPU_TYPE__CORTEX_M4 != UBI_CPU_TYPE) )
	#error "This file has to be compiled with ARM operating state option, '-marm'."
#endif

#if ( (defined(__SOCHECK__) && !defined(__NOSOCHECK__)) || (defined(__SUCHECK__) && !defined(__NOSUCHECK__)) )
	#error "This file has to be compiled without stack overflow check and stack usage check option."
#endif

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_intr.h"
#include "itf_ubinos/itf/bsp_dtty.h"

#include "_bsp_board.h"

#if (1 == UBI_SOCHECK)
	#if (2 != UBI_SOCHECK_TYPE)
				#define _bsp_getstacklimit()	((unsigned int *) _socheck_stacklimit)

				#define _bsp_setstacklimit(stacklimit) {							\
					_socheck_stacklimit = stacklimit;								\
				}

				#define _bsp_checkstacklimit(stacklimit)
	#else
				static inline unsigned int * _bsp_getstacklimit() {
					 unsigned int * stacklimit;
					__asm__ __volatile__ (
							"mov		%0, r10" : "=r" (stacklimit));
					return stacklimit;
				}

				static inline void _bsp_setstacklimit(volatile unsigned int * stacklimit) {
					__asm__ __volatile__ (
							"mov		r10, %0" : : "r" (stacklimit)
					);
				}

				static inline int _bsp_checkstacklimit(volatile unsigned int * stacklimit) {
					unsigned int * limit;

					limit = _bsp_getstacklimit();

					if (limit != stacklimit) {
		#if defined(UBI_COMCONFIG__lib_ubik)
						dtty_puts_nosocheck("\r\npollution of stack limit register is detected", 80);

						if (0 != _ubik_intrcount) {
							dtty_puts_nosocheck(" at isr (svc stack)\r\n", 80);
						}
						else if (0 != _ubik_active) {
							dtty_puts_nosocheck(" at task \"", 80);
							dtty_puts_nosocheck(_task_cur->name, 80);
							dtty_puts_nosocheck("\"\r\n", 80);
						}
						else {
							dtty_puts_nosocheck(" at system init code (svc stack)\r\n", 80);
						}
		#else
						dtty_puts_nosocheck("\r\npollution of stack limit register is detected\r\n", 80);
		#endif
						bsp_abortsystem();

						return -1;
					}

					return 0;
				}
	#endif

#else

				#define _bsp_getstacklimit()

				#define _bsp_setstacklimit(stacklimit)

				#define _bsp_checkstacklimit(stacklimit)

#endif

#if defined(UBI_COMCONFIG__lib_ubik)

				void _bsp_initfirsttask(void) {
					_bsp_checkstacklimit(_svc_stacklimit);
					_bsp_setstacklimit(_task_cur->stacklimit);
					_bsp_restore_stacktop_max();
				}

#else

				void _bsp_initfirsttask(void) {
				}


				#define ubik_task_schedule_irq()

				#define ubik_swisr(swino)

				#define ubik_tickisr()

#endif

void bsp_exception_handler(unsigned int exceptiontype, unsigned int swino) {
	isr_ft isr = 0;

	_exception_increase_intrcount();

	if (EXCEPTION_TYPE_IRQ_END < exceptiontype) {
		dtty_puts_nosocheck("\r\n\r\nunknown exception (", 80);
		switch (exceptiontype) {
		default:
			dtty_putc_nosocheck((int) (exceptiontype + '0'));
			break;
		}
		dtty_puts_nosocheck(")\r\n", 80);
		bsp_abortsystem();
	}
	else if (EXCEPTION_TYPE_IRQ_START <= exceptiontype) {
		_exception_check_segmentendboundary();
		_exception_check_ubik_critcount();
		_exception_preprocess();
		isr = _irq_handler_table[exceptiontype - EXCEPTION_TYPE_IRQ_START];
		if (isr != (isr_ft) 0x0) {
			(*isr)();
		}
		else {
			dtty_puts_nosocheck("\r\n\r\nunknown exception (", 80);
			switch (exceptiontype) {
			default:
				dtty_putc_nosocheck((int) (exceptiontype + '0'));
				break;
			}
			dtty_puts_nosocheck(")\r\n", 80);
			bsp_abortsystem();
		}
		ubik_task_schedule_irq();
		_exception_postprocess();
		_exception_check_segmentendboundary();
	}
	else {
		switch (exceptiontype) {

		case EXCEPTION_TYPE_SYSTICK_HANDLER:
			_exception_check_segmentendboundary();
			_exception_check_ubik_critcount();
			_exception_preprocess();
			ubik_tickisr();
			_exception_postprocess();
			_exception_check_segmentendboundary();
			break;

		case EXCEPTION_TYPE_SVC_HANDLER:
			_exception_check_segmentendboundary();
			_exception_preprocess();
			ubik_swisr(swino);
			_exception_postprocess();
			_exception_check_segmentendboundary();
			break;

		case EXCEPTION_TYPE_PENDSV_HANDLER:
			_exception_check_segmentendboundary();
			_exception_preprocess();
			ubik_swisr(SWINO__TASK_YIELD);
			_exception_postprocess();
			_exception_check_segmentendboundary();
			break;

		case EXCEPTION_TYPE_NMI_HANDLER:
		case EXCEPTION_TYPE_HARDFAULT_HANDLER:
		case EXCEPTION_TYPE_MEMMANAGE_HANDLER:
		case EXCEPTION_TYPE_BUSFAULT_HANDLER:
		case EXCEPTION_TYPE_USAGEFAULT_HANDLER:
		case EXCEPTION_TYPE_DEBUGMON_HANDLER:
		default:
			dtty_puts_nosocheck("\r\n\r\nunknown exception (", 80);
			switch (exceptiontype) {
			default:
				dtty_putc_nosocheck((int) (exceptiontype + '0'));
				break;
			}
			dtty_puts_nosocheck(")\r\n", 80);
			bsp_abortsystem();
			break;
		}
	}

	_exception_decrease_intrcount();
}

isr_ft _irq_handler_table[EXCEPTION_TYPE_IRQ_END - EXCEPTION_TYPE_IRQ_START + 1] = {
		0, /**<  0 SAM4E16E Supply Controller (SUPC) */
		0, /**<  1 SAM4E16E Reset Controller (RSTC) */
		0, /**<  2 SAM4E16E Real Time Clock (RTC) */
		0, /**<  3 SAM4E16E Real Time Timer (RTT) */
		0, /**<  4 SAM4E16E Watchdog/Dual Watchdog Timer (WDT) */
		0, /**<  5 SAM4E16E Power Management Controller (PMC) */
		0, /**<  6 SAM4E16E Enhanced Embedded Flash Controller (EFC) */
		0, /**<  7 SAM4E16E UART 0 (UART0) */
		0, /**<  8 SAM4E16E Reserved */
		0, /**<  9 SAM4E16E Parallel I/O Controller A (PIOA) */
		0, /**< 10 SAM4E16E Parallel I/O Controller B (PIOB) */
		0, /**< 11 SAM4E16E Parallel I/O Controller C (PIOC) */
		0, /**< 12 SAM4E16E Parallel I/O Controller D (PIOD) */
		0, /**< 13 SAM4E16E Parallel I/O Controller E (PIOE) */
		0, /**< 14 SAM4E16E USART 0 (USART0) */
		0, /**< 15 SAM4E16E USART 1 (USART1) */
		0, /**< 16 SAM4E16E Multimedia Card Interface (HSMCI) */
		0, /**< 17 SAM4E16E Two Wire Interface 0 (TWI0) */
		0, /**< 18 SAM4E16E Two Wire Interface 1 (TWI1) */
		0, /**< 19 SAM4E16E Serial Peripheral Interface (SPI) */
		0, /**< 20 SAM4E16E DMAC (DMAC) */
		0, /**< 21 SAM4E16E Timer/Counter 0 (TC0) */
		0, /**< 22 SAM4E16E Timer/Counter 1 (TC1) */
		0, /**< 23 SAM4E16E Timer/Counter 2 (TC2) */
		0, /**< 24 SAM4E16E Timer/Counter 3 (TC3) */
		0, /**< 25 SAM4E16E Timer/Counter 4 (TC4) */
		0, /**< 26 SAM4E16E Timer/Counter 5 (TC5) */
		0, /**< 27 SAM4E16E Timer/Counter 6 (TC6) */
		0, /**< 28 SAM4E16E Timer/Counter 7 (TC7) */
		0, /**< 29 SAM4E16E Timer/Counter 8 (TC8) */
		0, /**< 30 SAM4E16E Analog Front End 0 (AFEC0) */
		0, /**< 31 SAM4E16E Analog Front End 1 (AFEC1) */
		0, /**< 32 SAM4E16E Digital To Analog Converter (DACC) */
		0, /**< 33 SAM4E16E Analog Comparator (ACC) */
		0, /**< 34 SAM4E16E FPU signals : FPIXC, FPOFC, FPUFC, FPIOC, FPDZC, FPIDC, FPIXC (ARM) */
		0, /**< 35 SAM4E16E USB DEVICE (UDP) */
		0, /**< 36 SAM4E16E PWM (PWM) */
		0, /**< 37 SAM4E16E CAN0 (CAN0) */
		0, /**< 38 SAM4E16E CAN1 (CAN1) */
		0, /**< 39 SAM4E16E AES (AES) */
		0, /**< 40 SAM4E16E Reserved */
		0, /**< 41 SAM4E16E Reserved */
		0, /**< 42 SAM4E16E Reserved */
		0, /**< 43 SAM4E16E Reserved */
		0, /**< 44 SAM4E16E EMAC (GMAC) */
		0, /**< 45 SAM4E16E UART (UART1) */
		0, /**< 46 SAM4E16E PERIPH_COUNT Number of peripheral IDs */
};

