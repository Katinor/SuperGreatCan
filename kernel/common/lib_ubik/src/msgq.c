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

#if (1 != EXCLUDE_MSGQ)

#if (!defined(UBI_COMCONFIG__lib_ubik__dummy))

#include <string.h>

#include "itf_ubinos/itf/ubiclib_logm.h"
#include "itf_ubinos/itf/ubiclib_heap.h"
#include "itf_ubinos/itf/ubiclib_cirbuf.h"
#include "itf_ubinos/itf/ubik_msgq.h"

#include "_ubik.h"

int msgq_create(msgq_pt * msgq_p, unsigned int msgsize, unsigned int maxcount) {
	return msgq_create_ext(msgq_p, msgsize, maxcount, 0);
}

int msgq_create_ext(msgq_pt * msgq_p, unsigned int msgsize, unsigned int maxcount, unsigned int option) {
	#define	__FUNCNAME__	"msgq_create_ext"
	int r;
	int r2;
	_sigobj_pt sigobj;
	cirbuf_pt cirbuf;

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

	if (NULL == msgq_p) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end0;
	}

	if (0 >= msgsize || MSGQ_MSGSIZE_MAX < msgsize) {
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

	r = cirbuf_create_ext(&cirbuf, msgsize * maxcount, CIRBUF_OPT__NOOVERWRITE);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at cirbuf_create_ext()\r\n");
		r = -1;
		goto end1;
	}

	r = _sigobj_create(&sigobj);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at _sigobj_create()\r\n");
		r2 = cirbuf_delete(&cirbuf);
		if (0 != r2) {
			logme(""__FUNCNAME__": fail at cirbuf_delete()\r\n");
		}
		goto end1;
	}

	sigobj->maxcount	= maxcount;
	sigobj->msgsize_1	= msgsize - 1;
	sigobj->msgbuf		= cirbuf;

	if (0 != (option & MSGQ_OPT__IGNOREOVERFLOW)) {
		sigobj->ignoreoverflow = 1;
	}

	sigobj->type 		= OBJTYPE__UBIK_MSGQ;
	sigobj->valid		= 1;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	_kernel_monitor_sigobjlist_insertnext(&_kernel_monitor_sigobjlist, NULL, sigobj);
	#endif

	*msgq_p = (msgq_pt) sigobj;

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

int msgq_delete(msgq_pt * msgq_p) {
	return _sigobj_delete((_sigobj_pt *) msgq_p);
}

int msgq_receive(msgq_pt _msgq, unsigned char * msg) {
	#define	__FUNCNAME__	"msgq_receive"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _msgq;
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

	if (NULL == msg) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MSGQ != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (0 < sigobj->count) {
		r = cirbuf_read(sigobj->msgbuf, msg, sigobj->msgsize_1 + 1, NULL);
		if (0 != r) {
			logme(""__FUNCNAME__": fail at cirbuf_read()\r\n");
			r = -1;
			goto end1;
		}
		sigobj->count--;
	}
	else {
		if (0 == task->sysflag01) {
			task->wtask_p->msg = msg;
			r = _sigobj_wait(sigobj, task->wtask_p);
			if (SIGOBJ_SIGTYPE__TIMEOUT != r && 0 != r) {
				logme(""__FUNCNAME__": fail at _sigobj_wait()\r\n");
			}
			task->wtask_p->msg = NULL;
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

int msgq_receive_timed(msgq_pt _msgq, unsigned char * msg, unsigned int tick) {
	#define	__FUNCNAME__	"msgq_receive_timed"
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

	r = msgq_receive(_msgq, msg);

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

int msgq_receive_timedms(msgq_pt _msgq, unsigned char * msg, unsigned int timems) {
	return msgq_receive_timed(_msgq, msg, ubik_timemstotick(timems));
}

int msgq_send(msgq_pt _msgq, unsigned char * msg) {
	#define	__FUNCNAME__	"msgq_send"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _msgq;
	_task_pt otask;
	_wtask_pt wtask;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	if (NULL == msg) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MSGQ != sigobj->type	)		)
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

	if (sigobj->maxcount == sigobj->count) {
		if (0 == sigobj->ignoreoverflow) {
			logme(""__FUNCNAME__": count is over max count\r\n");
			r = UBIK_ERR__OVERFLOWED;
		}
		else {
			r = 0;
		}
	}
	else {
		wtask = _sigobj_wtasklist_gettask(sigobj);
		if (NULL == wtask) {
			r = cirbuf_write(sigobj->msgbuf, msg, sigobj->msgsize_1 + 1, NULL);
			if (0 != r) {
				logme(""__FUNCNAME__": fail at cirbuf_write()\r\n");
				r = -1;
				goto end1;
			}
			sigobj->count++;
		}
		else {
			if (NULL == wtask->msg) {
				logme(""__FUNCNAME__": message buffer of task is NULL.\r\n");
				r = -1;
				goto end1;
			}

			memcpy(wtask->msg, msg, sigobj->msgsize_1 + 1);
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

int msgq_clear(msgq_pt _msgq) {
	#define	__FUNCNAME__	"msgq_clear"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _msgq;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MSGQ != sigobj->type	)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	r = cirbuf_clear(sigobj->msgbuf);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at cirbuf_clear()\r\n");
	}
	else {
		sigobj->count = 0;
	}

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int msgq_setsender(msgq_pt _msgq, task_pt _task) {
	#define	__FUNCNAME__	"msgq_setsender"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _msgq;
	_task_pt task = (_task_pt) _task;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MSGQ != sigobj->type	)		)
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

int msgq_getcount(msgq_pt _msgq, unsigned int * count_p) {
	#define	__FUNCNAME__	"msgq_getcount"
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _msgq;

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
			(	OBJTYPE__UBIK_MSGQ != sigobj->type	)		)
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

#endif /* (1 != EXCLUDE_MSGQ) */
