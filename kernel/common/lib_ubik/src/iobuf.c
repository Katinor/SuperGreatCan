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

#if (1 != EXCLUDE_IOBUF)

#if (!defined(UBI_COMCONFIG__lib_ubik__dummy))

#include <stdlib.h>

#include "itf_ubinos/itf/ubinos.h"
#include "itf_ubinos/itf/ubik_iobuf.h"

#include "_ubik.h"

int iobuf_create(iobuf_pt * iobuf_p, unsigned int maxsize) {
	return iobuf_create_ext(iobuf_p, maxsize, 0);
}

int iobuf_create_ext(iobuf_pt * iobuf_p, unsigned int maxsize, unsigned int option) {
	#define	__FUNCNAME__	"iobuf_create_ext"
	int r;
	#if (1 != EXCLUDE_KERNEL_MONITORING)
	int r2;
	#endif
	_iobuf_pt iobuf;
	unsigned int cbuf_option;

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

	if (NULL == iobuf_p) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end0;
	}

	if (0 >= maxsize) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
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

	iobuf = malloc(sizeof(_iobuf_t));
	if (NULL == iobuf) {
		logme(""__FUNCNAME__": fail at malloc()\r\n");
		r = -1;
		goto end1;
	}

	iobuf->type						= OBJTYPE__UBIK_IOBUF;
	iobuf->valid					= 0;
	iobuf->reserved1				= 0;

	if (0 != (option & IOBUF_OPT__NOOVERWRITE)) {
		iobuf->overwrite			= 0;
	}
	else {
		iobuf->overwrite			= 1;
	}

	iobuf->overflowevent			= 0;

	iobuf->overflowcount			= 0;

	iobuf->maxsize					= maxsize;
	iobuf->size						= 0;

	cbuf_option = 0;
	if (0 == iobuf->overwrite) {
		cbuf_option |= CIRBUF_OPT__NOOVERWRITE;
	}
	r = cirbuf_create_ext(&iobuf->rbuf, maxsize, cbuf_option);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at cirbuf_create_ext()\r\n");
		free(iobuf);
		r = -1;
		goto end1;
	}

	r = cirbuf_create_ext(&iobuf->wbuf, maxsize, cbuf_option);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at cirbuf_create_ext()\r\n");
		cirbuf_delete(&iobuf->rbuf);
		free(iobuf);
		r = -1;
		goto end1;
	}

	r = mutex_create(&iobuf->rmutex);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_create()\r\n");
		cirbuf_delete(&iobuf->wbuf);
		cirbuf_delete(&iobuf->rbuf);
		free(iobuf);
		r = -1;
		goto end1;
	}

	r = condv_create(&iobuf->condv);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at condv_create()\r\n");
		mutex_delete(&iobuf->rmutex);
		cirbuf_delete(&iobuf->wbuf);
		cirbuf_delete(&iobuf->rbuf);
		free(iobuf);
		r = -1;
		goto end1;
	}

	r = mutex_create(&iobuf->mutex);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_create()\r\n");
		condv_delete(&iobuf->condv);
		mutex_delete(&iobuf->rmutex);
		cirbuf_delete(&iobuf->wbuf);
		cirbuf_delete(&iobuf->rbuf);
		free(iobuf);
		r = -1;
		goto end1;
	}

	iobuf->valid					= 1;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	_kernel_monitor_iobuflist_insertnext(&_kernel_monitor_iobuflist, NULL, iobuf);
	#endif

	*iobuf_p = (iobuf_pt) iobuf;

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

