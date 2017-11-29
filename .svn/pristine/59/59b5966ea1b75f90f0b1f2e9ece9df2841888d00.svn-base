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
#include <stdlib.h>
#include <string.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubiclib_logm.h"
#include "itf_ubinos/itf/ubiclib_heap.h"
#include "itf_ubinos/itf/ubiclib_cirbuf.h"

#include "_ubik.h"

int _sigobj_create(_sigobj_pt * sigobj_p) {
	#define	__FUNCNAME__	"_sigobj_create"
	int r;
	_sigobj_pt sigobj;

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

	if (NULL == sigobj_p) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end0;
	}

	sigobj = malloc(sizeof(_sigobj_t));
	if (NULL == sigobj) {
		logme(""__FUNCNAME__": fail at malloc()\r\n");
		r = -1;
		goto end0;
	}

	sigobj->type					= 0;
	sigobj->valid					= 0;
	sigobj->reserved1				= 0;

	sigobj->reserved2				= 0;
	sigobj->nopriorityinheritance	= 0;
	sigobj->ignoreoverflow			= 0;
	sigobj->msgsize_1				= 0;
	sigobj->wtasklist_maxpriority	= 0;

	edlist_init(&sigobj->wtasklist);
	sigobj->wtasklist.data			= sigobj;

	edlist_link_init(&sigobj->osigobjlist_link);

	sigobj->count					= 0;
	sigobj->maxcount				= UINT_MAX;
	sigobj->msgbuf					= NULL;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	edlist_link_init(&sigobj->kernel_monitor_sigobjlist_link);
	#endif

	*sigobj_p = sigobj;

	r = 0;

end0:
	return r;
	#undef __FUNCNAME__
}

int _sigobj_delete(_sigobj_pt * sigobj_p) {
	#define	__FUNCNAME__	"_sigobj_delete"
	int r;
	int r2;
	_sigobj_pt sigobj;
	cirbuf_pt msgbuf = NULL;
	void * sigobjbuf = NULL;
	_task_pt owner;

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

	if (NULL == sigobj_p) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end0;
	}

	sigobj = *sigobj_p;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
		r = -1;
		goto end0;
	}
	#endif

	ubik_entercrit();

	if (	(	NULL == sigobj								)	||
			(	0 == sigobj->valid							) 	||
			(	OBJTYPE__UBIK_SIGNAL	!= sigobj->type	&&
				OBJTYPE__UBIK_CONDV 	!= sigobj->type &&
				OBJTYPE__UBIK_MUTEX 	!= sigobj->type	&&
				OBJTYPE__UBIK_SEM 		!= sigobj->type	&&
				OBJTYPE__UBIK_MSGQ 		!= sigobj->type		)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	owner = _task_osigobjlist_owner(sigobj);
	if (NULL != owner) {
		if (0 != _ubik_active && 0 == _ubik_intrcount && owner == _task_cur) {
			r = _sigobj_setsender(sigobj, NULL);
			if (0 != r) {
				logme(""__FUNCNAME__": fail at _sigobj_setsender()\r\n");
				r = -1;
				goto end1;
			}
		}
		else {
			logme(""__FUNCNAME__": still owned\r\n");
			r = -2;
			goto end1;
		}
	}

	if (0 == _ubik_tasklockcount) {
		_ubik_tasklockcount++;
		r = _sigobj_broadcast(sigobj, SIGOBJ_SIGTYPE__TERMINATED);
		_ubik_tasklockcount--;
	}
	else {
		r = _sigobj_broadcast(sigobj, SIGOBJ_SIGTYPE__TERMINATED);
	}
	if (0 != r) {
		logme(""__FUNCNAME__": fail at _sigobj_broadcast()\r\n");
		r = -2;
		goto end1;
	}

	msgbuf 			= sigobj->msgbuf;
	sigobjbuf 		= sigobj;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	_kernel_monitor_sigobjlist_remove(sigobj);
	#endif

	sigobj->valid 	= 0;

	*sigobj_p 		= NULL;

	_task_schedule();

	r = 0;

end1:
	ubik_exitcrit();

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	r2 = mutex_unlock(_kernel_monitor_mutex);
	if (0 != r2) {
		logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
		r = -1;
	}
	#endif

	if (NULL != msgbuf) {
		r2 = cirbuf_delete(&msgbuf);
		if (0 != r2) {
			logme(""__FUNCNAME__": fail at cirbuf_delete()\r\n");
			r = -1;
		}
	}

	if (NULL != sigobjbuf) {
		free(sigobjbuf);
	}

end0:
	return r;
	#undef __FUNCNAME__
}

