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

#ifndef _UBIK_H_
#define _UBIK_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file _ubik.h
 *
 * @brief ubik (Ubinos Kernel) ������Ʈ ���� �������̽�
 *
 * ubik ������Ʈ ���� �������̽��� �����մϴ�.
 */

#include "../ubiconfig.h"

#include "itf_ubinos/itf/type.h"
#include "itf_ubinos/itf/ubiclib.h"
#include "itf_ubinos/itf/ubiclib_edlist.h"
#include "itf_ubinos/itf/ubiclib_cirbuf.h"
#include "itf_ubinos/itf/ubik.h"
#include "itf_ubinos/itf/ubik_task.h"
#include "itf_ubinos/itf/ubik_signal.h"

#if (1 != EXCLUDE_CONDV)
	#include "itf_ubinos/itf/ubik_condv.h"
#endif /* (1 != EXCLUDE_CONDV) */

#include "itf_ubinos/itf/ubik_mutex.h"

#if (1 != EXCLUDE_SEM)
	#include "itf_ubinos/itf/ubik_sem.h"
#endif /* (1 != EXCLUDE_SEM) */

/*****************************************************************************/

/**
 * ubik porting ������Ʈ�� �ʱ�ȭ�ϴ� �Լ�
 *
 * �� �Լ��� ubik_comp_init �Լ����� �ڵ������� ȣ��˴ϴ�.
 *
 * @param	idle_stackdepth: idle task ���� ����(���� ���� ũ��� stackdepth * INT_SIZE ����Ʈ�� ��)<br>
 * 			0: �⺻  ���� ���� ��(task_getdefaultstackdepth �Լ��� �����ִ� ��)<br>
 * 			1 ~ ���� ���� ���� ��: ���� ���� ���� ��(task_getminstackdepth �Լ��� �����ִ� ��)<br>
 * 			<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 */
int ubik_porting_comp_init(unsigned int idle_stackdepth);

/**
 * ubik porting ������Ʈ�� �����ϴ� �Լ�
 */
void ubik_porting_comp_start(void);

/**
 * tick�� �ʱ�ȭ �ϴ� �Լ�
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 */
int ubik_inittick(void);

/**
 * tick ���ͷ�Ʈ ���� ��ƾ
 */
void ubik_tickisr(void);

/**
 * ����Ʈ���� ���ͷ�Ʈ ���� ��ƾ
 */
void ubik_swisr(unsigned int swino);

/*****************************************************************************/

#define 				TASK_PRIORITY__IDLE				0

#define					TASK_STATE__READY				0
#define					TASK_STATE__RUNNING				1
#define					TASK_STATE__BLOCKED				2
#define					TASK_STATE__TERMINATED			3

/** @define �ñ׳� ����: ���� */
#define					SIGOBJ_SIGTYPE__SUCCESS			UBIK_ERR__SUCCESS
/** @define �ñ׳� ����: ���� */
#define					SIGOBJ_SIGTYPE__FAIL			UBIK_ERR__FAIL
/** @define �ñ׳� ����: ���� �ð� �ʰ� */
#define					SIGOBJ_SIGTYPE__TIMEOUT			UBIK_ERR__TIMEOUT
/** @define �ñ׳� ����: �ñ׳��� ���� ��ü�� �����Ǿ��� */
#define					SIGOBJ_SIGTYPE__TERMINATED		UBIK_ERR__TERMINATED

/**
 * task ������Ʈ�� �ʱ�ȭ�ϴ� �Լ�
 *
 * @param	idle_stackdepth: idle task ���� ����(���� ���� ũ��� stackdepth * INT_SIZE ����Ʈ�� ��)<br>
 * 			0: �⺻  ���� ���� ��(task_getdefaultstackdepth �Լ��� �����ִ� ��)<br>
 * 			1 ~ ���� ���� ���� ��: ���� ���� ���� ��(task_getminstackdepth �Լ��� �����ִ� ��)<br>
 * 			<br>
 *
 * @return	  0: ����<br>
 * 			<br>
 * 			 -1: ����<br>
 */
int task_comp_init(unsigned int idle_stackdepth);

/**
 * task ������Ʈ�� �����ϴ� �Լ�
 */
void task_comp_start(void);

/*****************************************************************************/

