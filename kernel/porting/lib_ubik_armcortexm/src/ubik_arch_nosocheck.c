/*
  Copyright (C) 2011 RTLab, Yu Jin Park, Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the lib_ubik_armcortexm component of the Ubinos.

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

#if ( (defined(__SOCHECK__) && !defined(__NOSOCHECK__)) || (defined(__SUCHECK__) && !defined(__NOSUCHECK__)) )
	#error "This file has to be compiled without stack overflow check and stack usage check option."
#endif

#include "_ubik_arch.h"

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"
#include "itf_ubinos/itf/ubiclib.h"
#include "itf_ubinos/itf/ubiclib_logm.h"
#include "itf_ubinos/itf/ubiclib_heap.h"
#include "itf_ubinos/itf/ubik.h"
#include "itf_ubinos/itf/ubik_task.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int task_getstacksize_nosocheck(task_pt _task, unsigned int * stacksize_p) {
	#define	__FUNCNAME__	"task_getstacksize_nosocheck"
	int r;
	_task_pt task = (_task_pt) _task;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	if (NULL == stacksize_p) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	if (NULL == task && 0 == _ubik_active) {
		r = bsp_getstacksize_nosocheck(0, stacksize_p);
		goto end0;
	}

	if (NULL == task) {
		task = _task_cur;
	}

	ubik_entercrit_nosocheck();

	if (0 == task->valid || (OBJTYPE__UBIK_TASK != task->type && OBJTYPE__UBIK_IDLETASK != task->type)) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	*stacksize_p = task->stacksize;

	r = 0;

end1:
	ubik_exitcrit_nosocheck();

end0:
	return r;
	#undef __FUNCNAME__
}

int task_getmaxstackusage_nosocheck(task_pt _task, unsigned int * maxstackusage_p) {
#if (1 == UBI_SUCHECK)

	#define	__FUNCNAME__	"task_getmaxstackusage_nosocheck"
	int r;
	_task_pt task = (_task_pt) _task;

	if (NULL == _task_cur) {
		logme(""__FUNCNAME__": ubik is not initialized\r\n");
		r = -1;
		goto end0;
	}

	if (NULL == maxstackusage_p) {
		logme(""__FUNCNAME__": parameter 2 is wrong\r\n");
		r = -3;
		goto end0;
	}

	if (NULL == task && 0 == _ubik_active) {
		r = bsp_getmaxstackusage_nosocheck(0, maxstackusage_p);
		goto end0;
	}

	if (NULL == task) {
		task = _task_cur;
	}

	ubik_entercrit_nosocheck();

	if (0 == task->valid || (OBJTYPE__UBIK_TASK != task->type && OBJTYPE__UBIK_IDLETASK != task->type)) {
		logme(""__FUNCNAME__": parameter 1 is wrong\r\n");
		r = -2;
		goto end1;
	}

	if (_task_cur == task && 0 == _ubik_intrcount && 1 == _ubik_active) {
		task->stacktop_max = (unsigned int *) _sucheck_stacktop_max;
	}
	*maxstackusage_p = (unsigned int) task->stack + task->stacksize
			- (unsigned int) task->stacktop_max
			+ bsp_getsocheckmargin_nosocheck() + ARM_CONTEXT_SIZE;

	r = 0;

end1:
	ubik_exitcrit_nosocheck();

end0:
	return r;
	#undef __FUNCNAME__

#else
	if (NULL != maxstackusage_p) {
		*maxstackusage_p = 0;
	}

	return -1;
#endif
}