int iobuf_delete(iobuf_pt * iobuf_p) {
	#define	__FUNCNAME__	"iobuf_delete"
	int r;
	int r2;
	_iobuf_pt iobuf = NULL;
	cirbuf_pt rbuf = NULL;
	cirbuf_pt wbuf = NULL;
	mutex_pt rmutex = NULL;
	condv_pt condv = NULL;
	mutex_pt mutex = NULL;

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

	if (NULL == iobuf_p) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end0;
	}

	iobuf = (_iobuf_pt) (*iobuf_p);

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
		r = -1;
		goto end0;
	}
	#endif

	ubik_entercrit();

	if (	(	NULL == iobuf								)	||
			(	0 == iobuf->valid							) 	||
			(	OBJTYPE__UBIK_IOBUF	!= iobuf->type			)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (0 != mutex_islocked(iobuf->rmutex)) {
		logme(""__FUNCNAME__": rmutex is locked\r\n");
		r = -1;
		goto end1;
	}

	if (0 != mutex_islocked(iobuf->mutex)) {
		logme(""__FUNCNAME__": mutex is locked\r\n");
		r = -1;
		goto end1;
	}

	rbuf			= iobuf->rbuf;
	wbuf			= iobuf->wbuf;
	rmutex			= iobuf->rmutex;
	condv			= iobuf->condv;
	mutex			= iobuf->mutex;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	_kernel_monitor_iobuflist_remove(iobuf);
	#endif

	iobuf->valid 	= 0;

	*iobuf_p 		= NULL;

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

	if (NULL != mutex) {
		r2 = mutex_delete(&mutex);
		if (0 != r2) {
			logme(""__FUNCNAME__": fail at mutex_delete()\r\n");
			r = -1;
		}
	}
	if (NULL != condv) {
		r2 = condv_delete(&condv);
		if (0 != r2) {
			logme(""__FUNCNAME__": fail at condv_delete()\r\n");
			r = -1;
		}
	}
	if (NULL != rmutex) {
		r2 = mutex_delete(&rmutex);
		if (0 != r2) {
			logme(""__FUNCNAME__": fail at mutex_delete()\r\n");
			r = -1;
		}
	}
	if (NULL != wbuf) {
		r2 = cirbuf_delete(&wbuf);
		if (0 != r2) {
			logme(""__FUNCNAME__": fail at cirbuf_delete()\r\n");
			r = -1;
		}
	}
	if (NULL != rbuf) {
		r2 = cirbuf_delete(&rbuf);
		if (0 != r2) {
			logme(""__FUNCNAME__": fail at cirbuf_delete()\r\n");
			r = -1;
		}
	}
	if (NULL != iobuf) {
		free(iobuf);
	}

end0:
	return r;
	#undef __FUNCNAME__
}

int iobuf_write(iobuf_pt _iobuf, unsigned char * buf, unsigned int size, unsigned int * written_p) {
	#define	__FUNCNAME__	"iobuf_write"
	int r;
	int r2;
	_iobuf_pt iobuf = (_iobuf_pt) _iobuf;
	mutex_pt mutex;
	unsigned int written;
	unsigned int isize;

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
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	if (0 == size) {
		if (NULL != written_p) {
			*written_p = size;
		}
		r = 0;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == iobuf								)	||
			(	0 == iobuf->valid							) 	||
			(	OBJTYPE__UBIK_IOBUF	!= iobuf->type			)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		ubik_exitcrit();
		goto end0;
	}

	mutex = iobuf->mutex;

	ubik_exitcrit();

	r = mutex_lock(mutex);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
		r = -2;
		goto end0;
	}

	if (	(	0 == iobuf->valid							) 	||
			(	OBJTYPE__UBIK_IOBUF	!= iobuf->type			)		)
	{
		logme(""__FUNCNAME__": terminated\r\n");
		r = UBIK_ERR__TERMINATED;
		goto end1;
	}

	if (iobuf->maxsize < size) {
		logme(""__FUNCNAME__": parameter 3 is wrong\r\n");
		r = -4;
		goto end1;
	}

	isize = iobuf->wbuf->size;
	r = cirbuf_write(iobuf->wbuf, buf, size, &written);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at cirbuf_write()\r\n");
		r = -1;
		goto end1;
	}

	if (0 != iobuf->wbuf->overflowcount) {
		if (UINT16_MAX > iobuf->overflowcount) {
			iobuf->overflowcount++;
		}
		if(0 != iobuf->overwrite) {
			iobuf->overflowevent = 1;
		}
		iobuf->wbuf->overflowcount = 0;
	}
	isize = iobuf->wbuf->size - isize;

	if (0 < isize) {
		if (0 == iobuf->size) {
			iobuf->size += isize;
			r = condv_broadcast(iobuf->condv);
			if (0 != r) {
				logme(""__FUNCNAME__": fail at condv_broadcast()\r\n");
				r = -1;
				goto end1;
			}
		}
		else {
			iobuf->size += isize;
		}
	}

	if (NULL != written_p) {
		*written_p = written;
	}