/*
 * �ñ׳��� ���� �� �ִ� ��ü �� ����
 */
typedef	struct __sigobj_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved1				:  7;

	unsigned int		reserved2				:  1;
	unsigned int		nopriorityinheritance	:  1;
	unsigned int		ignoreoverflow			:  1;
	unsigned int		msgsize_1				:  5;
	unsigned int		wtasklist_maxpriority	:  8;

	edlist_t			wtasklist;

	struct {
		struct __sigobj_t *	prev;
		struct __sigobj_t *	next;
		edlist_pt			list;
	}					osigobjlist_link;

	unsigned int		count;
	unsigned int		maxcount;
	cirbuf_pt			msgbuf;

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	struct {
		struct __sigobj_t *	prev;
		struct __sigobj_t *	next;
		edlist_pt			list;
	}					kernel_monitor_sigobjlist_link;
	#endif
} _sigobj_t;

typedef _sigobj_t * _sigobj_pt;

/*
 * �½�ũ ��ü �� ����
 */
typedef	struct __task_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved1				:  7;

	unsigned int		reserved2				: 16;

	unsigned int * 		stacktop;
	unsigned int * 		stacktop_max;
	unsigned int *		stacklimit;
	unsigned int *		stack;
	unsigned int		stacksize;

	struct {
		struct __task_t *	prev;
		struct __task_t *	next;
		edlist_pt			list;
	} 					tasklist_link;

	struct __wtask_t {
		struct {
			struct __wtask_t *	prev;
			struct __wtask_t *	next;
			edlist_pt			list;
		} link;
		struct __task_t *		task;
		int						sigtype;
		unsigned char *			msg;
		mutex_pt				mutex;
	} 					wtask;

	struct __wtask_t *	wtask_p;
	int					wtask_max;
	int					wtask_count;
	int					wtask_waitcount;
	int					wtask_recvcount;

	edlist_t			osigobjlist;

	unsigned int		state					:  2;
	unsigned int		suspended				:  1;
	unsigned int		timed					:  1;
	unsigned int		sysflag01				:  1;
	unsigned int		stackext				:  1;
	unsigned int		waitall					:  1;
	unsigned int		waitmask				:  8;
	unsigned int		reserved3				:  1;

	unsigned int		priority				:  8;
	unsigned int		priority_ori			:  8;

	unsigned int		reserved4				: 16;

	unsigned int		wakeuptick;

	taskfunc_ft			func;

	#if (1 != EXCLUDE_TASK_MONITORING)
	signal_pt			monitor;
	#endif /* (1 != EXCLUDE_TASK_MONITORING) */

	char				name[TASK_NAME_SIZE_MAX];

	#if (1 != EXCLUDE_KERNEL_MONITORING)
	struct {
		struct __task_t *	prev;
		struct __task_t *	next;
		edlist_pt			list;
	}					kernel_monitor_tasklist_link;
	#endif
} _task_t;

typedef _task_t * _task_pt;

typedef struct __wtask_t _wtask_t;

typedef _wtask_t * _wtask_pt;

/*
 * Ÿ�̸� ��ü �� ����
 */
typedef	struct __stimer_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved1				:  7;

	unsigned int		reserved2				: 13;
	unsigned int		broadcast				:  1;
	unsigned int		oneshot					:  1;
	unsigned int		running					:  1;

	struct {
		struct __stimer_t *	prev;
		struct __stimer_t *	next;
		edlist_pt			list;
	}					stimerlist_link;

	unsigned int		tick;
	unsigned int		wakeuptick;

	_sigobj_pt			sigobj;
	int					sigtype;

	#if       (1 != EXCLUDE_KERNEL_MONITORING)
	struct {
		struct __stimer_t *	prev;
		struct __stimer_t *	next;
		edlist_pt			list;
	}					kernel_monitor_stimerlist_link;
	#endif /* (1 != EXCLUDE_KERNEL_MONITORING) */
} _stimer_t;

typedef _stimer_t * _stimer_pt;

#if       (1 != EXCLUDE_IOBUF)
/*
 * ����¹��� ���� ��
 */
