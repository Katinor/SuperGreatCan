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

#if (1 != EXCLUDE_STIMER)

#if (!defined(UBI_COMCONFIG__lib_ubik__dummy))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "itf_ubinos/itf/ubiclib_logm.h"
#include "itf_ubinos/itf/ubiclib_heap.h"
#include "itf_ubinos/itf/ubik_sem.h"
#include "itf_ubinos/itf/ubik_stimer.h"

#include "_ubik.h"

int stimer_create(stimer_pt * stimer_p) {
	#define	__FUNCNAME__	"stimer_create"
	int r;
	#if (1 != EXCLUDE_KERNEL_MONITORING)
	int r2;
	#endif
	_stimer_pt stimer;

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

	if (NULL == stimer_p) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
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

	stimer = malloc(sizeof(_stimer_t));
	if (NULL == stimer) {
		logme(""__FUNCNAME__": fail at malloc()\r\n");
		r = -1;
		goto end1;
	}

	stimer->type					= OBJTYPE__UBIK_STIMER;
	stimer->valid					= 0;
	stimer->reserved1				= 0;

	stimer->reserved2				= 0;
	stimer->broadcast				= 0;
	stimer->oneshot					= 0;
	stimer->running					= 0;

	edlist_link_init(&stimer->stimerlist_link);

	stimer->tick					= 0;
	stimer->wakeuptick				= 0;

	stimer->sigobj					= NULL;
	stimer->sigtype					= 0;

	stimer->valid					= 1;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	edlist_link_init(&stimer->kernel_monitor_stimerlist_link);
	_kernel_monitor_stimerlist_insertnext(&_kernel_monitor_stimerlist, NULL, stimer);
	#endif

	*stimer_p = (stimer_pt) stimer;

	r = 0;

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

int stimer_delete(stimer_pt * stimer_p) {
	#define	__FUNCNAME__	"stimer_delete"
	int r;
	#if (1 != EXCLUDE_KERNEL_MONITORING)
	int r2;
	#endif
	_stimer_pt stimer;
	void * stimerbuf = NULL;

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

	if (NULL == stimer_p) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end0;
	}

	stimer = (_stimer_pt) (*stimer_p);

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
		r = -1;
		goto end0;
	}
	#endif

	ubik_entercrit();

	if (	(	NULL == stimer								)	||
			(	0 == stimer->valid							) 	||
			(	OBJTYPE__UBIK_STIMER		!= stimer->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (0 != stimer->running) {
		logme(""__FUNCNAME__": stimer is running\r\n");
		r = -2;
		goto end1;
	}

	stimerbuf 		= stimer;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	_kernel_monitor_stimerlist_remove(stimer);
	#endif

	stimer->valid 	= 0;

	*stimer_p 		= NULL;

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

	if (NULL != stimerbuf) {
		free(stimerbuf);
	}

end0:
	return r;
	#undef __FUNCNAME__
}

int stimer_set(stimer_pt _stimer, unsigned int tick, sem_pt sem, unsigned int option) {
	#define	__FUNCNAME__	"stimer_set"
	int r;
	_stimer_pt stimer = (_stimer_pt) _stimer;
	_sigobj_pt sigobj = (_sigobj_pt) sem;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	if (0 >= tick) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == stimer							)	||
			(	0 == stimer->valid						)	||
			(	OBJTYPE__UBIK_STIMER != stimer->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (0 != stimer->running) {
		logme(""__FUNCNAME__": stimer is running\r\n");
		r = -2;
		goto end1;
	}

	if (	(	NULL == sigobj								)	||
			(	0 == sigobj->valid							) 	||
			(	OBJTYPE__UBIK_SEM 		!= sigobj->type		)		)
	{
		logme(""__FUNCNAME__": parameter 3 is wrong\r\n");
		r = -4;
		goto end1;
	}

	if (0 != (option & STIMER_OPT__ONESHOT)) {
		stimer->oneshot	= 1;
	}
	else {
		stimer->oneshot	= 0;
	}

	stimer->tick		= tick;

	stimer->sigobj		= sigobj;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int stimer_setms(stimer_pt _stimer, unsigned int timems, sem_pt sem, unsigned int option) {
	return stimer_set(_stimer, ubik_timemstotick(timems), sem, option);
}

int stimer_set_signal(stimer_pt _stimer, unsigned int tick, signal_pt signal, int sigtype, unsigned int option) {
	#define	__FUNCNAME__	"stimer_set_signal"
	int r;
	_stimer_pt stimer = (_stimer_pt) _stimer;
	_sigobj_pt sigobj = (_sigobj_pt) signal;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	if (0 >= tick) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	if (0 > sigtype) {
		logme(""__FUNCNAME__": parameter 4 is wrong\r\n");
		r = -5;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == stimer							)	||
			(	0 == stimer->valid						)	||
			(	OBJTYPE__UBIK_STIMER != stimer->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (0 != stimer->running) {
		logme(""__FUNCNAME__": stimer is running\r\n");
		r = -2;
		goto end1;
	}

	if (	(	NULL == sigobj								)	||
			(	0 == sigobj->valid							) 	||
			(	OBJTYPE__UBIK_SIGNAL 	!= sigobj->type		)		)
	{
		logme(""__FUNCNAME__": parameter 3 is wrong\r\n");
		r = -4;
		goto end1;
	}

	if (0 != (option & STIMER_OPT__ONESHOT)) {
		stimer->oneshot	= 1;
	}
	else {
		stimer->oneshot	= 0;
	}

	if (0 != (option & STIMER_OPT__BROADCAST)) {
		stimer->broadcast	= 1;
	}
	else {
		stimer->broadcast	= 0;
	}

	stimer->tick		= tick;

	stimer->sigobj		= sigobj;

	stimer->sigtype		= sigtype;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int stimer_setms_signal(stimer_pt _stimer, unsigned int tick, signal_pt signal, int sigtype, unsigned int option) {
	return stimer_set_signal(_stimer, ubik_timemstotick(tick), signal, sigtype, option);
}

int stimer_start(stimer_pt _stimer) {
	#define	__FUNCNAME__	"stimer_start"
	int r;
	_stimer_pt stimer = (_stimer_pt) _stimer;
	_sigobj_pt sigobj;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == stimer							)	||
			(	0 == stimer->valid						)	||
			(	OBJTYPE__UBIK_STIMER 	!= stimer->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (0 != stimer->running) {
		logme(""__FUNCNAME__": stimer is running\r\n");
		r = -2;
		goto end1;
	}

	if (0 >= stimer->tick) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	sigobj = stimer->sigobj;

	if (	(	NULL == sigobj																)	||
			(	0 == sigobj->valid															) 	||
			(	OBJTYPE__UBIK_SEM != sigobj->type && OBJTYPE__UBIK_SIGNAL != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	_stimerlist_add(stimer);

	stimer->running = 1;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int stimer_stop(stimer_pt _stimer) {
	#define	__FUNCNAME__	"stimer_stop"
	int r;
	_stimer_pt stimer = (_stimer_pt) _stimer;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == stimer							)	||
			(	0 == stimer->valid						)	||
			(	OBJTYPE__UBIK_STIMER 	!= stimer->type	)	)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (0 == stimer->running) {
		logme(""__FUNCNAME__": stimer is not running\r\n");
		r = -2;
		goto end1;
	}

	_stimerlist_remove(stimer);

	stimer->running = 0;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

#endif

#endif /* (1 != EXCLUDE_STIMER) */