end1:
	r2 = mutex_unlock(mutex);
	if (0 != r2) {
		logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
		r = -1;
	}

end0:
	return r;
	#undef __FUNCNAME__
}

int iobuf_read(iobuf_pt _iobuf, unsigned char * buf, unsigned int size, unsigned int * read_p) {
	#define	__FUNCNAME__	"iobuf_read"
	int r;
	int r2;
	_iobuf_pt iobuf = (_iobuf_pt) _iobuf;
	mutex_pt mutex;
	mutex_pt rmutex;
	int needunlock = 0;
	unsigned int read;
	unsigned int size1;
	unsigned int size2;
	cirbuf_pt tempcirbuf;

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

	if (NULL == buf) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -3;
		goto end0;
	}

	if (0 == size) {
		if (NULL != read_p) {
			*read_p = size;
		}
		r = 0;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == iobuf								)	||
			(	0 == iobuf->valid							) 	||
			(	OBJTYPE__UBIK_IOBUF	!= iobuf->type			)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		ubik_exitcrit();
		goto end0;
	}

	mutex = iobuf->mutex;
	rmutex = iobuf->rmutex;

	ubik_exitcrit();

	r = mutex_lock(mutex);
	if (UBIK_ERR__TIMEOUT == r) {
		goto end0;
	}
	else if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
		r = -1;
		goto end0;
	}
	needunlock = 1;

	if (	(	0 == iobuf->valid							) 	||
			(	OBJTYPE__UBIK_IOBUF	!= iobuf->type			)		)
	{
		logme(""__FUNCNAME__": terminated\r\n");
		r = UBIK_ERR__TERMINATED;
		goto end0;
	}

	for (;;) {
		while (0 == iobuf->size) {
			r = condv_wait(iobuf->condv, mutex);
			if (UBIK_ERR__TIMEOUT == r) {
				goto end0;
			}
			else if (UBIK_ERR__TERMINATED == r) {
				logme(""__FUNCNAME__": terminated\r\n");
				goto end0;
			}
			else if (0 != r) {
				logme(""__FUNCNAME__": fail at condv_wait()\r\n");
				r = -1;
				goto end0;
			}
		}

		needunlock = 0;
		r = mutex_unlock(mutex);
		if (0 != r) {
			logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
			r = -1;
			goto end0;
		}

		r = mutex_lock(rmutex);
		if (UBIK_ERR__TIMEOUT == r) {
			goto end0;
		}
		else if (0 != r) {
			logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
			r = -1;
			goto end0;
		}

		if (	(	0 == iobuf->valid							) 	||
				(	OBJTYPE__UBIK_IOBUF	!= iobuf->type			)		)
		{
			logme(""__FUNCNAME__": terminated\r\n");
			r = UBIK_ERR__TERMINATED;
			goto end1;
		}

		r = mutex_lock(mutex);
		if (UBIK_ERR__TIMEOUT == r) {
			goto end1;
		}
		else if (0 != r) {
			logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
			r = -1;
			goto end1;
		}
		needunlock = 1;

		if (0 != iobuf->overflowevent) {
			size1 = iobuf->rbuf->size;
			r = cirbuf_clear(iobuf->rbuf);
			if (0 != r) {
				logme(""__FUNCNAME__": fail at cirbuf_clear()\r\n");
				r = -1;
				goto end1;
			}
			iobuf->size -= size1;
			iobuf->overflowevent = 0;
		}

		if (0 < iobuf->size) {
			break;
		}

		r = mutex_unlock(rmutex);
		if (0 != r) {
			logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
			r = -1;
			goto end0;
		}
	}

	if (0 >= iobuf->rbuf->size) {
		tempcirbuf = iobuf->rbuf;
		iobuf->rbuf = iobuf->wbuf;
		iobuf->wbuf = tempcirbuf;
	}

	size1 = size;
	if (size1 > iobuf->rbuf->size) {
		size1 = iobuf->rbuf->size;
	}

	iobuf->size -= size1;

	needunlock = 0;
	r = mutex_unlock(mutex);
	if (0 != r) {
		iobuf->size += size1;
		logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
		r = -1;
		goto end1;
	}

	r = cirbuf_read(iobuf->rbuf, buf, size1, &read);
	if (0 != r || size1 != read) {
		logme(""__FUNCNAME__": fail at cirbuf_read()\r\n");
		r = -1;
		goto end1;
	}

	if (NULL != read_p) {
		*read_p = size1;
	}

	if (size1 < size) {
		r = mutex_lock(mutex);
		if (UBIK_ERR__TIMEOUT == r) {
			r = 0;
			goto end1;
		}
		else if (0 != r) {
			logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
			r = -1;
			goto end1;
		}
		needunlock = 1;

		if (0 != iobuf->overflowevent) {
			r = 0;
			goto end1;
		}

		if (0 < iobuf->wbuf->size) {
			tempcirbuf = iobuf->rbuf;
			iobuf->rbuf = iobuf->wbuf;
			iobuf->wbuf = tempcirbuf;

			size2 = size - size1;
			if (size2 > iobuf->rbuf->size) {
				size2 = iobuf->rbuf->size;
			}

			iobuf->size -= size2;
		}
		else {
			size2 = 0;
		}

		needunlock = 0;
		r = mutex_unlock(mutex);
		if (0 != r) {
			iobuf->size += size2;
			logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
			r = -1;
			goto end1;
		}

		if (0 < size2){
			r = cirbuf_read(iobuf->rbuf, &buf[size1], size2, &read);
			if (0 != r || size2 != read) {
				logme(""__FUNCNAME__": fail at cirbuf_read()\r\n");
				r = -1;
				goto end1;
			}
		}

		if (NULL != read_p) {
			*read_p = size1 + size2;
		}
	}

