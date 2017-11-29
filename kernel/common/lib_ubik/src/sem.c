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

#if (1 != EXCLUDE_SEM)

#if (!defined(UBI_COMCONFIG__lib_ubik__dummy))

#include "itf_ubinos/itf/ubiclib_logm.h"
#include "itf_ubinos/itf/ubiclib_heap.h"
#include "itf_ubinos/itf/ubik_sem.h"

#include "_ubik.h"

int sem_create(sem_pt * sem_p) {
	return sem_create_ext(sem_p, 0, UINT_MAX, 0);
}

int semb_create(sem_pt * sem_p) {
	return sem_create_ext(sem_p, 0, 1, SEM_OPT__IGNOREOVERFLOW);
}

int sem_create_ext(sem_pt * sem_p, unsigned int initcount, unsigned int maxcount, unsigned int option) {
	#define	__FUNCNAME__	"sem_create_ext"
	int r;
	#if (1 != EXCLUDE_KERNEL_MONITORING)
	int r2;
	#endif
	_sigobj_pt sigobj;

	if (initcount > maxcount) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	if (0 >= maxcount) {
		logme(""__FUNCNAME__": parameter 3 is wrong\r\n");
		r = -4;
		goto end0;
	}

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
		r = -1;
		goto end0;
	}
	#endif

	if (NULL == sem_p) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end0;
	}

	r = _sigobj_create(&sigobj);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at _sigobj_create()\r\n");
		goto end1;
	}

	sigobj->count 		= initcount;
	sigobj->maxcount 	= maxcount;

	if (0 != (option & SEM_OPT__IGNOREOVERFLOW)) {
		sigobj->ignoreoverflow = 1;
	}

	sigobj->type		= OBJTYPE__UBIK_SEM;
	sigobj->valid 		= 1;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	_kernel_monitor_sigobjlist_insertnext(&_kernel_monitor_sigobjlist, NULL, sigobj);
	#endif

	*sem_p = (sem_pt) sigobj;

end1:
	#if (1 != EXCLUDE_KERNEL_MONITORING)
	r2 = mutex_unlock(_kernel_monitor_mutex);
	if (0 != r2) {
		logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
		r = -1;
	}
	#endif

end0:
	return r;
	#undef __FUNCNAME__
}

int sem_delete(sem_pt * sem_p) {
	return _sigobj_delete((_sigobj_pt *) sem_p);
}

int sem_take(sem_pt _sem) {
	#define	__FUNCNAME__	"sem_take"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _sem;
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
			(	OBJTYPE__UBIK_SEM != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (0 < sigobj->count) {
		sigobj->count--;
		r = 0;
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

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int sem_take_timed(sem_pt _sem, unsigned int tick) {
	#define	__FUNCNAME__	"sem_take_timed"
	int r;
	unsigned int sysflag01_old = 0;

	if (0 == _ubik_active) {
		logme(""__FUNCNAME__": ubik is not active\r\n");
		r = -1;
		goto end0;
	}

	if (0 != _ubik_intrcount) {
		if (0 == tick) {
			sysflag01_old = _task_cur->sysflag01;
			_task_cur->sysflag01 = 1;
		}
		else {
			logme(""__FUNCNAME__": in interrupt\r\n");
			r = -1;
			goto end0;
		}
	}
	else {
		_task_cur->timed		= 1;
		_task_cur->wakeuptick 	= tick;
	}

	r = sem_take(_sem);

	if (0 != _ubik_intrcount) {
		 _task_cur->sysflag01 = sysflag01_old;
	}
	else {
		_task_cur->timed		= 0;
	}

end0:
	return r;
	#undef __FUNCNAME__
}

int sem_take_timedms(sem_pt _sem, unsigned int timems) {
	return sem_take_timed(_sem, ubik_timemstotick(timems));
}

int sem_give(sem_pt _sem) {
	#define	__FUNCNAME__	"sem_give"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _sem;
	_task_pt otask;
	_wtask_pt wtask;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SEM != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	otask = _task_osigobjlist_owner(sigobj);
	if (	(0 == _ubik_intrcount && NULL != otask && _task_cur != otask)	||
			(0 != _ubik_intrcount && NULL != otask						)		)
	{
		logme(""__FUNCNAME__": current task is not an appropriate giver\r\n");
		r = -1;
		goto end1;
	}

	if (sigobj->maxcount == sigobj->count) {
		if (0 == sigobj->ignoreoverflow) {
			logme(""__FUNCNAME__": count is over max count\r\n");
			r  = UBIK_ERR__OVERFLOWED;
		}
		else {
			r = 0;
		}
	}
	else {
		wtask = _sigobj_wtasklist_gettask(sigobj);
		if (NULL == wtask) {
			sigobj->count++;
			r = 0;
		}
		else {
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

int sem_clear(sem_pt _sem) {
	#define	__FUNCNAME__	"sem_clear"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _sem;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SEM != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	sigobj->count = 0;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int sem_setsender(sem_pt _sem, task_pt _task) {
	#define	__FUNCNAME__	"sem_setsender"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _sem;
	_task_pt task = (_task_pt) _task;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SEM != sigobj->type	)		)
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

int sem_getcount(sem_pt _sem, unsigned int * count_p) {
	#define	__FUNCNAME__	"sem_getcount"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _sem;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	if (NULL == count_p) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SEM != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	*count_p = sigobj->count;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

#endif

#endif /* (1 != EXCLUDE_SEM) */