typedef	struct __iobuf_t {
	unsigned int		type					:  8;
	unsigned int		valid					:  1;
	unsigned int		reserved1				:  5;
	unsigned int		overwrite				:  1;
	unsigned int		overflowevent			:  1;

	unsigned int		overflowcount			: 16;

	unsigned int		maxsize;
	unsigned int		size;

	cirbuf_pt			rbuf;
	cirbuf_pt			wbuf;

	mutex_pt			rmutex;

	condv_pt			condv;
	mutex_pt			mutex;

	#if       (1 != EXCLUDE_KERNEL_MONITORING)
	struct {
		struct __iobuf_t *	prev;
		struct __iobuf_t *	next;
		edlist_pt			list;
	}					kernel_monitor_iobuflist_link;
	#endif /* (1 != EXCLUDE_KERNEL_MONITORING) */
} _iobuf_t;

typedef _iobuf_t * _iobuf_pt;
#endif /* (1 != EXCLUDE_IOBUF) */

/*****************************************************************************/

#define _tasklist_insertprev(tasklist, ref, task)	edlist_insertprev(	_task_pt, tasklist_link, tasklist, ref, task)
#define _tasklist_insertnext(tasklist, ref, task)	edlist_insertnext(	_task_pt, tasklist_link, tasklist, ref, task)
#define _tasklist_remove(task)						edlist_remove(		_task_pt, tasklist_link, task)
#define _tasklist_getcur(tasklist)					edlist_getcur(		_task_pt, tasklist_link, tasklist)
#define _tasklist_getcurnext(tasklist)				edlist_getcurnext(	_task_pt, tasklist_link, tasklist)
#define _tasklist_head(tasklist)					edlist_head(		_task_pt, tasklist_link, tasklist)
#define _tasklist_tail(tasklist)					edlist_tail(		_task_pt, tasklist_link, tasklist)
#define _tasklist_next(task)						edlist_next(		_task_pt, tasklist_link, task)
#define _tasklist_prev(task)						edlist_prev(		_task_pt, tasklist_link, task)
#define _tasklist_list(task)						edlist_list(		_task_pt, tasklist_link, task)

/*****************************************************************************/

#define _sigobj_wtasklist_insertprev(sigobj, ref, wtask)	\
													edlist_insertprev(	_wtask_pt, link, &(sigobj)->wtasklist, ref, wtask)
#define _sigobj_wtasklist_insertnext(sigobj, ref, wtask)	\
													edlist_insertnext(	_wtask_pt, link, &(sigobj)->wtasklist, ref, wtask)
#define _sigobj_wtasklist_remove(wtask)				edlist_remove(		_wtask_pt, link, wtask)
#define _sigobj_wtasklist_getcur(sigobj)			edlist_getcur(		_wtask_pt, link, &(sigobj)->wtasklist)
#define _sigobj_wtasklist_getcurnext(sigobj)		edlist_getcurnext(	_wtask_pt, link, &(sigobj)->wtasklist)
#define _sigobj_wtasklist_head(sigobj)				edlist_head(		_wtask_pt, link, &(sigobj)->wtasklist)
#define _sigobj_wtasklist_tail(sigobj)				edlist_tail(		_wtask_pt, link, &(sigobj)->wtasklist)
#define _sigobj_wtasklist_next(wtask)				edlist_next(		_wtask_pt, link, wtask)
#define _sigobj_wtasklist_prev(wtask)				edlist_prev(		_wtask_pt, link, wtask)
#define _sigobj_wtasklist_list(wtask)				edlist_list(		_wtask_pt, link, wtask)

#define _sigobj_wtasklist_owner(wtask)				((_sigobj_pt) (NULL != _sigobj_wtasklist_list(wtask) ? _sigobj_wtasklist_list(wtask)->data : NULL))

unsigned int	_sigobj_wtasklist_getmaxpriority(_sigobj_pt sigobj);
_wtask_pt 		_sigobj_wtasklist_gettask(_sigobj_pt sigobj);
#if       (1 != EXCLUDE_TASK_PRIORITY_INHERITANCE)
void 			_sigobj_wtasklist_notifychange(_sigobj_pt sigobj);
#else
#define 		_sigobj_wtasklist_notifychange(sigobj)
#endif /* (1 != EXCLUDE_TASK_PRIORITY_INHERITANCE) */

/*****************************************************************************/

