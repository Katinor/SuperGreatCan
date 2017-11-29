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

#include "../../ubiconfig.h"

#if ( defined(__thumb__) && (UBI_CPU_TYPE__CORTEX_M3 != UBI_CPU_TYPE && UBI_CPU_TYPE__CORTEX_M4 != UBI_CPU_TYPE) )
	#error "This file has to be compiled with ARM operating state option, '-marm'."
#endif

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"

#include "_bsp_arch.h"

#if (1 != EXCLUDE_MISC)

void bsp_enableintr(void) {
	ARM_INTERRUPT_ENABLE();
}

void bsp_disableintr(void) {
	ARM_INTERRUPT_DISABLE();
}

#endif /* (1 != EXCLUDE_MISC) */

void bsp_busywait(unsigned int count) {
	__asm__ __volatile__ (													\
		"cmp	r0, #0												\n\t"	\
		"beq	2f													\n\t"	\
		"1:															\n\t"	\
		"subs	r0, r0, #1											\n\t"	\
		"bne	1b													\n\t"	\
		"2:															\n\t"	\
	);
}

#if (1 != EXCLUDE_MISC)

unsigned int arm_get_cp15(void) {
	register unsigned int value;
	__asm__ __volatile__ ("mrc p15, 0, %0, c1, c0, 0" : "=r" (value));
	return value;
}

void arm_set_cp15(unsigned int value) {
	__asm__ __volatile__ ("mcr p15, 0, %0, c1, c0, 0" : : "r" (value));
}

unsigned int arm_get_xpsr(void) {
	register unsigned int value;
	__asm__ __volatile__ ("mrs %0, xpsr" : "=r" (value));
	return value;
}

unsigned int arm_get_apsr(void) {
	register unsigned int value;
	__asm__ __volatile__ ("mrs %0, apsr" : "=r" (value));
	return value;
}

unsigned int arm_get_ipsr(void) {
	register unsigned int value;
	__asm__ __volatile__ ("mrs %0, ipsr" : "=r" (value));
	return value;
}

unsigned int arm_get_epsr(void) {
	register unsigned int value;
	__asm__ __volatile__ ("mrs %0, epsr" : "=r" (value));
	return value;
}

#endif /* (1 != EXCLUDE_MISC) */

