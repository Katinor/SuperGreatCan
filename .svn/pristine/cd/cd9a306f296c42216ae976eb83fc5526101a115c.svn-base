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

#if ( (defined(__SOCHECK__) && !defined(__NOSOCHECK__)) || (defined(__SUCHECK__) && !defined(__NOSUCHECK__)) )
	#error "This file has to be compiled without stack overflow check and stack usage check option."
#endif

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_intr.h"
#include "itf_ubinos/itf/bsp_dtty.h"

#include "_bsp_arch.h"

void bsp_abortsystem(void) {
	ARM_INTERRUPT_DISABLE();

	dtty_puts_nosocheck("\r\n\r\nsystem is aborted\r\n\r\n", 80);

	__asm__ __volatile__ (
			"1:                                                 \n\t"
			"b	1b                                              \n\t"
	);
}

#if (1 != EXCLUDE_MISC)

unsigned int arm_get_cp15_nosocheck(void) {
	register unsigned int value;
	__asm__ __volatile__ ("mrc p15, 0, %0, c1, c0, 0" : "=r" (value));
	return value;
}

void arm_set_cp15_nosocheck(unsigned int value) {
	__asm__ __volatile__ ("mcr p15, 0, %0, c1, c0, 0" : : "r" (value));
}

unsigned int arm_get_xpsr_nosocheck(void) {
	register unsigned int value;
	__asm__ __volatile__ ("mrs %0, xpsr" : "=r" (value));
	return value;
}

unsigned int arm_get_apsr_nosocheck(void) {
	register unsigned int value;
	__asm__ __volatile__ ("mrs %0, apsr" : "=r" (value));
	return value;
}

unsigned int arm_get_ipsr_nosocheck(void) {
	register unsigned int value;
	__asm__ __volatile__ ("mrs %0, ipsr" : "=r" (value));
	return value;
}

unsigned int arm_get_epsr_nosocheck(void) {
	register unsigned int value;
	__asm__ __volatile__ ("mrs %0, epsr" : "=r" (value));
	return value;
}

#endif /* (1 != EXCLUDE_MISC) */

