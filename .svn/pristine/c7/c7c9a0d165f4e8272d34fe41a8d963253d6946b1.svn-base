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

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"
#include "../ubiconfig.h"


#if (0 == UBI_INCLUDE_STDLIB)

//#define __aeabi_div0  		__div0
//#define __aeabi_uimod		__umodsi3
//#define __aeabi_imod		__modsi3
//#define __aeabi_uidiv		__udivsi3
//#define __aeabi_idiv		__divsi3
//#define __aeabi_uidivmod	__udivmodsi3

void __aeabi_div0(void) {
	dtty_puts("\r\ndivide by zero\r\n", 80);
	bsp_abortsystem();
}

unsigned long __aeabi_uimod(unsigned long numerator, unsigned long denominator) {
	//////////////////////
	unsigned long quotient = 0;
	unsigned long bit = 1;

	if (0 == denominator) {
		__aeabi_div0();
		return 0;
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

	return numerator;
}

long __aeabi_imod(long _numerator, long _denominator) {
	int sign = 0;
	unsigned long numerator;
	unsigned long denominator;

	if (_numerator < 0) {
		sign = 1;
		_numerator = - _numerator;
	}
	numerator = (unsigned long) _numerator;

	if (_denominator < 0) {
		_denominator = - _denominator;
	}
	denominator = (unsigned long) _denominator;

	//////////////////////
	unsigned long quotient = 0;
	unsigned long bit = 1;

	if (0 == denominator) {
		__aeabi_div0();
		return 0;
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

	_numerator = (long) numerator;
	if (sign) {
		_numerator = - _numerator;
	}

	return _numerator;
}

unsigned long __aeabi_uidiv(unsigned long numerator, unsigned long denominator) {
	//////////////////////
	unsigned long quotient = 0;
	unsigned long bit = 1;

	if (0 == denominator) {
		__aeabi_div0();
		return 0;
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

	return quotient;
}

long __aeabi_idiv(long _numerator, long _denominator) {
	int sign = 0;
	unsigned long numerator;
	unsigned long denominator;

	if (_numerator < 0) {
		sign = 1;
		_numerator = - _numerator;
	}
	numerator = (unsigned long) _numerator;

	if (_denominator < 0) {
		sign = 1 - sign;
		_denominator = - _denominator;
	}
	denominator = (unsigned long) _denominator;

	//////////////////////
	unsigned long quotient = 0;
	unsigned long bit = 1;

	if (0 == denominator) {
		__aeabi_div0();
		return 0;
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

	_numerator = (long) quotient;
	if (sign) {
		_numerator = - _numerator;
	}

	return _numerator;
}

#endif /* (0 == UBI_INCLUDE_STDLIB) */

