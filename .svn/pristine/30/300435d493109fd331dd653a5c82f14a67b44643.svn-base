/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the lib_ubik component of the Ubinos.

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

#if (!defined(UBI_COMCONFIG__lib_ubik__dummy))

#include <stdio.h>
#include <string.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubiclib_logm.h"
#include "itf_ubinos/itf/ubik.h"
#include "itf_ubinos/itf/ubik_mutex.h"

#include "_ubik.h"

#if 	(0 >= TASK_NAME_SIZE_MAX)
	#error "TASK_NAME_SIZE_MAX should be larger than 0."
#endif

#if 	(9 > TASK_PRIORITY_MAX)
	#error "TASK_PRIORITY_MAX should be equal or larger than 9."
#endif

#if 	(INT_MAX <= TASK_PRIORITY_MAX)
	#error "TASK_PRIORITY_MAX should be less than INT_MAX."
#endif

#if		(56 > TASK_STACK_DEPTH_MIN)
	#error "TASK_STACK_DEPTH_MIN should be equal or larger than 56."
#endif

#if		(TASK_STACK_DEPTH_MIN > TASK_STACK_DEPTH_DEFAULT)
	#error "TASK_STACK_DEPTH_DEFAULT should be equal or larger than TASK_STACK_DEPTH_MIN."
#endif

#if		(16 < TASK_MAXWAITSIGOBJ_MAX)
	#error "TASK_MAXWAITSIGOBJ_MAX should be equal or less than 16."
#endif

#if		(1 > IDLETASK_SPINWAIT_INTERVALTICK)
	#error "IDLETASK_SPINWAIT_INTERVALTICK should be equal or larger than 1."
#endif

#if		(32 < MSGQ_MSGSIZE_MAX)
	#error "MSGQ_MSGSIZE_MAX should be equal or less than 32."
#endif

/*****************************************************************************/

volatile unsigned int	_ubik_active								= 0;		/* variable representing whether system is active or not */
volatile unsigned int	_ubik_tickcount								= 0;		/* system tick count */
volatile unsigned int	_ubik_tickcounth							= 0;		/* system tick count high value */
volatile unsigned int	_ubik_intrcount								= 0;		/* interrupt nesting count */
volatile unsigned int	_ubik_critcount								= 0;		/* critical section nesting count */
volatile unsigned int	_ubik_tasklockcount							= 0;		/* task lock nesting count */
volatile unsigned int	_ubik_rtpriority							= 0;		/* lowest priority of task considered as a real-time task */

		 edlist_t		_task_list_blocked_timed_a[2]				= {{0,},};	/* timed blocked task list array */
volatile edlist_pt		_task_list_blocked_timed_cur 				= NULL;		/* current timed blocked task list pointer */
volatile edlist_pt		_task_list_blocked_timed_next 				= NULL;		/* next timed blocked task list pointer */
		 edlist_t		_task_list_blocked							= {0,};		/* blocked forever task list */
		 edlist_t		_task_list_ready_a[TASK_PRIORITY_MAX+1]		= {{0,},};	/* ready task list array */
volatile unsigned int 	_task_list_ready_index 						= 0;		/* current ready task list index */
volatile edlist_pt		_task_list_ready_cur 						= NULL;		/* current ready task list pointer */
		 edlist_t		_task_list_suspended						= {0,};		/* suspended task list */
		 edlist_t		_task_list_terminated						= {0,};		/* terminated task list */
volatile _task_pt 		_task_cur 									= NULL;		/* current running task pointer */
volatile _task_pt 		_task_prev									= NULL;		/* next running task pointer */

#if (1 != EXCLUDE_STIMER)
		 edlist_t		_stimer_list_a[2]							= {{0,},};	/* stimer list array */
volatile edlist_pt		_stimer_list_cur 							= NULL;		/* current stimer list pointer */
volatile edlist_pt		_stimer_list_next 							= NULL;		/* next stimer list pointer */
#endif