#define _task_osigobjlist_insertprev(task, ref, sigobj)	\
													edlist_insertprev(	_sigobj_pt, osigobjlist_link, &(task)->osigobjlist, ref, sigobj)
#define _task_osigobjlist_insertnext(task, ref, sigobj)	\
													edlist_insertnext(	_sigobj_pt, osigobjlist_link, &(task)->osigobjlist, ref, sigobj)
#define _task_osigobjlist_remove(sigobj)			edlist_remove(		_sigobj_pt, osigobjlist_link, sigobj)
#define _task_osigobjlist_getcur(task)				edlist_getcur(		_sigobj_pt, osigobjlist_link, &(task)->osigobjlist)
#define _task_osigobjlist_getcurnext(task)			edlist_getcurnext(	_sigobj_pt, osigobjlist_link, &(task)->osigobjlist)
#define _task_osigobjlist_head(task)				edlist_head(		_sigobj_pt, osigobjlist_link, &(task)->osigobjlist)
#define _task_osigobjlist_tail(task)				edlist_tail(		_sigobj_pt, osigobjlist_link, &(task)->osigobjlist)
#define _task_osigobjlist_next(sigobj)				edlist_next(		_sigobj_pt, osigobjlist_link, sigobj)
#define _task_osigobjlist_prev(sigobj)				edlist_prev(		_sigobj_pt, osigobjlist_link, sigobj)
#define _task_osigobjlist_list(sigobj)				edlist_list(		_sigobj_pt, osigobjlist_link, sigobj)

#define _task_osigobjlist_owner(sigobj)				((_task_pt) (NULL != _task_osigobjlist_list(sigobj) ? _task_osigobjlist_list(sigobj)->data : NULL))

/*****************************************************************************/

#if       (1 != EXCLUDE_STIMER)

#define _stimerlist_insertprev(stimerlist, ref, stimer)	\
													edlist_insertprev(	_stimer_pt, stimerlist_link, stimerlist, ref, stimer)
#define _stimerlist_insertnext(stimerlist, ref, stimer)	\
													edlist_insertnext(	_stimer_pt, stimerlist_link, stimerlist, ref, stimer)
#define _stimerlist_remove(stimer)					edlist_remove(		_stimer_pt, stimerlist_link, stimer)
#define _stimerlist_getcur(stimerlist)				edlist_getcur(		_stimer_pt, stimerlist_link, stimerlist)
#define _stimerlist_getcurnext(stimerlist)			edlist_getcurnext(	_stimer_pt, stimerlist_link, stimerlist)
#define _stimerlist_head(stimerlist)				edlist_head(		_stimer_pt, stimerlist_link, stimerlist)
#define _stimerlist_tail(stimerlist)				edlist_tail(		_stimer_pt, stimerlist_link, stimerlist)
#define _stimerlist_next(stimer)					edlist_next(		_stimer_pt, stimerlist_link, stimer)
#define _stimerlist_prev(stimer)					edlist_prev(		_stimer_pt, stimerlist_link, stimer)
#define _stimerlist_list(stimer)					edlist_list(		_stimer_pt, stimerlist_link, stimer)

void _stimerlist_add(_stimer_pt stimer);

#endif /* (1 != EXCLUDE_STIMER) */

/*****************************************************************************/

#if       (1 != EXCLUDE_KERNEL_MONITORING)

#define _kernel_monitor_tasklist_insertprev(kernel_monitor_tasklist, ref, task)	\
																			edlist_insertprev(	_task_pt, kernel_monitor_tasklist_link, kernel_monitor_tasklist, ref, task)
#define _kernel_monitor_tasklist_insertnext(kernel_monitor_tasklist, ref, task)	\
																			edlist_insertnext(	_task_pt, kernel_monitor_tasklist_link, kernel_monitor_tasklist, ref, task)
