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

#include "itf_ubinos/itf/bsp_intr.h"

#include "_bsp_board.h"

int intr_connectisr(int no, isr_ft isr, int priority, unsigned int option) {
	if (0 > no || 46 < no) {	// 140718 jaegeun - SAM4E exception table max is 46 !!!
		return -2;
	}
	if (NVIC_PRIO_LOWEST < priority || NVIC_PRIO_HIGHEST > priority) {
		return -3;
	}

	/* Enable the interrupt on the interrupt controller */
	NVIC->ICER[(no >> 5)] = (1 << (no & 0x1F));

	/* Clear the Pending Register on the interrupt controller */
	NVIC->ICPR[(no >> 5)] = (1 << (no & 0x1F));

	/* Set the interrupt priority */
	NVIC->IP[no] = (priority & 0xff);

	/* Register the interrupt handler on the interrupt handler table */
	_irq_handler_table[no] = isr;

	return 0;
}

int intr_enable(int no) {
	NVIC->ISER[(no >> 5)] = (1 << (no & 0x1F));
	return 0;
}

int intr_disable(int no) {
	NVIC->ICER[(no >> 5)] = (1 << (no & 0x1F));
	return 0;
}

int intr_ClearPending(int no){
	NVIC->ICPR[(no >> 5)] = (1 << (no & 0x1F));
	return 0;
}

int intr_getcurno() {
	unsigned int value;

#if defined(UBI_COMCONFIG__lib_ubik)
	if (0 == _ubik_intrcount) {
#else
	if (0 == _bsp_intrcount) {
#endif
		return -2;
	}
	__asm__ __volatile__ ("mrs %0, ipsr" : "=r" (value));

	return (int) value;
}

int intr_gethighestpriority(void) {
	return NVIC_PRIO_HIGHEST;
}

int intr_getlowestpriority(void) {
	return NVIC_PRIO_LOWEST;
}
