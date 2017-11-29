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

#include "../ubiconfig.h"

//#define __aeabi_div0  	__div0_temp
//#define __aeabi_uimod		__umodsi3_temp
//#define __aeabi_imod		__modsi3_temp
//#define __aeabi_uidiv		__udivsi3_temp
//#define __aeabi_idiv		__divsi3_temp

#define TEST_INTERVAL_I		(INT_MAX / 1000 + 7)
#define TEST_INTERVAL_J		(INT_MAX / 1000 + 11)

extern unsigned long __umodsi3_temp(unsigned long numerator, unsigned long denominator);
extern long __modsi3_temp(long _numerator, long _denominator);
extern unsigned long __udivsi3_temp(unsigned long numerator, unsigned long denominator);
extern long __divsi3_temp(long _numerator, long _denominator);
extern long __udivmodsi3_temp(unsigned long numerator, unsigned long denominator, int modwante);

void moddiv_test(void) {
	printf("__umodsi3\n\r");
	//unsigned long ui, uj, uk, um;
	ui = 0;
	for (;;) {
		uj = 0;
		for (;;) {
			if (0 == uj) {
				uj += TEST_INTERVAL_J;
				continue;
			}

			uk = ui % uj;
			um = __umodsi3_temp (ui, uj);
			if (uk != um) {
				printf ("__umodsi3: fail %ld %ld %ld %ld\n", ui, uj, uk, um);
			}

			if (UINT_MAX == uj) {
				break;
			}
			else if (UINT_MAX - uj < TEST_INTERVAL_J) {
				uj = UINT_MAX;
			}
			else {
				uj += TEST_INTERVAL_J;
			}
		}

		if (UINT_MAX == ui) {
			break;
		}
		else if (UINT_MAX - ui < TEST_INTERVAL_I) {
			ui = UINT_MAX;
		}
		else {
			ui += TEST_INTERVAL_I;
		}
	}

	printf("__modsi3\n\r");
	//long i, j, k, m;
	i = INT_MIN;
	for (;;) {
		j = INT_MIN;
		for (;;) {
			if (0 == j) {
				j += TEST_INTERVAL_J;
				continue;
			}

			k = i % j;
			m = __modsi3_temp(i, j);
			if (k != m) {
				printf ("__modsi3: fail %ld %ld %ld %ld\n", i, j, k, m);
			}

			if (INT_MAX == j) {
				break;
			}
			else if (0 < j && INT_MAX - j < TEST_INTERVAL_J) {
				j = INT_MAX;
			}
			else {
				j += TEST_INTERVAL_J;
			}
		}

		if (INT_MAX == i) {
			break;
		}
		else if (0 < i && INT_MAX - i < TEST_INTERVAL_I) {
			i = INT_MAX;
		}
		else {
			i += TEST_INTERVAL_I;
		}
	}

	printf("__udivsi3\n\r");
	unsigned long ui, uj, uk, um;
	ui = 0;
	for (;;) {
		uj = 0;
		for (;;) {
			if (0 == uj) {
				uj += TEST_INTERVAL_J;
				continue;
			}

			uk = ui / uj;
			um = __udivsi3_temp(ui, uj);
			if (uk != um) {
				printf ("__udivsi3: fail %ld %ld %ld %ld\n", ui, uj, uk, um);
			}

			if (UINT_MAX == uj) {
				break;
			}
			else if (UINT_MAX - uj < TEST_INTERVAL_J) {
				uj = UINT_MAX;
			}
			else {
				uj += TEST_INTERVAL_J;
			}
		}

		if (UINT_MAX == ui) {
			break;
		}
		else if (UINT_MAX - ui < TEST_INTERVAL_I) {
			ui = UINT_MAX;
		}
		else {
			ui += TEST_INTERVAL_I;
		}
	}

	printf("__divsi3\n\r");
	long i, j, k, m;
	i = INT_MIN;
	for (;;) {
		j = INT_MIN;
		for (;;) {
			if (0 == j) {
				j += TEST_INTERVAL_J;
				continue;
			}

			k = i / j;
			m = __divsi3_temp(i, j);
			if (k != m) {
				printf ("__divsi3: fail %ld %ld %ld %ld\n", i, j, k, m);
			}

			if (INT_MAX == j) {
				break;
			}
			else if (0 < j && INT_MAX - j < TEST_INTERVAL_J) {
				j = INT_MAX;
			}
			else {
				j += TEST_INTERVAL_J;
			}
		}

		if (INT_MAX == i) {
			break;
		}
		else if (0 < i && INT_MAX - i < TEST_INTERVAL_I) {
			i = INT_MAX;
		}
		else {
			i += TEST_INTERVAL_I;
		}
	}
}

