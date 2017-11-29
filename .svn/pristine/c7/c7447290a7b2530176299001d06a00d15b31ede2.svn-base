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

#if (1 != EXCLUDE_CONDV)

#if (!defined(UBI_COMCONFIG__lib_ubik__dummy))

#include "itf_ubinos/itf/ubiclib_logm.h"
#include "itf_ubinos/itf/ubiclib_heap.h"
#include "itf_ubinos/itf/ubik_mutex.h"
#include "itf_ubinos/itf/ubik_condv.h"

#include "_ubik.h"

int condv_create(condv_pt * condv_p) {
	return condv_create_ext(condv_p, 0);
}

int condv_create_ext(condv_pt * condv_p, unsigned int option) {
	#define	__FUNCNAME__	"condv_create_ext"
	int r;
	#if (1 != EXCLUDE_KERNEL_MONITORING)
	int r2;
	#endif
	_sigobj_pt sigobj;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
		r = -1;
		goto end0;
	}
	#endif

	if (NULL == condv_p) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end0;
	}

	r = _sigobj_create(&sigobj);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at _sigobj_create()\r\n");
		goto end1;
	}

	sigobj->type		= OBJTYPE__UBIK_CONDV;
	sigobj->valid 		= 1;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	_kernel_monitor_sigobjlist_insertnext(&_kernel_monitor_sigobjlist, NULL, sigobj);
	#endif

	*condv_p = (condv_pt) sigobj;

end1:
	#if (1 != EXCLUDE_KERNEL_MONITORING)
	r2 = mutex_unlock(_kernel_monitor_mutex);
	if (0 != r2) {
		logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
		r = -1;
	}

end0:
	#endif

	return r;
	#undef __FUNCNAME__
}

int condv_delete(condv_pt * condv_p) {
	return _sigobj_delete((_sigobj_pt *) condv_p);
}

int condv_wait(condv_pt _condv, mutex_pt _mutex) {
	#define	__FUNCNAME__	"condv_wait"
	int r;
	int r2;
	int needlock = 0;
	unsigned int task_timed_old;
	_sigobj_pt sigobj = (_sigobj_pt) _condv;
	_sigobj_pt mutex = (_sigobj_pt) _mutex;
	_task_pt otask;
	_task_pt task = _task_cur;

	if (0 == _ubik_active) {
		logme(""__FUNCNAME__": ubik is not active\r\n");
		r = -1;
		goto end0;
	}

	if (0 == task->sysflag01) {
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

		if (0 != _ubik_tasklockcount) {
			logme(""__FUNCNAME__": task is locked\r\n");
			r = -1;
			goto end0;
		}
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_CONDV != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (	(	NULL == mutex							)	||
			(	0 == mutex->valid						)	||
			(	OBJTYPE__UBIK_MUTEX != mutex->type		)	||
			(	1 != mutex->count						)		)
	{
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end1;
	}

	otask = _task_osigobjlist_owner(mutex);
	if (NULL != otask && task != otask) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end1;
	}

	if (0 == task->sysflag01) {
		if (0 == _ubik_tasklockcount) {
			_ubik_tasklockcount++;
			r = mutex_unlock((mutex_pt) mutex);
			_ubik_tasklockcount--;
		}
		else {
			r = mutex_unlock((mutex_pt) mutex);
		}
		if (0 != r) {
			logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
			r = -3;
			goto end1;
		}
		needlock = 1;

		task->wtask_p->mutex = (mutex_pt) mutex;
		r = _sigobj_wait(sigobj, task->wtask_p);
		if (SIGOBJ_SIGTYPE__TIMEOUT != r && 0 != r) {
			logme(""__FUNCNAME__": fail at _sigobj_wait()\r\n");
		}
		task->wtask_p->mutex = NULL;
	}
	else {
		r = SIGOBJ_SIGTYPE__TIMEOUT;
	}

end1:
	ubik_exitcrit();

	if (0 != needlock) {
		task_timed_old = task->timed;
		task->timed = 0;
		r2 = mutex_lock((mutex_pt) mutex);
		if (0 != r2) {
			logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
			r = -3;
		}
		task->timed = task_timed_old;
	}

end0:
	return r;
	#undef __FUNCNAME__
}

int condv_wait_timed(condv_pt _condv, mutex_pt _mutex, unsigned int tick) {
	#define	__FUNCNAME__	"condv_wait_timed"
	int r;

	if (0 == _ubik_active) {
		logme(""__FUNCNAME__": ubik is not active\r\n");
		r = -1;
		goto end0;
	}

	if (0 != _ubik_intrcount) {
		logme(""__FUNCNAME__": in interrupt\r\n");
		r = -1;
		goto end0;
	}

	_task_cur->timed		= 1;
	_task_cur->wakeuptick 	= tick;

	r = condv_wait(_condv, _mutex);

	_task_cur->timed		= 0;

end0:
	return r;
	#undef __FUNCNAME__
}

int condv_wait_timedms(condv_pt _condv, mutex_pt mutex, unsigned int timems) {
	return condv_wait_timed(_condv, mutex, ubik_timemstotick(timems));
}

int condv_signal(condv_pt _condv) {
	#define	__FUNCNAME__	"condv_signal"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _condv;
	_task_pt otask;

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

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_CONDV != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	otask = _task_osigobjlist_owner(sigobj);
	if (NULL != otask && _task_cur != otask) {
		logme(""__FUNCNAME__": current task is not an appropriate sender\r\n");
		r = -1;
		goto end1;
	}

	r = _sigobj_send(sigobj, SIGOBJ_SIGTYPE__SUCCESS);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at _sigobj_send()\r\n");
	}

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int condv_broadcast(condv_pt _condv) {
	#define	__FUNCNAME__	"condv_broadcast"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _condv;
	_task_pt otask;

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

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_CONDV != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	otask = _task_osigobjlist_owner(sigobj);
	if (NULL != otask && _task_cur != otask) {
		logme(""__FUNCNAME__": current task is not an appropriate sender\r\n");
		r = -1;
		goto end1;
	}

	r = _sigobj_broadcast(sigobj, SIGOBJ_SIGTYPE__SUCCESS);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at _sigobj_broadcast()\r\n");
	}

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int condv_setsender(condv_pt _condv, task_pt _task) {
	#define	__FUNCNAME__	"condv_setsender"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _condv;
	_task_pt task = (_task_pt) _task;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_CONDV != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	r = _sigobj_setsender(sigobj, task);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at _sigobj_setsender()\r\n");
	}

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

#endif

#endif /* (1 != EXCLUDE_CONDV) */