end1:
	r2 = mutex_unlock(rmutex);
	if (0 != r2) {
		logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
		r = -1;
	}

end0:
	if (0 != needunlock) {
		r2 = mutex_unlock(mutex);
		if (0 != r2) {
			logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
			r = -1;
		}
	}

	return r;
	#undef __FUNCNAME__
}

int iobuf_read_timed(iobuf_pt iobuf, unsigned char * buf, unsigned int size, unsigned int * read_p, unsigned int tick) {
	#define	__FUNCNAME__	"iobuf_read_timed"
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

	r = iobuf_read(iobuf, buf, size, read_p);

	_task_cur->timed		= 0;

end0:
	return r;
	#undef __FUNCNAME__
}

int iobuf_wait(iobuf_pt _iobuf) {
	#define	__FUNCNAME__	"iobuf_wait"
	int r;
	int r2;
	_iobuf_pt iobuf = (_iobuf_pt) _iobuf;
	mutex_pt mutex;
	_task_pt task = _task_cur;
	int needunlock = 0;

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
	else {
		if (0 == _ubik_critcount) {
			logme(""__FUNCNAME__": sysflag01 is 1 but not in critical section\r\n");
			r = -1;
			goto end0;
		}
	}

	ubik_entercrit();

	if (	(	NULL == iobuf								)	||
			(	0 == iobuf->valid							) 	||
			(	OBJTYPE__UBIK_IOBUF	!= iobuf->type			)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		ubik_exitcrit();
		goto end0;
	}

	mutex = iobuf->mutex;

	ubik_exitcrit();

	if (0 == task->sysflag01) {
		r = mutex_lock(mutex);
		if (UBIK_ERR__TIMEOUT == r) {
			goto end0;
		}
		else if (0 != r) {
			logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
			r = -1;
			goto end0;
		}
		needunlock = 1;

		if (	(	0 == iobuf->valid							) 	||
				(	OBJTYPE__UBIK_IOBUF	!= iobuf->type			)		)
		{
			logme(""__FUNCNAME__": terminated\r\n");
			r = UBIK_ERR__TERMINATED;
			goto end0;
		}

		while (0 == iobuf->size) {
			r = condv_wait(iobuf->condv, mutex);
			if (UBIK_ERR__TIMEOUT == r) {
				goto end0;
			}
			else if (UBIK_ERR__TERMINATED == r) {
				logme(""__FUNCNAME__": terminated\r\n");
				goto end0;
			}
			else if (0 != r) {
				logme(""__FUNCNAME__": fail at condv_wait()\r\n");
				r = -1;
				goto end0;
			}
		}

		r = 0;
	}
	else {
		if (0 < iobuf->size) {
			r = 0;
		}
		else {
			r = SIGOBJ_SIGTYPE__TIMEOUT;
		}
	}

end0:
	if (0 != needunlock) {
		r2 = mutex_unlock(mutex);
		if (0 != r2) {
			logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
			r = -1;
		}
	}

	return r;
	#undef __FUNCNAME__
}

