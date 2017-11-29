/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the lib_ubiclib_arm component of the Ubinos.

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

extern void __aeabi_div0(void);

// __aeabi_uidivmod(unsigned numerator:r0, unsigned denominator:r1)
//     Divide r0 by r1 and return the quotient in r0 and the remainder in r1

void __aeabi_uidivmod(unsigned long numerator, unsigned long denominator) {
	volatile unsigned long quotient_temp = 0;
	volatile unsigned long remainder_temp = 0;

	//////////////////////
	unsigned long quotient = 0;
	unsigned long bit = 1;

	if (0 == denominator) {
		__aeabi_div0();
		return;
	}

	while (denominator < numerator && !(denominator & (1L<<(sizeof(unsigned long)*8-1)))) {
		denominator <<=1;
		bit <<=1;
	}

	while (bit) {
		if (denominator <= numerator) {
			numerator -= denominator;
			quotient |= bit;
		}

		bit >>=1;
		denominator >>=1;
	}
	///////////////////////

	quotient_temp = quotient;
	remainder_temp = numerator;

	__asm__ __volatile__ ("ldr r0, %0" : : "m" (quotient_temp));
	__asm__ __volatile__ ("ldr r1, %0" : : "m" (remainder_temp));
	return;
}