int _sigobj_wait(_sigobj_pt sigobj, _wtask_pt wtask) {
	#define	__FUNCNAME__	"_sigobj_wait"
	int r;
	unsigned int critcount;
	unsigned int wakeuptick;
	unsigned int spintick;

	if (0 >= _ubik_critcount) {
		logme(""__FUNCNAME__": not in critical section\r\n");
		bsp_abortsystem();
	}

	if (0 != _task_cur->timed) {
		if (0 == _task_cur->wakeuptick) {
			r = SIGOBJ_SIGTYPE__TIMEOUT;
			goto end0;
		}
		wakeuptick = _task_cur->wakeuptick;
		_task_cur->wakeuptick += _ubik_tickcount;
	}
	else {
		wakeuptick = 0;
	}

	_task_cur->wtask_count		= 1;
	_task_cur->wtask_waitcount	= 1;
	_task_cur->wtask_recvcount 	= 0;

	wtask->sigtype = SIGOBJ_SIGTYPE__FAIL;

	_sigobj_wtasklist_insertprev(sigobj, NULL, wtask);
	_sigobj_wtasklist_notifychange(sigobj);

	if (OBJTYPE__UBIK_IDLETASK  == _task_cur->type) {
		for (;;) {
			if (0 != _task_cur->timed) {
				if (wakeuptick <= 0) {
					wtask->sigtype = SIGOBJ_SIGTYPE__TIMEOUT;
					_task_cur->wtask_recvcount++;
					_sigobj_wtasklist_remove(wtask);
					_sigobj_wtasklist_notifychange(sigobj);

					_task_cur->wakeuptick = 0;
					break;
				}
				spintick = min(IDLETASK_SPINWAIT_INTERVALTICK, wakeuptick);
			}
			else {
				spintick = IDLETASK_SPINWAIT_INTERVALTICK;
			}

			critcount = _ubik_critcount;
			_ubik_critcount = 1;
			ubik_exitcrit();
			bsp_busywait(bsp_getbusywaitcountperms() * ubik_ticktotimems(spintick));
			ubik_entercrit();
			_ubik_critcount = critcount;

			if (0 != _task_cur->timed) {
				wakeuptick -= spintick;
			}

			if (SIGOBJ_SIGTYPE__FAIL != wtask->sigtype) {
				_task_cur->wakeuptick = wakeuptick;
				break;
			}
		}
	}
	else {
		_task_cur->state =  TASK_STATE__BLOCKED;
		_task_changelist(_task_cur);

		_task_yield();
	}

	_task_cur->wtask_count		= 0;

	r = wtask->sigtype;

end0:
	return r;
	#undef __FUNCNAME__
}

int _sigobj_send(_sigobj_pt sigobj, int sigtype) {
	#define	__FUNCNAME__	"_sigobj_send"
	_wtask_pt wtask;
	_task_pt task;

	wtask = _sigobj_wtasklist_gettask(sigobj);
	if (NULL != wtask) {
		task = wtask->task;

		/////////////
		wtask->sigtype = sigtype;
		task->wtask_recvcount++;
		_sigobj_wtasklist_remove(wtask);

		if (task->wtask_waitcount == task->wtask_recvcount) {
			task->state = TASK_STATE__READY;
			_task_calcremainingtimeout(task);
			_task_changelist(task);
		}
		else if (0 == task->waitall) {
			_task_sigobj_removewtask(task);

			task->state = TASK_STATE__READY;
			_task_calcremainingtimeout(task);
			_task_changelist(task);
		}
		////////////////

		_sigobj_wtasklist_notifychange(sigobj);

		_task_schedule();
	}

	return 0;
	#undef __FUNCNAME__
}

int _sigobj_broadcast(_sigobj_pt sigobj, int sigtype) {
	#define	__FUNCNAME__	"_sigobj_broadcast"
	_wtask_pt wtask;
	_wtask_pt wtask_next;
	_task_pt task;

	wtask = _sigobj_wtasklist_head(sigobj);
	if (NULL != wtask) {
		for (;;) {
			wtask_next = _sigobj_wtasklist_next(wtask);

			task = wtask->task;
			if (0 == task->suspended || SIGOBJ_SIGTYPE__TERMINATED == sigtype) {

				/////////////
				wtask->sigtype = sigtype;
				task->wtask_recvcount++;
				_sigobj_wtasklist_remove(wtask);

				if (task->wtask_waitcount == task->wtask_recvcount) {
					task->state = TASK_STATE__READY;
					_task_calcremainingtimeout(task);
					_task_changelist(task);
				}
				else if (0 == task->waitall) {
					_task_sigobj_removewtask(task);

					task->state = TASK_STATE__READY;
					_task_calcremainingtimeout(task);
					_task_changelist(task);
				}
				////////////////

			}

			wtask = wtask_next;
			if (NULL == wtask) {
				break;
			}
		}

		_sigobj_wtasklist_notifychange(sigobj);

		_task_schedule();
	}

	return 0;
	#undef __FUNCNAME__
}

int _sigobj_setsender(_sigobj_pt sigobj, _task_pt task) {
	#define	__FUNCNAME__	"_sigobj_setsender"
	_task_pt task_old;

	task_old = _task_osigobjlist_owner(sigobj);
	if (task_old != task) {
		if (NULL != task_old) {
			_task_osigobjlist_remove(sigobj);
			_task_applypriority(task_old);
		}
		if (NULL != task) {
			_task_osigobjlist_insertprev(task, NULL, sigobj);
			_task_applypriority(task);
		}
	}

	return 0;
	#undef __FUNCNAME__
}

#endif
