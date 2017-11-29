/*
  Copyright (C) 2011 RTLab, Yu Jin Park, Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the lib_ubik_armcortexm component of the Ubinos.

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

#ifndef _UBIK_ARCH_H_
#define _UBIK_ARCH_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file _ubik_arch.h
 *
 * @brief ubik (Ubinos Kernel) ARM classic porting ������Ʈ ���� �������̽�
 *
 * ubik ARM classic porting ������Ʈ ���� �������̽��� �����մϴ�.
 */

#include "../ubiconfig.h"

#include "../../../common/lib_ubik/src/_ubik.h"


/* Common definition for ARM */

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

void ubik_irq_handler(void) __attribute__((naked));
void ubik_swi_handler(void) __attribute__((naked));
void ubik_psv_handler(void) __attribute__((naked));

#if (1 == UBI_SUCHECK)
extern volatile unsigned int * 	_svc_stacktop_max;
extern volatile unsigned int * 	_sucheck_stacktop_max;
#endif

#else

#if (1 == UBI_SUCHECK)
.extern 						_svc_stacktop_max
.extern 						_sucheck_stacktop_max
#endif

#endif /* __ASSEMBLY__ */

#ifdef	__cplusplus
}
#endif

#endif /* _UBIK_ARCH_H_ */
