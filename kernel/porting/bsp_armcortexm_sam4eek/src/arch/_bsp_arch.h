/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the bsp_arm... component of the Ubinos.

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

#ifndef _BSP_ARCH_H_
#define _BSP_ARCH_H_

#include "../../ubiconfig.h"

#include "../common/_bsp.h"

#define ARM_MODE_USR						0x11
#define ARM_MODE_SYS						0x10
#define ARM_MODE_HDR						0x00

#define EXCEPTION_TYPE_NMI_HANDLER			   2
#define EXCEPTION_TYPE_HARDFAULT_HANDLER	   3
#define EXCEPTION_TYPE_MEMMANAGE_HANDLER	   4
#define EXCEPTION_TYPE_BUSFAULT_HANDLER		   5
#define EXCEPTION_TYPE_USAGEFAULT_HANDLER	   6
#define EXCEPTION_TYPE_SVC_HANDLER			  11
#define EXCEPTION_TYPE_DEBUGMON_HANDLER		  12
#define EXCEPTION_TYPE_PENDSV_HANDLER		  14
#define EXCEPTION_TYPE_SYSTICK_HANDLER		  15
#define EXCEPTION_TYPE_IRQ_START			  16
#define EXCEPTION_TYPE_IRQ_END				  63

#define SWINO__TASK_YIELD					0x01

#define NVIC_PRIO_HIGHEST					0x20
#define NVIC_PRIO_MIDDLE					0x80
#define NVIC_PRIO_LOWEST					0xE0

#define ARM_INTERRUPT_ENABLE() {												\
	__asm__ __volatile__ (														\
		"cpsie	i														\n\t"	\
		"isb				 											\n\t"	\
	);																			\
}

#define ARM_INTERRUPT_DISABLE() {												\
	__asm__ __volatile__ (														\
		"cpsid	i														\n\t"	\
		"isb				 											\n\t"	\
	);																			\
}

#define ARM_DATASYNC() {														\
	__asm__ __volatile__ (														\
		"dsb				 											\n\t"	\
	);																			\
}

#define ARM_CONTEXT_SIZE				(17*4)		/* _ubik_critcount, r4~r11, r0~r3, r12, lr, pc, psr */

#ifndef __ASSEMBLY__
	extern unsigned int _stack_top      __asm__ ("__stack_top__");      /* Defined by the linker.  */
	extern unsigned int _stack_top_temp	__asm__ ("__stack_top_temp__"); /* Defined by the linker.  */
	extern unsigned int _end            __asm__ ("__end__");            /* Defined by the linker.  */

	unsigned int arm_get_cp15(void);
	void arm_set_cp15(unsigned int value);
	unsigned int arm_get_xpsr(void);
	unsigned int arm_get_apsr(void);
	unsigned int arm_get_ipsr(void);
	unsigned int arm_get_epsr(void);

	void arm_set_pendsv(void);
	unsigned int arm_get_pendsv(void);
	void arm_set_svcpend(void);
	unsigned int arm_get_svcpend(void);

	unsigned int arm_get_cp15_nosocheck(void);
	void arm_set_cp15_nosocheck(unsigned int value);
	unsigned int arm_get_cpsr_nosocheck(void);

			extern volatile unsigned int * _svc_stack;

	#if (1 == UBI_SOCHECK)
			extern volatile unsigned int * _svc_stacklimit;
		#if (2 != UBI_SOCHECK_TYPE)
			extern volatile unsigned int * _socheck_stacklimit;
		#endif
	#endif

	#if (1 == UBI_SUCHECK)
			extern volatile unsigned int * _svc_stacktop_max;
			extern volatile unsigned int * _sucheck_stacktop_max;
	#endif


	#if (1 != EXCLUDE_SEGMENT_END_BOUNDARY_CHECK)
			#define SEGMENT_END_BOUNDARY_VALUE	0xA5B4

			extern unsigned int _data_segment_end_boundary 		__asm__ ("__data_end_boundary__");	/* Defined by the linker.  */
			extern unsigned int _bss_segment_end_boundary 		__asm__ ("__bss_end_boundary__");	/* Defined by the linker.  */
			extern unsigned int _common_segment_end_boundary 	__asm__ ("__common_end_boundary__");	/* Defined by the linker.  */
	#endif

#if (1 != EXCLUDE_SEGMENT_END_BOUNDARY_CHECK)
	#define _exception_check_segmentendboundary() {											\
		if (SEGMENT_END_BOUNDARY_VALUE != _data_segment_end_boundary) {						\
			dtty_puts_nosocheck("\r\ndata segment was polluted\r\n", 80);		\
			bsp_abortsystem();																\
		}																					\
		if (SEGMENT_END_BOUNDARY_VALUE != _bss_segment_end_boundary) {						\
			dtty_puts_nosocheck("\r\nbss segment was polluted\r\n", 80);		\
			bsp_abortsystem();																\
		}																					\
		if (SEGMENT_END_BOUNDARY_VALUE != _common_segment_end_boundary) {					\
			dtty_puts_nosocheck("\r\ncommon segment was polluted\r\n", 80);	\
			bsp_abortsystem();																\
		}																					\
	}
#else
	#define _exception_check_segmentendboundary()
#endif

#if (1 == UBI_SUCHECK)
				#define _bsp_save_stacktop_max() {										\
					_task_cur->stacktop_max = (unsigned int *) _sucheck_stacktop_max;	\
					_sucheck_stacktop_max = _svc_stacktop_max;							\
				}

				#define _bsp_restore_stacktop_max() {									\
					_svc_stacktop_max = _sucheck_stacktop_max;							\
					_sucheck_stacktop_max = _task_cur->stacktop_max;					\
				}
#else
				#define _bsp_save_stacktop_max()

				#define _bsp_restore_stacktop_max()
#endif

#if defined(UBI_COMCONFIG__lib_ubik)

				#define _exception_preprocess() {										\
					if (0 != _ubik_active && 1 == _ubik_intrcount) {					\
						_bsp_save_stacktop_max();										\
						_bsp_checkstacklimit(_task_cur->stacklimit);					\
						_bsp_setstacklimit(_svc_stacklimit);							\
					}																	\
					else {																\
						_bsp_checkstacklimit(_svc_stacklimit);							\
					}																	\
				}

				#define _exception_postprocess() {										\
					if (0 != _ubik_active && 1 == _ubik_intrcount) {					\
						_bsp_checkstacklimit(_svc_stacklimit);							\
						_bsp_setstacklimit(_task_cur->stacklimit);						\
						_bsp_restore_stacktop_max();									\
					}																	\
					else {																\
						_bsp_checkstacklimit(_svc_stacklimit);							\
					}																	\
				}

#else

				#define _exception_preprocess() {										\
					_bsp_checkstacklimit(_svc_stacklimit);								\
				}

				#define _exception_postprocess() {										\
					_bsp_checkstacklimit(_svc_stacklimit);								\
				}

#endif

#else

			.extern _svc_stack;

	#if (1 == UBI_SOCHECK)
			.extern _svc_stacklimit
		#if (2 != UBI_SOCHECK_TYPE)
			.extern _socheck_stacklimit;
		#endif
	#endif
	#if (1 == UBI_SUCHECK)
			.extern _svc_stacktop_max
			.extern _sucheck_stacktop_max
	#endif

#endif

#endif /* _BSP_ARCH_H_ */
