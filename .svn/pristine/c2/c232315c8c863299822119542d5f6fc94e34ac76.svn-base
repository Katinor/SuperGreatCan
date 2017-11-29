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

#if ( (defined(__SOCHECK__) && !defined(__NOSOCHECK__)) || (defined(__SUCHECK__) && !defined(__NOSUCHECK__)) )
	#error "This file has to be compiled without stack overflow check and stack usage check option."
#endif

#include <stdio.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"

#include "_bsp_arch.h"

#if (1 != EXCLUDE_MISC)

unsigned int bsp_getsocheckmargin_nosocheck(void) {
	return SOCHECK_MARGIN;
}

int bsp_getstacksize_nosocheck(int type, unsigned int * stacksize_p) {
	if (NULL == stacksize_p) {
		return -3;
	}

	switch (type) {
	case 0:
		*stacksize_p = (SVC_STACK_SIZE + ABT_STACK_SIZE + UND_STACK_SIZE + FIQ_STACK_SIZE + IRQ_STACK_SIZE + SYS_STACK_SIZE);
		return 0;

	case 1:
		*stacksize_p = SVC_STACK_SIZE;
		return 0;

	default:
		return -2;
	}
}

int bsp_getmaxstackusage_nosocheck(int type, unsigned int * maxstackusage_p) {
#if (1 == UBI_SUCHECK)
	if (NULL == maxstackusage_p) {
		return -3;
	}

	switch (type) {
	case 0:
#if defined(UBI_COMCONFIG__lib_ubik)
		if (0 < _ubik_intrcount || 0 == _ubik_active) {
			_svc_stacktop_max = _sucheck_stacktop_max;
		}
#else
		_svc_stacktop_max = _sucheck_stacktop_max;
#endif

		*maxstackusage_p = (ABT_STACK_SIZE + UND_STACK_SIZE + FIQ_STACK_SIZE + IRQ_STACK_SIZE + SYS_STACK_SIZE)
			+ ((unsigned int) _svc_stack - (unsigned int) _svc_stacktop_max)
			+  SOCHECK_MARGIN + ARM_CONTEXT_SIZE;

		return 0;

	case 1:
#if defined(UBI_COMCONFIG__lib_ubik)
		if (0 < _ubik_intrcount || 0 == _ubik_active) {
			_svc_stacktop_max = _sucheck_stacktop_max;
		}
#else
		_svc_stacktop_max = _sucheck_stacktop_max;
#endif

		*maxstackusage_p = ((unsigned int) _svc_stack - (unsigned int) _svc_stacktop_max)
			+  SOCHECK_MARGIN + ARM_CONTEXT_SIZE;

		return 0;

	default:
		return -2;
	}
#else
	if (NULL != maxstackusage_p) {
		*maxstackusage_p = 0;
	}

	return -1;
#endif
}

#endif /* (1 != EXCLUDE_MISC) */
