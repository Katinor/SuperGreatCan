/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the lib_ubiclib component of the Ubinos.

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

#if (UBI_BUILDMODE__DEBUG == UBI_BUILDMODE)

#include <stdarg.h>
#include <stdio.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"
#include "itf_ubinos/itf/ubiclib.h"
#include "itf_ubinos/itf/ubiclib_logm.h"
#include "_ubiclib_vprintfn.h"

#define FORMAT_SIZE_MAX		1024
#define OUTPUT_SIZE_MAX		1024
#define DIGIT_SIZE_MAX		128

volatile int	_ubiclib_logm_level[LOGM_CATEGORY__END] = {0, };

const char * 	_ubiclib_logm_levelname[LOGM_LEVEL__END] = {
		"[none   ] ",
		"[always ] ",
		"[fatal  ] ",
		"[error  ] ",
		"[warning] ",
		"[info   ] ",
		"[debug  ] ",
};

int logm_setlevel(int category, int level) {
	int i;

	if (0 > category || LOGM_CATEGORY__END <= category) {
		return -2;
	}

	if (0 > level || LOGM_LEVEL__END <= level) {
		return -3;
	}

	if (LOGM_CATEGORY__ALL == category) {
		for (i=0; i<LOGM_CATEGORY__END; i++) {
			_ubiclib_logm_level[i] = level;
		}
	}
	else {
		_ubiclib_logm_level[category] = level;
	}

	return 0;
}

int logm_getlevel(int category) {
	if (0 > category || LOGM_CATEGORY__ALL <= category) {
		return -2;
	}

	return _ubiclib_logm_level[category];
}

int logm_printf(int category, int level, const char * tag, const char * format, ...) {
#if (1 != EXCLUDE_PRINTF)
	int n = 0;

	va_list ap;

	if (0 > category || LOGM_CATEGORY__END <= category) {
		return -2;
	}

	if (_ubiclib_logm_level[category] < level) {
		return 0;
	}

	if (NULL != tag) {
		n += dtty_puts(_ubiclib_logm_levelname[level], 128);
		n += dtty_puts(tag , 128);
		n += dtty_puts(": ", 128);
	}

	va_start(ap, format);

	n += vfprintfn(SYS_OUT__DTTY, 0x0, OUTPUT_SIZE_MAX, format, ap);

	va_end(ap);

	n += dtty_puts("\r\n", 128);

	return n;
#else
	int n = 0;

	if (0 > category || LOGM_CATEGORY__END <= category) {
		return -2;
	}

	if (_ubiclib_logm_level[category] < level) {
		return 0;
	}

	if (NULL != tag) {
		n += dtty_puts(_ubiclib_logm_levelname[level], 128);
		n += dtty_puts(tag , 128);
		n += dtty_puts(": ", 128);
	}

	n += dtty_puts_nosocheck(format, 120);

	n += dtty_puts("\r\n", 128);

	return n;
#endif
}

#endif
