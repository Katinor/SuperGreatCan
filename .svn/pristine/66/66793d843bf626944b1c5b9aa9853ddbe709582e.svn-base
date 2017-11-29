/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the bsp_arm... component of the Ubinos.

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

#include "../../ubiconfig.h"

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"

#include "_bsp.h"

#if defined(UBI_COMCONFIG__lib_ubik)
#else
		unsigned int _bsp_intrcount = 0;
#endif

int bsp_comp_init(void) {
	int r = 0;

	r = dtty_init();
	if (0 != r) {
		return -1;
	}

	return 0;
}

#if (1 != EXCLUDE_MISC)
/* 2015. 06. 05. RTLAB_GYLee */
/* add us busywaiting*/
/* ms */
void bsp_busywaitms(unsigned int timems) {
	unsigned int count;

	count = bsp_timemstobwc(timems);
	if (count < timems) {
		count = UINT_MAX;
	}

	bsp_busywait(count);
}

unsigned int bsp_getbusywaitcountperms(void) {
	return BUSYWAITCOUNT_PER_MS;
}

unsigned int bsp_timemstobwc(unsigned int timems) {
	return (timems * BUSYWAITCOUNT_PER_MS);
}

unsigned int bsp_bwctotimems(unsigned int count) {
	return (count / BUSYWAITCOUNT_PER_MS);
}
/* us */
void bsp_busywaitus(unsigned int timems) {
	unsigned int count;

	count = bsp_timeustobwc(timems);
	if (count < timems) {
		count = UINT_MAX;
	}

	bsp_busywait(count);
}

unsigned int bsp_getbusywaitcountperus(void) {
	return BUSYWAITCOUNT_PER_US;
}

unsigned int bsp_timeustobwc(unsigned int timems) {
	return (timems * BUSYWAITCOUNT_PER_US);
}

unsigned int bsp_bwctotimeus(unsigned int count) {
	return (count / BUSYWAITCOUNT_PER_US);
}


#endif /* (1 != EXCLUDE_MISC) */