#if (1 != EXCLUDE_KERNEL_MONITORING)
		 mutex_pt		_kernel_monitor_mutex						= NULL;
		 edlist_t		_kernel_monitor_tasklist					= {0,};
		 edlist_t		_kernel_monitor_stimerlist					= {0,};
		 edlist_t		_kernel_monitor_sigobjlist					= {0,};
		 edlist_t		_kernel_monitor_iobuflist					= {0,};
#endif

#if (1 != EXCLUDE_IDLETASK_HOOKFUNC)
		idletaskhookfunc_ft	_task_idletaskhookfunc_func							= NULL;
		void * 				_task_idletaskhookfunc_arg							= NULL;
		unsigned int		_task_idletaskhookfunc_option						= 0;
		char				_task_idletaskhookfunc_name[TASK_NAME_SIZE_MAX]		= {0,};
#endif

#if (1 != EXCLUDE_TICK_HOOKFUNC)
		tickhookfunc_ft		_ubik_tickhookfunc_func								= NULL;
#endif

#if (1 != EXCLUDE_HRTICK)
volatile unsigned int	_ubik_hrtick_active							= 0;		/* variable representing whether high resolution tick is active or not */
#endif /* (1 != EXCLUDE_HRTICK) */

/*****************************************************************************/

int ubik_comp_init(unsigned int idle_stackdepth) {
	#define	__FUNCNAME__	"ubik_comp_init"
	int r;

	_ubik_active		= 0;
	_ubik_tickcount		= 0;
	_ubik_tickcounth	= 0;
	_ubik_intrcount		= 0;
	_ubik_critcount		= 0;
	_ubik_tasklockcount	= 0;
	_ubik_rtpriority	= INT_MAX;

	_task_list_blocked_timed_cur 	= NULL;
	_task_list_blocked_timed_next 	= NULL;
	_task_list_ready_index 			= 0;
	_task_list_ready_cur 			= NULL;

	_task_cur 						= NULL;
	_task_prev 						= NULL;

	#if (1 != EXCLUDE_STIMER)
	_stimer_list_cur 				= NULL;
	_stimer_list_next 				= NULL;
	#endif

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	edlist_init(&_kernel_monitor_tasklist);
	edlist_init(&_kernel_monitor_stimerlist);
	edlist_init(&_kernel_monitor_sigobjlist);
	edlist_init(&_kernel_monitor_iobuflist);
	#endif

	r = ubik_porting_comp_init(idle_stackdepth);
	if (r != 0) {
		logme(""__FUNCNAME__": fail at ubik_porting_comp_init\r\n");
		goto end0;
	}

	#if (1 != EXCLUDE_STIMER)
	edlist_init(&_stimer_list_a[0]);
	edlist_init(&_stimer_list_a[1]);
	_stimer_list_cur  	= &_stimer_list_a[0];
	_stimer_list_next 	= &_stimer_list_a[1];
	#endif

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	r = mutex_create(&_kernel_monitor_mutex);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_create\r\n");
		goto end0;
	}
	#endif

	#if (1 != EXCLUDE_IDLETASK_HOOKFUNC)
	_task_idletaskhookfunc_func		= NULL;
	_task_idletaskhookfunc_arg		= NULL;
	_task_idletaskhookfunc_option	= 0;
	_task_idletaskhookfunc_name[0]	= 0x00;
	#endif

	#if (1 != EXCLUDE_TICK_HOOKFUNC)
	_ubik_tickhookfunc_func			= NULL;
	#endif

	#if (1 != EXCLUDE_HRTICK)
	_ubik_hrtick_active				= 0;
	#endif /* (1 != EXCLUDE_HRTICK) */

	r = 0;

end0:
	#undef __FUNCNAME__
	return r;
}

int ubik_comp_start(void) {

	ubik_porting_comp_start();

	return 0;
}

int ubik_collectgarbage(void) {
	_task_collectgarbage();

	return 0;
}

