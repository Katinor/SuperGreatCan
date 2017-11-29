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

#include "_ubik_arch.h"

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubiclib.h"
#include "itf_ubinos/itf/ubiclib_heap.h"
#include "itf_ubinos/itf/ubiclib_logm.h"
#include "itf_ubinos/itf/ubik.h"
#include "itf_ubinos/itf/ubik_task.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ubik_porting_comp_init(unsigned int idle_stackdepth) {
	int r = 0;

	r = ubik_inittick();
	if (r != 0) {
		return -1;
	}

	r = task_comp_init(idle_stackdepth);
	if (r != 0) {
		return -1;
	}

	return 0;
}

void ubik_porting_comp_start(void) {
	task_comp_start();
}

int ubik_isactive(void) {
	return _ubik_active;
}

tickcount_t ubik_gettickcount(void) {
	tickcount_t tc;

	ubik_entercrit();

	tc.high	= _ubik_tickcounth;
	tc.low	= _ubik_tickcount;

	ubik_exitcrit();

	return tc;
}

tickcount_t ubik_gettickdiff(tickcount_t tick1, tickcount_t tick2) {
	tickcount_t diff;

	if (tick1.high > tick2.high) {
		diff.high = UINT_MAX - tick1.high + 1 + tick2.high;
	}
	else {
		diff.high = tick2.high - tick1.high;
	}

	if (tick1.low > tick2.low) {
		diff.high--;
		diff.low = UINT_MAX - tick1.low + 1 + tick2.low;
	}
	else {
		diff.low = tick2.low - tick1.low;
	}

	return diff;
}

void ubik_swisr(unsigned int swino) {
	switch(swino) {
	case SWINO__TASK_YIELD:
		_task_prev = _task_cur;
		if(TASK_STATE__RUNNING == _task_cur->state) {
			_task_cur->state = TASK_STATE__READY;
		}
		_task_cur = _tasklist_getcurnext(_task_list_ready_cur);
		_task_cur->state = TASK_STATE__RUNNING;
		break;
	}
}

void _task_stackinit(_task_pt task, void * arg) {
	unsigned int *	stacktop;
	unsigned int	stacklimit;

	stacktop 		= (unsigned int *) (((unsigned int) task->stack) + task->stacksize);
	stacklimit		= ((unsigned int) task->stack) + bsp_getsocheckmargin() + ARM_CONTEXT_SIZE;

	*(--stacktop) = ((unsigned int) 0x01000000);					/* psr		 			*/
	*(--stacktop) = ((unsigned int) _task_rootfunc) | 0x00000001;	/* r15 (pc)				*/
	*(--stacktop) = ((unsigned int) 0x00000000);					/* r14 (lr)				*/
	*(--stacktop) = ((unsigned int) 0x00001212);					/* r12					*/
	*(--stacktop) = ((unsigned int) 0x00000303);					/* r3 					*/
	*(--stacktop) = ((unsigned int) 0x00000202);					/* r2 					*/
	*(--stacktop) = ((unsigned int) 0x00000101);					/* r1 					*/
	*(--stacktop) = ((unsigned int) arg       );					/* r0 					*/

	*(--stacktop) = ((unsigned int) 0x00001111);					/* r11					*/
	*(--stacktop) = ((unsigned int) stacklimit);					/* r10 (stack limit)	*/
	*(--stacktop) = ((unsigned int) 0x00000909);					/* r9					*/
	*(--stacktop) = ((unsigned int) 0x00000808);					/* r8 					*/
	*(--stacktop) = ((unsigned int) 0x00000707);					/* r7 					*/
	*(--stacktop) = ((unsigned int) 0x00000606);					/* r6 					*/
	*(--stacktop) = ((unsigned int) 0x00000505);					/* r5 					*/
	*(--stacktop) = ((unsigned int) 0x00000404);					/* r4 					*/

	*(--stacktop) = 0 												/* _ubik_critcount = 0 */;

	task->stacktop = stacktop;
	task->stacktop_max = stacktop;
	task->stacklimit = (unsigned int *) stacklimit;

	return;
}

int task_getstacksize(task_pt _task, unsigned int * stacksize_p) {
	#define	__FUNCNAME__	"task_getstacksize"
	int r;
	_task_pt task = (_task_pt)_task;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	if (NULL == stacksize_p) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	if (NULL == task && 0 == _ubik_active) {
		r = bsp_getstacksize(0, stacksize_p);
		goto end0;
	}

	if (NULL == task) {
		task = _task_cur;
	}

	ubik_entercrit();

	if (0 == task->valid || (OBJTYPE__UBIK_TASK != task->type && OBJTYPE__UBIK_IDLETASK != task->type)) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	*stacksize_p = task->stacksize;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int task_getmaxstackusage(task_pt _task, unsigned int * maxstackusage_p) {
#if (1 == UBI_SUCHECK)

	#define	__FUNCNAME__	"task_getmaxstackusage"
	int r;
	_task_pt task = (_task_pt) _task;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	if (NULL == maxstackusage_p) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	if (NULL == task && 0 == _ubik_active) {
		r = bsp_getmaxstackusage(0, maxstackusage_p);
		goto end0;
	}

	if (NULL == task) {
		task = _task_cur;
	}

	ubik_entercrit();

	if (0 == task->valid || (OBJTYPE__UBIK_TASK != task->type && OBJTYPE__UBIK_IDLETASK != task->type)) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (_task_cur == task && 0 == _ubik_intrcount && 1 == _ubik_active) {
		task->stacktop_max = (unsigned int *) _sucheck_stacktop_max;
	}
	*maxstackusage_p = (unsigned int) task->stack + task->stacksize
			- (unsigned int) task->stacktop_max
			+ bsp_getsocheckmargin() + ARM_CONTEXT_SIZE;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__

#else
	if (NULL != maxstackusage_p) {
		*maxstackusage_p = 0;
	}

	return -1;
#endif
}

int ubik_iscrit(void) {
	if (	(0 != _ubik_critcount) ||
			(0 != _ubik_intrcount) 		) {
		return 1;
	}
	else {
		return 0;
	}
}

int ubik_istask(void) {
	if (	(0 != _ubik_active && 0 == _ubik_intrcount) 	) {
		return 1;
	}
	else {
		return 0;
	}
}

int ubik_isrt(void) {
	if (	(0 != _ubik_critcount) ||
			(0 != _ubik_intrcount) ||
			(0 != _ubik_tasklockcount) ||
			(0 != _ubik_active && NULL != _task_cur && _task_cur->priority >= _ubik_rtpriority)	) {
		return 1;
	}
	else {
		return 0;
	}
}

void _task_yield(void) {
	if (0 == _ubik_intrcount && 0 == _ubik_tasklockcount && 0 != _ubik_active) {
		if (0 == _ubik_critcount) {
			logme("not in critical section");
			bsp_abortsystem();
		}
		arm_set_pendsv();
		ARM_INTERRUPT_ENABLE();
		while (arm_get_pendsv());
	}
}
