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

#if (1 != EXCLUDE_SIGNAL)

#if (!defined(UBI_COMCONFIG__lib_ubik__dummy))

#include "itf_ubinos/itf/ubiclib_logm.h"
#include "itf_ubinos/itf/ubiclib_heap.h"
#include "itf_ubinos/itf/ubik_signal.h"

#include "_ubik.h"

int signal_create(signal_pt * signal_p) {
	return signal_create_ext(signal_p, 0);
}

int signal_create_ext(signal_pt * signal_p, unsigned int option) {
	#define	__FUNCNAME__	"signal_create_ext"
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

	r = _sigobj_create(&sigobj);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at _sigobj_create()\r\n");
		goto end1;
	}

	sigobj->type		= OBJTYPE__UBIK_SIGNAL;
	sigobj->valid 		= 1;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	_kernel_monitor_sigobjlist_insertnext(&_kernel_monitor_sigobjlist, NULL, sigobj);
	#endif

	*signal_p = (signal_pt) sigobj;

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

int signal_delete(signal_pt * signal_p) {
	return _sigobj_delete((_sigobj_pt *) signal_p);
}

int signal_wait(signal_pt _signal) {
	#define	__FUNCNAME__	"signal_wait"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _signal;
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
			(	OBJTYPE__UBIK_SIGNAL != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (0 == task->sysflag01) {
		r = _sigobj_wait(sigobj, task->wtask_p);
	}
	else {
		r = SIGOBJ_SIGTYPE__TIMEOUT;
	}

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int signal_wait_timed(signal_pt _signal, unsigned int tick) {
	#define	__FUNCNAME__	"signal_wait_timed"
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

	r = signal_wait(_signal);

	_task_cur->timed		= 0;

end0:
	return r;
	#undef __FUNCNAME__
}

int signal_wait_timedms(signal_pt _signal, unsigned int timems) {
	return signal_wait_timed(_signal, ubik_timemstotick(timems));
}

int signal_send(signal_pt _signal, int sigtype) {
	#define	__FUNCNAME__	"signal_send"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _signal;
	_task_pt otask;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	if (0 > sigtype) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SIGNAL != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	otask = _task_osigobjlist_owner(sigobj);
	if (	(0 == _ubik_intrcount && NULL != otask && _task_cur != otask)	||
			(0 != _ubik_intrcount && NULL != otask						)		)
	{
		logme(""__FUNCNAME__": current task is not an appropriate sender\r\n");
		r = -1;
		goto end1;
	}

	r = _sigobj_send(sigobj, sigtype);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at _sigobj_send()\r\n");
	}

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int signal_broadcast(signal_pt _signal, int sigtype) {
	#define	__FUNCNAME__	"signal_broadcast"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _signal;
	_task_pt otask;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	if (0 > sigtype) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SIGNAL != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	otask = _task_osigobjlist_owner(sigobj);
	if (	(0 == _ubik_intrcount && NULL != otask && _task_cur != otask)	||
			(0 != _ubik_intrcount && NULL != otask						)		)
	{
		logme(""__FUNCNAME__": current task is not an appropriate sender\r\n");
		r = -1;
		goto end1;
	}

	r = _sigobj_broadcast(sigobj, sigtype);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at _sigobj_broadcast()\r\n");
	}

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int signal_setsender(signal_pt _signal, task_pt _task) {
	#define	__FUNCNAME__	"signal_setsender"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _signal;
	_task_pt task = (_task_pt) _task;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_SIGNAL != sigobj->type	)		)
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

#endif /* (1 != EXCLUDE_SIGNAL) */
