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

#include <stdio.h>

#if !defined(__thumb__)
	#error "This file has to be compiled with THUMB operating state option, '-mthumb'."
#endif

#if ( (defined(__SOCHECK__) && !defined(__NOSOCHECK__)) || (defined(__SUCHECK__) && !defined(__NOSUCHECK__)) )
	#error "This file has to be compiled without stack overflow check and stack usage check option."
#endif

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_intr.h"
#include "itf_ubinos/itf/bsp_dtty.h"

#include "_bsp.h"

#if (1 == UBI_SOCHECK)
				void _socheck_overflow_handler__thumb (void) {
	#if defined(UBI_COMCONFIG__lib_ubik)
					ubik_entercrit_nosocheck();

					dtty_puts_nosocheck("\r\nstack overflowed", 80);

					if (0 != _ubik_intrcount) {
						dtty_puts_nosocheck(" at isr (svc stack)\r\n", 80);
					}
					else if (0 != _ubik_active) {
						dtty_puts_nosocheck(" at task \"", 80);
						dtty_puts_nosocheck(_task_cur->name, 80);
						dtty_puts_nosocheck("\"\r\n", 80);
					}
					else {
						dtty_puts_nosocheck(" at system init code (svc stack)\r\n", 80);
					}
		#if (1 == UBI_SUCHECK)
					dtty_puts_nosocheck("\r\n", 80);
					dtty_puts_nosocheck("name : stack size (byte(depth)), stack usage maximum record (byte(depth))\r\n", 80);

					dtty_puts_nosocheck("svc  : 0x", 80);
					bsp_getstacksize_nosocheck(1, &_socheck_overflow_handler__var1);
					_socheck_overflow_handler__var2 = _socheck_overflow_handler__var1/INT_SIZE;
					if (0 != _socheck_overflow_handler__var1%INT_SIZE) {
						_socheck_overflow_handler__var2++;
					}
					uitoah_nosocheck(_socheck_overflow_handler__var1, _socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
					dtty_puts_nosocheck(_socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
					dtty_puts_nosocheck("(0x", 80);
					uitoah_nosocheck(_socheck_overflow_handler__var2, _socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
					dtty_puts_nosocheck(_socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
					dtty_puts_nosocheck("), 0x", 80);

					bsp_getmaxstackusage_nosocheck(1, &_socheck_overflow_handler__var1);
					_socheck_overflow_handler__var2 = _socheck_overflow_handler__var1/INT_SIZE;
					if (0 != _socheck_overflow_handler__var1%INT_SIZE) {
						_socheck_overflow_handler__var2++;
					}
					uitoah_nosocheck(_socheck_overflow_handler__var1, _socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
					dtty_puts_nosocheck(_socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
					dtty_puts_nosocheck("(0x", 80);
					uitoah_nosocheck(_socheck_overflow_handler__var2, _socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
					dtty_puts_nosocheck(_socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
					dtty_puts_nosocheck(")\r\n", 80);
			#if (1 != EXCLUDE_KERNEL_MONITORING)
					_socheck_overflow_handler__task = _kernel_monitor_tasklist_head(&_kernel_monitor_tasklist);
					for (; NULL != _socheck_overflow_handler__task; _socheck_overflow_handler__task = _kernel_monitor_tasklist_next(_socheck_overflow_handler__task)) {
						dtty_puts_nosocheck(_socheck_overflow_handler__task->name, 12);
				#if (1 != EXCLUDE_IDLETASK_HOOKFUNC)
						if (OBJTYPE__UBIK_IDLETASK == _socheck_overflow_handler__task->type && NULL != _task_idletaskhookfunc_func) {
							dtty_puts_nosocheck("(", 80);
							dtty_puts_nosocheck(_task_idletaskhookfunc_name, TASK_NAME_SIZE_MAX);
							dtty_puts_nosocheck(")", 80);
						}
				#endif
						dtty_puts_nosocheck(" : 0x", 80);

						task_getstacksize_nosocheck((task_pt) _socheck_overflow_handler__task, &_socheck_overflow_handler__var1);
						_socheck_overflow_handler__var2 = _socheck_overflow_handler__var1/INT_SIZE;
						if (0 != _socheck_overflow_handler__var1%INT_SIZE) {
							_socheck_overflow_handler__var2++;
						}
						uitoah_nosocheck(_socheck_overflow_handler__var1, _socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
						dtty_puts_nosocheck(_socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
						dtty_puts_nosocheck("(0x", 80);
						uitoah_nosocheck(_socheck_overflow_handler__var2, _socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
						dtty_puts_nosocheck(_socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
						dtty_puts_nosocheck("), 0x", 80);

						task_getmaxstackusage_nosocheck((task_pt) _socheck_overflow_handler__task, &_socheck_overflow_handler__var1);
						_socheck_overflow_handler__var2 = _socheck_overflow_handler__var1/INT_SIZE;
						if (0 != _socheck_overflow_handler__var1%INT_SIZE) {
							_socheck_overflow_handler__var2++;
						}
						uitoah_nosocheck(_socheck_overflow_handler__var1, _socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
						dtty_puts_nosocheck(_socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
						dtty_puts_nosocheck("(0x", 80);
						uitoah_nosocheck(_socheck_overflow_handler__var2, _socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
						dtty_puts_nosocheck(_socheck_overflow_handler__buf, SOCHECK_OVERFLOW_HANDLER_BUF_SIZE);
						dtty_puts_nosocheck(")\r\n", 80);
					}
			#endif
		#endif
					ubik_exitcrit_nosocheck();
	#else
					dtty_puts_nosocheck("\r\nstack overflowed (svc stack)\r\n", 80);
	#endif
					bsp_abortsystem();
					
					return;
				}
#endif