int ubik_sprintkernelinfo(char * buf, int max) {
#if (1 != EXCLUDE_KERNEL_MONITORING)
	#define	__FUNCNAME__	"ubik_sprintkernelinfo"
	int r;
	int r2;
	_task_pt 	task;
	_stimer_pt 	stimer;
	_sigobj_pt 	sigobj;
	#if (1 != EXCLUDE_IOBUF)
	_iobuf_pt 	iobuf;
	#endif /* (1 != EXCLUDE_IOBUF) */
	unsigned int stacksize;
	unsigned int stacksizedepth;
	unsigned int maxstackusage;
	unsigned int maxstackusagedepth;

	if (0 != _ubik_intrcount) {
		logme(""__FUNCNAME__": in interrupt\r\n");
		r = -1;
		goto end0;
	}

	if (0 != _ubik_critcount) {
		logme(""__FUNCNAME__": in critical section\r\n");
		r = -1;
		goto end0;
	}

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	if (NULL == buf) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end0;
	}

	if (max <= 0) {
		r = 0;
		goto end0;
	}

	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
		r = -1;
		goto end0;
	}

	r = 0;
	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "|task    |address    |function   |stack size   |max stack    |priority   |state   |suspended |timed |name               \r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "|type    |           |address    |(byte(depth))|usage        |(original) |        |          |      |                   \r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "|        |           |           |             |(byte(depth))|           |        |          |      |                   \r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\r\n");

	bsp_getstacksize(1, &stacksize);
	stacksizedepth = stacksize/INT_SIZE;
	if (0 != stacksize%INT_SIZE) {
		stacksizedepth++;
	}
	r2 = bsp_getmaxstackusage(1, &maxstackusage);
	maxstackusagedepth = maxstackusage/INT_SIZE;
	if (0 != maxstackusage%INT_SIZE) {
		maxstackusagedepth++;
	}
	if (0 != r2) {
		maxstackusage = 0;
		maxstackusagedepth = 0;
	}
	if (r < max) r += snprintf(&buf[r], max-r, "|svc     |           |           | 0x%08x  | 0x%08x  |           |        |          |      |        \r\n", stacksize, maxstackusage);
	if (r < max) r += snprintf(&buf[r], max-r, "|        |           |           |(0x%08x) |(0x%08x) |           |        |          |      |                   \r\n",
			stacksizedepth, maxstackusagedepth);

	task = _kernel_monitor_tasklist_head(&_kernel_monitor_tasklist);
	for (; NULL != task; task = _kernel_monitor_tasklist_next(task)) {
		stacksize = task->stacksize;
		stacksizedepth = stacksize/INT_SIZE;
		if (0 != stacksize%INT_SIZE) {
			stacksizedepth++;
		}
		r2 = task_getmaxstackusage((task_pt) task, &maxstackusage);
		maxstackusagedepth = maxstackusage/INT_SIZE;
		if (0 != maxstackusage%INT_SIZE) {
			maxstackusagedepth++;
		}
		if (0 != r2) {
			maxstackusage = 0;
			maxstackusagedepth = 0;
		}
		if (r < max) r += snprintf(&buf[r], max-r, "|task    |0x%08x |0x%08x | 0x%08x  | 0x%08x  |%04d(%04d) |",
				(unsigned int) task, (unsigned int) (task->func), stacksize, maxstackusage, task->priority, task->priority_ori);
		switch (task->state) {
		case TASK_STATE__READY:
			if (r < max) r += snprintf(&buf[r], max-r, "ready  ");
			break;
		case TASK_STATE__RUNNING:
			if (r < max) r += snprintf(&buf[r], max-r, "running");
			break;
		case TASK_STATE__BLOCKED:
			if (r < max) r += snprintf(&buf[r], max-r, "blocked");
			break;
		default:
			if (r < max) r += snprintf(&buf[r], max-r, "unknown");
			break;
		}
		if (r < max) r += snprintf(&buf[r], max-r, " |%d         |%d     |%s",
				task->suspended, task->timed, task->name);
#if (1 != EXCLUDE_IDLETASK_HOOKFUNC)
		if (OBJTYPE__UBIK_IDLETASK  == task->type && NULL != _task_idletaskhookfunc_func) {
			if (r < max) r += snprintf(&buf[r], max-r, " (%s)", _task_idletaskhookfunc_name);
		}
#endif
		if (r < max) r += snprintf(&buf[r], max-r, "\r\n");

		if (r < max) r += snprintf(&buf[r], max-r, "|        |           |           |(0x%08x) |(0x%08x) |           |        |          |      |                   \r\n",
				stacksizedepth, maxstackusagedepth);
	}
	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "\r\n");

	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "|timer   |address    |semaphore  |period     |oneshot |running                                      \r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "|type    |           |address    |(tick)     |        |                                             \r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\r\n");
	stimer = _kernel_monitor_stimerlist_head(&_kernel_monitor_stimerlist);
	for (; NULL != stimer; stimer = _kernel_monitor_stimerlist_next(stimer)) {
		if (r < max) r += snprintf(&buf[r], max-r, "|stimer  |0x%08x |0x%08x |0x%08x |%d       |%d\r\n",
				(unsigned int) stimer, (unsigned int) (stimer->sigobj), stimer->tick, stimer->oneshot, stimer->running);
	}
	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "\r\n");

	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "|sigobj  |address    |waiting |max      |sender     |count      |max        |etc                    \r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "|type    |           |task    |priority |(owner)    |(size)     |count      |                       \r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "|        |           |count   |         |address    |           |(size)     |                       \r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\r\n");
	sigobj = _kernel_monitor_sigobjlist_head(&_kernel_monitor_sigobjlist);
	for (; NULL != sigobj; sigobj = _kernel_monitor_sigobjlist_next(sigobj)) {
		switch (sigobj->type) {
		case OBJTYPE__UBIK_SIGNAL:
			if (r < max) r += snprintf(&buf[r], max-r, "|signal  |0x%08x |%04d    |%04d     |0x%08x |           |           |\r\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj));
			break;
		case OBJTYPE__UBIK_CONDV:
			if (r < max) r += snprintf(&buf[r], max-r, "|condv   |0x%08x |%04d    |%04d     |0x%08x |           |           |\r\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj));
			break;
		case OBJTYPE__UBIK_MUTEX:
			if (r < max) r += snprintf(&buf[r], max-r, "|mutex   |0x%08x |%04d    |%04d     |0x%08x |0x%08x |           |n: %d\r\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->nopriorityinheritance);
			break;
		case OBJTYPE__UBIK_SEM:
			if (r < max) r += snprintf(&buf[r], max-r, "|sem     |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |i: %d\r\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->maxcount, sigobj->ignoreoverflow);
			break;
		case OBJTYPE__UBIK_MSGQ:
			if (r < max) r += snprintf(&buf[r], max-r, "|msgq    |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |msg size: 0x%04x\r\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->maxcount, sigobj->msgsize_1 + 1);
			break;
		default:
			if (r < max) r += snprintf(&buf[r], max-r, "|unknown |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |type: 0x%04x\r\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->maxcount, sigobj->type);
			break;
		}
	}

	#if (1 != EXCLUDE_IOBUF)
	iobuf = _kernel_monitor_iobuflist_head(&_kernel_monitor_iobuflist);
	for (; NULL != iobuf; iobuf = _kernel_monitor_iobuflist_next(iobuf)) {
		sigobj = (_sigobj_pt) (iobuf->condv);
		if (r < max) r += snprintf(&buf[r], max-r, "|iobuf   |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |\r\n",
				(unsigned int) iobuf, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
				iobuf->size, iobuf->maxsize);
	}
	#endif /* (1 != EXCLUDE_IOBUF) */
	if (r < max) r += snprintf(&buf[r], max-r, "------------------------------------------------------------------------------------------------------------------------\r\n");
	if (r < max) r += snprintf(&buf[r], max-r, "\r\n");

	r2 = mutex_unlock(_kernel_monitor_mutex);
	if (0 != r2) {
		logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
		r = -1;
	}

end0:
	return r;
	#undef __FUNCNAME__
#else
	#define	__FUNCNAME__	"ubik_sprintkernelinfo"
	int r;

	if (NULL == buf) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end0;
	}

	if (max <= 0) {
		r = 0;
		goto end0;
	}

	buf[0] = 0;

	r = 0;

end0:
	return r;
	#undef __FUNCNAME__
#endif
}
int ubik_printkernelinfo(void) {
#if (1 != EXCLUDE_KERNEL_MONITORING)
	#define	__FUNCNAME__	"ubik_printkernelinfo"
	int r;
	int r2;
	_task_pt 	task;
	_stimer_pt 	stimer;
	_sigobj_pt 	sigobj;
	#if (1 != EXCLUDE_IOBUF)
	_iobuf_pt 	iobuf;
	#endif /* (1 != EXCLUDE_IOBUF) */
	unsigned int stacksize;
	unsigned int stacksizedepth;
	unsigned int maxstackusage;
	unsigned int maxstackusagedepth;

	if (0 != _ubik_intrcount) {
		logme(""__FUNCNAME__": in interrupt\r\n");
		r = -1;
		goto end0;
	}

	if (0 != _ubik_critcount) {
		logme(""__FUNCNAME__": in critical section\r\n");
		r = -1;
		goto end0;
	}

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
		r = -1;
		goto end0;
	}

	printf("------------------------------------------------------------------------------------------------------------------------\r\n");
	printf("|task    |address    |function   |stack size   |max stack    |priority   |state   |suspended |timed |name               \r\n");
	printf("|type    |           |address    |(byte(depth))|usage        |(original) |        |          |      |                   \r\n");
	printf("|        |           |           |             |(byte(depth))|           |        |          |      |                   \r\n");
	printf("------------------------------------------------------------------------------------------------------------------------\r\n");

	bsp_getstacksize(1, &stacksize);
	stacksizedepth = stacksize/INT_SIZE;
	if (0 != stacksize%INT_SIZE) {
		stacksizedepth++;
	}
	r2 = bsp_getmaxstackusage(1, &maxstackusage);
	maxstackusagedepth = maxstackusage/INT_SIZE;
	if (0 != maxstackusage%INT_SIZE) {
		maxstackusagedepth++;
	}
	if (0 != r2) {
		maxstackusage = 0;
		maxstackusagedepth = 0;
	}
	printf("|svc     |           |           | 0x%08x  | 0x%08x  |           |        |          |      |        \r\n", stacksize, maxstackusage);
	printf("|        |           |           |(0x%08x) |(0x%08x) |           |        |          |      |                   \r\n",
			stacksizedepth, maxstackusagedepth);

	task = _kernel_monitor_tasklist_head(&_kernel_monitor_tasklist);
	for (; NULL != task; task = _kernel_monitor_tasklist_next(task)) {
		stacksize = task->stacksize;
		stacksizedepth = stacksize/INT_SIZE;
		if (0 != stacksize%INT_SIZE) {
			stacksizedepth++;
		}
		r2 = task_getmaxstackusage((task_pt) task, &maxstackusage);
		maxstackusagedepth = maxstackusage/INT_SIZE;
		if (0 != maxstackusage%INT_SIZE) {
			maxstackusagedepth++;
		}
		if (0 != r2) {
			maxstackusage = 0;
			maxstackusagedepth = 0;
		}
		printf("|task    |0x%08x |0x%08x | 0x%08x  | 0x%08x  |%04d(%04d) |",
				(unsigned int) task, (unsigned int) (task->func), stacksize, maxstackusage, task->priority, task->priority_ori);
		switch (task->state) {
		case TASK_STATE__READY:
			printf("ready  ");
			break;
		case TASK_STATE__RUNNING:
			printf("running");
			break;
		case TASK_STATE__BLOCKED:
			printf("blocked");
			break;
		default:
			printf("unknown");
			break;
		}
		printf(" |%d         |%d     |%s",
				task->suspended, task->timed, task->name);
#if (1 != EXCLUDE_IDLETASK_HOOKFUNC)
		if (OBJTYPE__UBIK_IDLETASK  == task->type && NULL != _task_idletaskhookfunc_func) {
			printf(" (%s)", _task_idletaskhookfunc_name);
		}
#endif
		printf("\r\n");
		printf("|        |           |           |(0x%08x) |(0x%08x) |           |        |          |      |                   \r\n",
				stacksizedepth, maxstackusagedepth);
	}
	printf("------------------------------------------------------------------------------------------------------------------------\r\n");
	printf("\r\n");

	printf("------------------------------------------------------------------------------------------------------------------------\r\n");
	printf("|timer   |address    |semaphore  |period     |oneshot |running                                      \r\n");
	printf("|type    |           |address    |(tick)     |        |                                             \r\n");
	printf("------------------------------------------------------------------------------------------------------------------------\r\n");
	stimer = _kernel_monitor_stimerlist_head(&_kernel_monitor_stimerlist);
	for (; NULL != stimer; stimer = _kernel_monitor_stimerlist_next(stimer)) {
		printf("|stimer  |0x%08x |0x%08x |0x%08x |%d       |%d\r\n",
				(unsigned int) stimer, (unsigned int) (stimer->sigobj), stimer->tick, stimer->oneshot, stimer->running);
	}
	printf("------------------------------------------------------------------------------------------------------------------------\r\n");
	printf("\r\n");

	printf("------------------------------------------------------------------------------------------------------------------------\r\n");
	printf("|sigobj  |address    |waiting |max      |sender     |count      |max        |etc                    \r\n");
	printf("|type    |           |task    |priority |(owner)    |(size)     |count      |                       \r\n");
	printf("|        |           |count   |         |address    |           |(size)     |                       \r\n");
	printf("------------------------------------------------------------------------------------------------------------------------\r\n");
	sigobj = _kernel_monitor_sigobjlist_head(&_kernel_monitor_sigobjlist);
	for (; NULL != sigobj; sigobj = _kernel_monitor_sigobjlist_next(sigobj)) {
		switch (sigobj->type) {
		case OBJTYPE__UBIK_SIGNAL:
			printf("|signal  |0x%08x |%04d    |%04d     |0x%08x |           |           |\r\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj));
			break;
		case OBJTYPE__UBIK_CONDV:
			printf("|condv   |0x%08x |%04d    |%04d     |0x%08x |           |           |\r\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj));
			break;
		case OBJTYPE__UBIK_MUTEX:
			printf("|mutex   |0x%08x |%04d    |%04d     |0x%08x |0x%08x |           |n: %d\r\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->nopriorityinheritance);
			break;
		case OBJTYPE__UBIK_SEM:
			printf("|sem     |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |i: %d\r\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->maxcount, sigobj->ignoreoverflow);
			break;
		case OBJTYPE__UBIK_MSGQ:
			printf("|msgq    |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |msg size: 0x%04x\r\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->maxcount, sigobj->msgsize_1 + 1);
			break;
		default:
			printf("|unknown |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |type: 0x%04x\r\n",
					(unsigned int) sigobj, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
					sigobj->count, sigobj->maxcount, sigobj->type);
			break;
		}
	}

	#if (1 != EXCLUDE_IOBUF)
	iobuf = _kernel_monitor_iobuflist_head(&_kernel_monitor_iobuflist);
	for (; NULL != iobuf; iobuf = _kernel_monitor_iobuflist_next(iobuf)) {
		sigobj = (_sigobj_pt) (iobuf->condv);
		printf("|iobuf   |0x%08x |%04d    |%04d     |0x%08x |0x%08x |0x%08x |\r\n",
				(unsigned int) iobuf, sigobj->wtasklist.count, sigobj->wtasklist_maxpriority, (unsigned int) _task_osigobjlist_owner(sigobj),
				iobuf->size, iobuf->maxsize);
	}
	#endif /* (1 != EXCLUDE_IOBUF) */
	printf("------------------------------------------------------------------------------------------------------------------------\r\n");
	printf("\r\n");

	r = 0;

	r2 = mutex_unlock(_kernel_monitor_mutex);
	if (0 != r2) {
		logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
		r = -1;
	}

end0:
	return r;
	#undef __FUNCNAME__
#else
	return 0;
#endif
}

unsigned int _sigobj_wtasklist_getmaxpriority(_sigobj_pt sigobj) {
	_wtask_pt wtask;
	_task_pt task;
	int priority = 0;

	for (wtask = _sigobj_wtasklist_head(sigobj); NULL != wtask; wtask = _sigobj_wtasklist_next(wtask)) {
		task = wtask->task;
		if (0 == task->suspended && priority < task->priority) {
			priority = task->priority;
		}
	}

	return priority;
}

_wtask_pt _sigobj_wtasklist_gettask(_sigobj_pt sigobj) {
	_wtask_pt wtask;
	_task_pt task;
	int priority = 0;
	_wtask_pt mpwtask = NULL;

	for (wtask = _sigobj_wtasklist_head(sigobj); NULL != wtask; wtask = _sigobj_wtasklist_next(wtask)) {
		task = wtask->task;
		if (0 == task->suspended && (mpwtask == NULL || priority < task->priority)) {
			priority = task->priority;
			mpwtask = wtask;
		}
	}

	return mpwtask;
}

#if (1 != EXCLUDE_TASK_PRIORITY_INHERITANCE)
void _sigobj_wtasklist_notifychange(_sigobj_pt sigobj) {
	int priority;
	_task_pt task;

	priority = _sigobj_wtasklist_getmaxpriority(sigobj);
	if (sigobj->wtasklist_maxpriority != priority) {
		sigobj->wtasklist_maxpriority = priority;
		task = _task_osigobjlist_owner(sigobj);
		if (NULL != task) {
			_task_applypriority(task);
		}
	}
}
#endif

#if (1 != EXCLUDE_STIMER)
void _stimerlist_add(_stimer_pt stimer) {
	edlist_pt stimerlist;
	_stimer_pt ref;

	_stimerlist_remove(stimer);

	stimer->wakeuptick = _ubik_tickcount + stimer->tick;

	if (_ubik_tickcount >= stimer->wakeuptick) {
		stimerlist = _stimer_list_next;
	}
	else {
		stimerlist = _stimer_list_cur;
	}

	ref =  _stimerlist_head(stimerlist);
	for (;;) {
		if (NULL == ref || stimer->wakeuptick < ref->wakeuptick) {
			break;
		}
		ref = _stimerlist_next(ref);
	}

	_stimerlist_insertprev(stimerlist, ref,  stimer);
}
#endif

#if (1 != EXCLUDE_IDLETASK_HOOKFUNC)
int ubik_setidletaskhookfunc(idletaskhookfunc_ft func, void * arg, const char * name, unsigned int option) {
	ubik_entercrit();
	_task_idletaskhookfunc_func		= func;
	_task_idletaskhookfunc_arg		= arg;
	_task_idletaskhookfunc_option	= option;
	_task_idletaskhookfunc_name[0]	= 0x00;
	ubik_exitcrit();

	if (NULL != name) {
		strncpy(_task_idletaskhookfunc_name, name, TASK_NAME_SIZE_MAX);
	}

	return 0;
}
#endif

#if (1 != EXCLUDE_TICK_HOOKFUNC)
int ubik_settickhookfunc(tickhookfunc_ft tickhookfunc) {
	ubik_entercrit();
	_ubik_tickhookfunc_func			= tickhookfunc;
	ubik_exitcrit();

	return 0;
}
#endif

int ubik_setrtpriority(int rtpriority) {
	if (0 >= rtpriority) {
		return -2;
	}
	
	_ubik_rtpriority = rtpriority;

	return 0;
}

int ubik_getrtpriority(void) {
	return _ubik_rtpriority;
}

#endif /* (!defined(UBI_COMCONFIG__lib_ubik__dummy)) */