#define _kernel_monitor_tasklist_remove(task)								edlist_remove(		_task_pt, kernel_monitor_tasklist_link, task)
#define _kernel_monitor_tasklist_getcur(kernel_monitor_tasklist)			edlist_getcur(		_task_pt, kernel_monitor_tasklist_link, kernel_monitor_tasklist)
#define _kernel_monitor_tasklist_getcurnext(kernel_monitor_tasklist)		edlist_getcurnext(	_task_pt, kernel_monitor_tasklist_link, kernel_monitor_tasklist)
#define _kernel_monitor_tasklist_head(kernel_monitor_tasklist)				edlist_head(		_task_pt, kernel_monitor_tasklist_link, kernel_monitor_tasklist)
#define _kernel_monitor_tasklist_tail(kernel_monitor_tasklist)				edlist_tail(		_task_pt, kernel_monitor_tasklist_link, kernel_monitor_tasklist)
#define _kernel_monitor_tasklist_next(task)									edlist_next(		_task_pt, kernel_monitor_tasklist_link, task)
#define _kernel_monitor_tasklist_prev(task)									edlist_prev(		_task_pt, kernel_monitor_tasklist_link, task)
#define _kernel_monitor_tasklist_list(task)									edlist_list(		_task_pt, kernel_monitor_tasklist_link, task)

/*************************************/

#define _kernel_monitor_stimerlist_insertprev(kernel_monitor_stimerlist, ref, stimer)	\
																			edlist_insertprev(	_stimer_pt, kernel_monitor_stimerlist_link, kernel_monitor_stimerlist, ref, stimer)
#define _kernel_monitor_stimerlist_insertnext(kernel_monitor_stimerlist, ref, stimer)	\
																			edlist_insertnext(	_stimer_pt, kernel_monitor_stimerlist_link, kernel_monitor_stimerlist, ref, stimer)
#define _kernel_monitor_stimerlist_remove(stimer)							edlist_remove(		_stimer_pt, kernel_monitor_stimerlist_link, stimer)
#define _kernel_monitor_stimerlist_getcur(kernel_monitor_stimerlist)		edlist_getcur(		_stimer_pt, kernel_monitor_stimerlist_link, kernel_monitor_stimerlist)
#define _kernel_monitor_stimerlist_getcurnext(kernel_monitor_stimerlist)	edlist_getcurnext(	_stimer_pt, kernel_monitor_stimerlist_link, kernel_monitor_stimerlist)
#define _kernel_monitor_stimerlist_head(kernel_monitor_stimerlist)			edlist_head(		_stimer_pt, kernel_monitor_stimerlist_link, kernel_monitor_stimerlist)
#define _kernel_monitor_stimerlist_tail(kernel_monitor_stimerlist)			edlist_tail(		_stimer_pt, kernel_monitor_stimerlist_link, kernel_monitor_stimerlist)
#define _kernel_monitor_stimerlist_next(stimer)								edlist_next(		_stimer_pt, kernel_monitor_stimerlist_link, stimer)
#define _kernel_monitor_stimerlist_prev(stimer)								edlist_prev(		_stimer_pt, kernel_monitor_stimerlist_link, stimer)
#define _kernel_monitor_stimerlist_list(stimer)								edlist_list(		_stimer_pt, kernel_monitor_stimerlist_link, stimer)

/*************************************/

#define _kernel_monitor_sigobjlist_insertprev(kernel_monitor_sigobjlist, ref, sigobj)	\
																			edlist_insertprev(	_sigobj_pt, kernel_monitor_sigobjlist_link, kernel_monitor_sigobjlist, ref, sigobj)
#define _kernel_monitor_sigobjlist_insertnext(kernel_monitor_sigobjlist, ref, sigobj)	\
																			edlist_insertnext(	_sigobj_pt, kernel_monitor_sigobjlist_link, kernel_monitor_sigobjlist, ref, sigobj)
#define _kernel_monitor_sigobjlist_remove(sigobj)							edlist_remove(		_sigobj_pt, kernel_monitor_sigobjlist_link, sigobj)
#define _kernel_monitor_sigobjlist_getcur(kernel_monitor_sigobjlist)		edlist_getcur(		_sigobj_pt, kernel_monitor_sigobjlist_link, kernel_monitor_sigobjlist)
#define _kernel_monitor_sigobjlist_getcurnext(kernel_monitor_sigobjlist)	edlist_getcurnext(	_sigobj_pt, kernel_monitor_sigobjlist_link, kernel_monitor_sigobjlist)
#define _kernel_monitor_sigobjlist_head(kernel_monitor_sigobjlist)			edlist_head(		_sigobj_pt, kernel_monitor_sigobjlist_link, kernel_monitor_sigobjlist)
#define _kernel_monitor_sigobjlist_tail(kernel_monitor_sigobjlist)			edlist_tail(		_sigobj_pt, kernel_monitor_sigobjlist_link, kernel_monitor_sigobjlist)
#define _kernel_monitor_sigobjlist_next(sigobj)								edlist_next(		_sigobj_pt, kernel_monitor_sigobjlist_link, sigobj)
#define _kernel_monitor_sigobjlist_prev(sigobj)								edlist_prev(		_sigobj_pt, kernel_monitor_sigobjlist_link, sigobj)
#define _kernel_monitor_sigobjlist_list(sigobj)								edlist_list(		_sigobj_pt, kernel_monitor_sigobjlist_link, sigobj)

