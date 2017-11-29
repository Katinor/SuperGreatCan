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

#ifndef _BSP_H_
#define _BSP_H_

#include "../../ubiconfig.h"

#ifndef __ASSEMBLY__

	int main(void);

	#if defined(UBI_COMCONFIG__lib_ubik)
			#include "../../../../common/lib_ubik/src/_ubik.h"
	#else
			extern unsigned int _bsp_intrcount;
	#endif

	#if (1 == UBI_SOCHECK)
		#if defined(UBI_COMCONFIG__lib_ubik)
			#define SOCHECK_OVERFLOW_HANDLER_BUF_SIZE	12
			extern char 		_socheck_overflow_handler__buf[SOCHECK_OVERFLOW_HANDLER_BUF_SIZE];
			extern unsigned int	_socheck_overflow_handler__var1;
			extern unsigned int	_socheck_overflow_handler__var2;
			extern _task_pt 	_socheck_overflow_handler__task;
		#endif
	#endif

#if defined(UBI_COMCONFIG__lib_ubik)

				#define _exception_increase_intrcount() {								\
						_ubik_intrcount++;												\
				}

				#define _exception_decrease_intrcount() {								\
						_ubik_intrcount--;												\
				}

				#define _exception_check_ubik_critcount() {								\
					if (0 != _ubik_critcount) {											\
						dtty_puts_nosocheck("\r\ninterrupt disable fail\r\n", 80);		\
						bsp_abortsystem();												\
					}																	\
				}
#else

				#define _exception_increase_intrcount() {								\
						_bsp_intrcount++;												\
				}

				#define _exception_decrease_intrcount() {								\
						_bsp_intrcount--;												\
				}

				#define _exception_check_ubik_critcount()

#endif

#endif /* __ASSEMBLY__ */

#endif /* _BSP_H_ */

