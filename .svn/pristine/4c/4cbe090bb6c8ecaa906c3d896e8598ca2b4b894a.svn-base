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

#include "../ubiconfig.h"

#if (UBI_CPU_TYPE__CORTEX_M3 != UBI_CPU_TYPE && UBI_CPU_TYPE__CORTEX_M4 != UBI_CPU_TYPE )
	#if ( defined(__thumb__) && (UBI_CPU_TYPE__CORTEX_M3 != UBI_CPU_TYPE && UBI_CPU_TYPE__CORTEX_M4 != UBI_CPU_TYPE) )
		#error "This file has to be compiled with ARM operating state option, '-marm'."
	#endif
#endif

#if ( (defined(__SOCHECK__) && !defined(__NOSOCHECK__)) || (defined(__SUCHECK__) && !defined(__NOSUCHECK__)) )
	#error "This file has to be compiled without stack overflow check and stack usage check option."
#endif

#include "_ubik_arch.h"

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"

void ubik_entercrit_nosocheck(void) {
	if (0 == _ubik_intrcount) {
		ARM_INTERRUPT_DISABLE();
		_ubik_critcount++;
	}
}

void ubik_exitcrit_nosocheck(void) {
	if (0 == _ubik_intrcount) {
		if (0 == _ubik_critcount) {
			dtty_puts("\r\nubik_exitcrit_nosocheck fail (_ubik_critcount is already 0)\r\n", 80);
			bsp_abortsystem();
		}
		_ubik_critcount--;
		if (0 == _ubik_critcount && 0 != _ubik_active) {
			ARM_INTERRUPT_ENABLE();
		}
	}
}