/*************************************/

#define _kernel_monitor_iobuflist_insertprev(kernel_monitor_iobuflist, ref, iobuf)	\
																			edlist_insertprev(	_iobuf_pt, kernel_monitor_iobuflist_link, kernel_monitor_iobuflist, ref, iobuf)
#define _kernel_monitor_iobuflist_insertnext(kernel_monitor_iobuflist, ref, iobuf)	\
																			edlist_insertnext(	_iobuf_pt, kernel_monitor_iobuflist_link, kernel_monitor_iobuflist, ref, iobuf)
#define _kernel_monitor_iobuflist_remove(iobuf)								edlist_remove(		_iobuf_pt, kernel_monitor_iobuflist_link, iobuf)
#define _kernel_monitor_iobuflist_getcur(kernel_monitor_iobuflist)			edlist_getcur(		_iobuf_pt, kernel_monitor_iobuflist_link, kernel_monitor_iobuflist)
#define _kernel_monitor_iobuflist_getcurnext(kernel_monitor_iobuflist)		edlist_getcurnext(	_iobuf_pt, kernel_monitor_iobuflist_link, kernel_monitor_iobuflist)
#define _kernel_monitor_iobuflist_head(kernel_monitor_iobuflist)			edlist_head(		_iobuf_pt, kernel_monitor_iobuflist_link, kernel_monitor_iobuflist)
#define _kernel_monitor_iobuflist_tail(kernel_monitor_iobuflist)			edlist_tail(		_iobuf_pt, kernel_monitor_iobuflist_link, kernel_monitor_iobuflist)
#define _kernel_monitor_iobuflist_next(iobuf)								edlist_next(		_iobuf_pt, kernel_monitor_iobuflist_link, iobuf)
#define _kernel_monitor_iobuflist_prev(iobuf)								edlist_prev(		_iobuf_pt, kernel_monitor_iobuflist_link, iobuf)
#define _kernel_monitor_iobuflist_list(iobuf)								edlist_list(		_iobuf_pt, kernel_monitor_iobuflist_link, iobuf)

#endif /* (1 != EXCLUDE_KERNEL_MONITORING) */

/*****************************************************************************/

int _sigobj_create(_sigobj_pt * sigobj_p);
int _sigobj_delete(_sigobj_pt * sigobj_p);
int _sigobj_wait(_sigobj_pt sigobj, _wtask_pt wtask);
int _sigobj_send(_sigobj_pt sigobj, int sigtype);
int _sigobj_broadcast(_sigobj_pt sigobj, int sigtype);
int _sigobj_setsender(_sigobj_pt sigobj, _task_pt task);

/*****************************************************************************/

void _task_init(_task_pt task);
void _task_stackinit(_task_pt task, void * arg);

void _task_idlefunc(void * arg);
void _task_rootfunc(void * arg);

void _task_changelist(_task_pt task);
void _task_yield(void);
void _task_applypriority(_task_pt task);
void _task_collectgarbage(void);

#define _task_calcremainingtimeout(task) {											\
	if (0 != task->timed) {															\
		if (_ubik_tickcount >= task->wakeuptick) {									\
			task->wakeuptick = UINT_MAX - _ubik_tickcount + task->wakeuptick + 1;	\
		}																			\
		else {																		\
			task->wakeuptick -= _ubik_tickcount;									\
		}																			\
	}																				\
}

#define _task_schedule() {															\
	if (_task_list_ready_index > _task_cur->priority) {								\
		_task_yield();																\
	}																				\
}																					\

