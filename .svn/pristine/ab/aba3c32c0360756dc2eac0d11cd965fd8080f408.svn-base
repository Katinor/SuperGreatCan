/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the lib_newlib_port_arm component of the Ubinos.

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

#if (0 != UBI_INCLUDE_STDLIB)

//#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
//#include <reent.h>

#include "itf_ubinos/itf/stdlib_porting.h"
#include "itf_ubinos/itf/bsp_dtty.h"
#include "itf_ubinos/itf/ubinos.h"

mutex_pt _malloc_mutex = NULL;

extern void _EXFUN(__sinit,(struct _reent *));

int stdlib_port_comp_init(void) {
    if (0 == _impure_ptr) {
    	return -1;
    }

	__sinit(_impure_ptr);

	_impure_ptr->_stdin->_flags |= __SNBF;
	_impure_ptr->_stdout->_flags |= __SNBF;
	_impure_ptr->_stderr->_flags |= __SNBF;

	return 0;
}

caddr_t _sbrk(int incr) {
	extern char stack_top	__asm__ ("__stack_top__");	/* Defined by the linker.  */
	extern char end			__asm__ ("__end__"); 		/* Defined by the linker.  */
	static char * heap_end = NULL;
	char * heap_end_max = 0;
	char * prev_heap_end = 0;

	if (NULL == heap_end) {
		heap_end = &end;
	}
	heap_end_max = &stack_top;
	prev_heap_end = heap_end;

	if (heap_end + incr > heap_end_max) {
		errno = ENOMEM;
		return (caddr_t) - 1;
	}

	heap_end += incr;

	return (caddr_t) prev_heap_end;
}

int isatty(int fd) {
	return 1;
}

int _close(int file) {
	return 0;
}

int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;

	return 0;
}

int _lseek(int file, int ptr, int dir) {
	return 0;
}

int _read(int file, char * ptr, int len) {
	int i = 0;

	for (i = 0; i < len; i++) {
		dtty_getc(&ptr[i]);
	}

	return i;
}

int _write(int file, char * ptr, int len) {
	int i = 0;

	for (i = 0; i < len; i++) {
		dtty_putc(ptr[i]);
	}

	return i;
}

int stdlib_porting_comp_init_reent(void) {
	int r = 0;

	r = mutex_create(&_malloc_mutex);
	if (0 > r) {
		return -1;
	}

	return 0;
}

int stdlib_malloc_lock(void) {
	if (NULL != _malloc_mutex) {
		return mutex_lock(_malloc_mutex);
	}

	return 0;
}

int stdlib_malloc_unlock(void) {
	if (NULL != _malloc_mutex) {
		return mutex_unlock(_malloc_mutex);
	}

	return 0;
}

void * _sbrk_r(struct _reent * re, ptrdiff_t incr) {
	return _sbrk(incr);
}

int _close_r(struct _reent * re, int file) {
	return 0;
}

int _fstat_r(struct _reent * re, int file, struct stat * st) {
	st->st_mode = S_IFCHR;

	return 0;
}

_off_t _lseek_r(struct _reent * re, int file, _off_t ptr, int dir) {
	return 0;
}

_ssize_t _read_r(struct _reent * re, int file, void * ptr, size_t len) {
	int i = 0;

	for (i = 0; i < len; i++) {
		dtty_getc(&((char *)ptr)[i]);
	}

	return i;
}

_ssize_t _write_r(struct _reent * re, int file, const void * ptr, size_t len) {
	int i = 0;

	for (i = 0; i < len; i++) {
		dtty_putc(((char *)ptr)[i]);
	}

	return i;
}

#undef putchar

int putchar(int c) {
	int r = 0;

	r = dtty_putc(c);

	if (0 == r) {
		return c;
	}
	else {
		return EOF;
	}
}

#else

#include "itf_ubinos/itf/stdlib_porting.h"
#include "itf_ubinos/itf/ubinos.h"

int stdlib_port_comp_init(void) {
	return 0;
}

int stdlib_port_comp_init_reent(void) {
	return 0;
}

#endif /* (0 != UBI_INCLUDE_STDLIB) */
