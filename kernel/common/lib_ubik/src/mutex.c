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

#include "itf_ubinos/itf/ubiclib_logm.h"
#include "itf_ubinos/itf/ubiclib_heap.h"
#include "itf_ubinos/itf/ubik_mutex.h"

#include "_ubik.h"

int mutex_create(mutex_pt * mutex_p) {
	return mutex_create_ext(mutex_p, 0);
}

int mutex_create_ext(mutex_pt * mutex_p, unsigned int option) {
	#define	__FUNCNAME__	"mutex_create_ext"
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

	if (NULL == mutex_p) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end0;
	}

	r = _sigobj_create(&sigobj);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at _sigobj_create()\r\n");
		goto end1;
	}

	if (0 != (option & MUTEX_OPT__NOPRIORITYINHERITANCE)) {
		sigobj->nopriorityinheritance = 1;
	}

	sigobj->type		= OBJTYPE__UBIK_MUTEX;
	sigobj->valid 		= 1;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	_kernel_monitor_sigobjlist_insertnext(&_kernel_monitor_sigobjlist, NULL, sigobj);
	#endif

	*mutex_p = (mutex_pt) sigobj;

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

int mutex_delete(mutex_pt * mutex_p) {
	return _sigobj_delete((_sigobj_pt *) mutex_p);
}

int mutex_lock(mutex_pt _mutex) {
	#define	__FUNCNAME__	"mutex_lock"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _mutex;
	_task_pt task = _task_cur;

	if (0 == _ubik_active) {
		r = 0;
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
			(	OBJTYPE__UBIK_MUTEX != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (0 == sigobj->count) {
		if (0 == sigobj->nopriorityinheritance) {
			r = _sigobj_setsender(sigobj, task);
			if (0 != r) {
				logme(""__FUNCNAME__": fail at _sigobj_setsender()\r\n");
				r = -1;
				goto end1;
			}
		}
		sigobj->count++;
		r = 0;
	}
	else {
		if (task == _task_osigobjlist_owner(sigobj)) {
			if (sigobj->maxcount == sigobj->count) {
				logme(""__FUNCNAME__": count is over max count\r\n");
				r = UBIK_ERR__OVERFLOWED;
			}
			else {
				sigobj->count++;
				r = 0;
			}
		}
		else {
			if (0 == task->sysflag01) {
				r = _sigobj_wait(sigobj, task->wtask_p);
				if (SIGOBJ_SIGTYPE__TIMEOUT != r && 0 != r) {
					logme(""__FUNCNAME__": fail at _sigobj_wait()\r\n");
				}
			}
			else {
				r = SIGOBJ_SIGTYPE__TIMEOUT;
			}
		}
	}

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int mutex_lock_timed(mutex_pt _mutex, unsigned int tick) {
	#define	__FUNCNAME__	"mutex_lock_timed"
	int r;

	if (0 == _ubik_active) {
		r = 0;
		goto end0;
	}

	if (0 != _ubik_intrcount) {
		logme(""__FUNCNAME__": in interrupt\r\n");
		r = -1;
		goto end0;
	}

	_task_cur->timed		= 1;
	_task_cur->wakeuptick 	= tick;

	r = mutex_lock(_mutex);

	_task_cur->timed		= 0;

end0:
	return r;
	#undef __FUNCNAME__
}

int mutex_lock_timedms(mutex_pt _mutex, unsigned int timems) {
	return mutex_lock_timed(_mutex, ubik_timemstotick(timems));
}

int mutex_unlock(mutex_pt _mutex) {
	#define	__FUNCNAME__	"mutex_unlock"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _mutex;
	_task_pt otask;
	_wtask_pt wtask;

	if (0 == _ubik_active) {
		r = 0;
		goto end0;
	}

	if (0 != _ubik_intrcount) {
		logme(""__FUNCNAME__": in interrupt\r\n");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MUTEX != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	otask = _task_osigobjlist_owner(sigobj);
	if (NULL != otask && _task_cur != otask) {
		logme(""__FUNCNAME__": current task is not owner\r\n");
		r = -1;
		goto end1;
	}

	if (0 == sigobj->count) {
		logme(""__FUNCNAME__": not locked\r\n");
		r = -1;
		goto end1;
	}

	if (1 < sigobj->count) {
		sigobj->count--;
		r = 0;
	}
	else {
		wtask = _sigobj_wtasklist_gettask(sigobj);
		if (NULL == wtask) {
			if (0 == sigobj->nopriorityinheritance) {
				r = _sigobj_setsender(sigobj, NULL);
				if (0 != r) {
					logme(""__FUNCNAME__": fail at _sigobj_setsender()\r\n");
					r = -1;
					goto end1;
				}
			}
			sigobj->count--;
			r = 0;
		}
		else {
			if (0 == sigobj->nopriorityinheritance) {
				r = _sigobj_setsender(sigobj, wtask->task);
				if (0 != r) {
					logme(""__FUNCNAME__": fail at _sigobj_setsender()\r\n");
					r = -1;
					goto end1;
				}
			}
			r = _sigobj_send(sigobj, SIGOBJ_SIGTYPE__SUCCESS);
			if (0 != r) {
				logme(""__FUNCNAME__": fail at _sigobj_send()\r\n");
			}
		}
	}

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int mutex_islocked(mutex_pt _mutex) {
	#define	__FUNCNAME__	"mutex_islocked"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _mutex;

	if (0 == _ubik_active) {
		r = 0;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MUTEX != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (0 != sigobj->count) {
		r = 1;
	}
	else {
		r = 0;
	}

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

#endif