int iobuf_wait_timed(iobuf_pt iobuf, unsigned int tick) {
	#define	__FUNCNAME__	"iobuf_wait_timed"
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

	r = iobuf_wait(iobuf);

	_task_cur->timed		= 0;

end0:
	return r;
	#undef __FUNCNAME__
}

int iobuf_getsize(iobuf_pt _iobuf, unsigned int * size_p) {
	#define	__FUNCNAME__	"iobuf_getsize"
	int r;
	_iobuf_pt iobuf = (_iobuf_pt) _iobuf;

	if (NULL == size_p) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == iobuf								)	||
			(	0 == iobuf->valid							) 	||
			(	OBJTYPE__UBIK_IOBUF	!= iobuf->type			)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	*size_p = iobuf->size;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

int iobuf_clear(iobuf_pt _iobuf) {
	#define	__FUNCNAME__	"iobuf_clear"
	int r;
	int r2;
	_iobuf_pt iobuf = (_iobuf_pt) _iobuf;
	mutex_pt mutex;
	mutex_pt rmutex;

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

	if (	(	NULL == iobuf								)	||
			(	0 == iobuf->valid							) 	||
			(	OBJTYPE__UBIK_IOBUF	!= iobuf->type			)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		ubik_exitcrit();
		goto end0;
	}

	mutex = iobuf->mutex;
	rmutex = iobuf->rmutex;

	ubik_exitcrit();

	r = mutex_lock(rmutex);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
		r = -1;
		goto end0;
	}

	r = mutex_lock(mutex);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at mutex_lock()\r\n");
		r = -1;
		goto end1;
	}

	if (	(	0 == iobuf->valid							) 	||
			(	OBJTYPE__UBIK_IOBUF	!= iobuf->type			)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end2;
	}

	iobuf->overflowevent	= 0;
	iobuf->overflowcount	= 0;

	r = cirbuf_clear(iobuf->rbuf);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at cirbuf_clear()\r\n");
		r = -2;
		goto end2;
	}

	r = cirbuf_clear(iobuf->wbuf);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at cirbuf_clear()\r\n");
		r = -2;
		goto end2;
	}
	iobuf->size				= 0;

end2:
	r2 = mutex_unlock(mutex);
	if (0 != r2) {
		logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
		r = -1;
	}

end1:
	r2 = mutex_unlock(rmutex);
	if (0 != r2) {
		logme(""__FUNCNAME__": fail at mutex_unlock()\r\n");
		r = -1;
	}

end0:
	return r;
	#undef __FUNCNAME__
}

int iobuf_setsender(iobuf_pt _iobuf, task_pt task) {
	#define	__FUNCNAME__	"iobuf_setsender"
	int r;
	_iobuf_pt iobuf = (_iobuf_pt) _iobuf;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == iobuf								)	||
			(	0 == iobuf->valid							) 	||
			(	OBJTYPE__UBIK_IOBUF	!= iobuf->type			)		)
	{
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	r = condv_setsender(iobuf->condv, task);
	if (0 != r) {
		logme(""__FUNCNAME__": fail at condv_setsender()\r\n");
	}

end1:
	ubik_exitcrit();

end0:
	return r;
	#undef __FUNCNAME__
}

#endif

#endif /* (1 != EXCLUDE_IOBUF) */
