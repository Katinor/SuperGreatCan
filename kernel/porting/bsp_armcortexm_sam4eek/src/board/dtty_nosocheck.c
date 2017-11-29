/*
  Copyright (C) 2011 RTLab
  Contact: ubinos.org@gmail.com

  Contributors: Dong Hoon Kim, Sung Ho Park

  This file is part of the bsp_armcortexm_sam3sek component of the Ubinos.

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

#if ( (defined(__SOCHECK__) && !defined(__NOSOCHECK__)) || (defined(__SUCHECK__) && !defined(__NOSUCHECK__)) )
	#error "This file has to be compiled without stack overflow check and stack usage check option."
#endif

#include "itf_ubinos/itf/bsp_dtty.h"

#include "_bsp_board.h"

int dtty_putc_nosocheck(int ch) {
	for (;;) {
		if(USART0->US_CSR & US_CSR_TXRDY) {
			break;
		}
	}

	USART0->US_THR = (ch & 0xFF);

	return 0;
}

int dtty_puts_nosocheck(const char * str, int max) {
	int i = 0;

	if (NULL == str) {
		return -2;
	}

	for (i=0; i<max; i++) {
		if ('\0' == *str) {
			break;
		}

		for (;;) {
			if(USART0->US_CSR & US_CSR_TXRDY) {
				break;
			}
		}
		USART0->US_THR = (0x000000FF & *str);

		str++;
	}

	return i;
}