#define ubik_task_schedule_irq() {													\
	if (0 == _ubik_tasklockcount && 0 != _ubik_active) {							\
		if(_task_list_ready_index > _task_cur->priority) {							\
			_task_prev = _task_cur;													\
			if(TASK_STATE__RUNNING == _task_cur->state) {							\
				_task_cur->state = TASK_STATE__READY;								\
			}																		\
			_task_cur = _tasklist_getcurnext(_task_list_ready_cur);					\
			_task_cur->state = TASK_STATE__RUNNING;									\
		}																			\
	}																				\
}

void 		_task_sigobj_removewtask(_task_pt task);
#if       (1 != EXCLUDE_TASK_PRIORITY_INHERITANCE)
void 		_task_sigobj_notifywtaskchange(_task_pt task);
#else
#define 	_task_sigobj_notifywtaskchange(task)
#endif /* (1 != EXCLUDE_TASK_PRIORITY_INHERITANCE) */

/*****************************************************************************/

extern volatile unsigned int 	_ubik_active;								/* variable representing whether system is active or not */
extern volatile unsigned int	_ubik_tickcount;							/* system tick count */
extern volatile unsigned int	_ubik_tickcounth;							/* system tick count high value */
extern volatile unsigned int	_ubik_intrcount;							/* interrupt nesting count */
extern volatile unsigned int	_ubik_critcount;							/* critical section nesting count */
extern volatile unsigned int	_ubik_tasklockcount;						/* task lock nesting count */
extern volatile unsigned int	_ubik_rtpriority;							/* lowest priority of task considered as a real-time task */

extern			edlist_t		_task_list_blocked_timed_a[2];				/* timed blocked task list array */
extern volatile edlist_pt		_task_list_blocked_timed_cur;				/* current timed blocked task list pointer */
extern volatile edlist_pt		_task_list_blocked_timed_next;				/* next timed blocked task list pointer */
extern 			edlist_t		_task_list_blocked;							/* blocked forever task list */
extern 			edlist_t		_task_list_ready_a[TASK_PRIORITY_MAX+1];	/* ready task list array */
extern volatile unsigned int 	_task_list_ready_index;						/* current ready task list index */
extern volatile edlist_pt		_task_list_ready_cur;						/* current ready task list pointer */
extern 			edlist_t		_task_list_suspended;						/* suspended task list */
extern 			edlist_t		_task_list_terminated;						/* terminated task list */
extern volatile _task_pt 		_task_cur;									/* current running task pointer */
extern volatile _task_pt 		_task_prev;									/* next running task pointer */

#if       (1 != EXCLUDE_STIMER)
extern 			edlist_t		_stimer_list_a[2];							/* stimer list array */
extern volatile edlist_pt		_stimer_list_cur;							/* current stimer list pointer */
extern volatile edlist_pt		_stimer_list_next;							/* next stimer list pointer */
#endif /* (1 != EXCLUDE_STIMER) */

#if       (1 != EXCLUDE_KERNEL_MONITORING)
extern 			mutex_pt		_kernel_monitor_mutex;
extern			edlist_t		_kernel_monitor_tasklist;
extern			edlist_t		_kernel_monitor_stimerlist;
extern			edlist_t		_kernel_monitor_sigobjlist;
extern			edlist_t		_kernel_monitor_iobuflist;
#endif /* (1 != EXCLUDE_KERNEL_MONITORING) */

#if       (1 != EXCLUDE_IDLETASK_HOOKFUNC)
extern			idletaskhookfunc_ft	_task_idletaskhookfunc_func;
extern			void * 				_task_idletaskhookfunc_arg;
extern			unsigned int		_task_idletaskhookfunc_option;
extern			char				_task_idletaskhookfunc_name[TASK_NAME_SIZE_MAX];
#endif /* (1 != EXCLUDE_IDLETASK_HOOKFUNC) */

#if       (1 != EXCLUDE_TICK_HOOKFUNC)
extern			tickhookfunc_ft		_ubik_tickhookfunc_func;
#endif /* (1 != EXCLUDE_TICK_HOOKFUNC) */

#if       (1 != EXCLUDE_HRTICK)
extern volatile unsigned int	_ubik_hrtick_active;						/* variable representing whether high resolution tick is active or not */
#endif /* (1 != EXCLUDE_HRTICK) */

#ifdef	__cplusplus
}
#endif

#endif /* _UBIK_H_ */
