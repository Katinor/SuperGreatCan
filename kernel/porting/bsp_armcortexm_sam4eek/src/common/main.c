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
#include "itf_ubinos/itf/stdlib_porting.h"
#include "itf_ubinos/itf/ubiclib.h"
#include "itf_ubinos/itf/ubiclib_heap.h"
#include "itf_ubinos/itf/ubik.h"
#include "itf_ubinos/itf/ubishell.h"

#include "_bsp.h"

#if (1 != EXCLUDE_MISC)
#else
	#define dtty_puts(str, max)
#endif /* (1 != EXCLUDE_MISC) */

int main(void) {
	unsigned char * buf_p;
	unsigned int heapaddr;
	unsigned int heapsize;
	int r = 0;

	(void) buf_p;
	(void) heapaddr;
	(void) heapsize;
	(void) r;

	r = bsp_comp_init();
	if (0 != r) {
		dtty_puts("bsp_comp_init : fail\r\n", 80);
		bsp_abortsystem();
	}

#if defined(UBI_COMPONENT__lib_ubiclib)
	r = ubiclib_comp_init();
	if (0 != r) {
		dtty_puts("ubiclib_comp_init : fail\r\n", 80);
		bsp_abortsystem();
	}

	#if (0 == UBI_INCLUDE_STDLIB)
		r = bsp_getdefaultheapinfo(&heapaddr, & heapsize);
		if (0 != r) {
			dtty_puts("bsp_getdefaultheapinfo : fail\r\n", 80);
			bsp_abortsystem();
		}
		r = ubiclib_heap_comp_init(heapaddr, heapsize);
		if (HEAP_ERR__UNSUPPORTED == r) {
			dtty_puts("heap_comp_init : unsupported\r\n", 80);
		}
		else if (0 != r) {
			dtty_puts("heap_comp_init : fail\r\n", 80);
			bsp_abortsystem();
		}
	#endif

#endif

#if (0 != UBI_INCLUDE_STDLIB)
	r = stdlib_port_comp_init();
	if (0 != r) {
		dtty_puts("stdlib_port_comp_init : fail\r\n", 80);
		bsp_abortsystem();
	}
#endif

#if defined(UBI_COMPONENT__lib_ubik)
	r = ubik_comp_init(IDLETASK_STACK_SIZE/INT_SIZE);
	if (0 != r) {
		dtty_puts("ubik_comp_init : fail\r\n", 80);
		bsp_abortsystem();
	}
#endif

#if defined(UBI_COMPONENT__lib_ubiclib)
	r = ubiclib_comp_init_reent();
	if (0 != r) {
		dtty_puts("ubiclib_comp_init_reent : fail\r\n", 80);
		bsp_abortsystem();
	}

#if (0 == UBI_INCLUDE_STDLIB)
	r = ubiclib_heap_comp_init_reent();
	if (HEAP_ERR__UNSUPPORTED == r) {
		dtty_puts("heap_comp_init_reent : unsupported\r\n", 80);
	}
	else if (0 != r) {
		dtty_puts("heap_comp_init_reent : fail\r\n", 80);
		bsp_abortsystem();
	}
#endif
#endif

#if (0 != UBI_INCLUDE_STDLIB)
	r = stdlib_porting_comp_init_reent();
	if (0 != r) {
		dtty_puts("stdlib_port_init_reent : fail\r\n", 80);
		bsp_abortsystem();
	}
#endif

#if defined(UBI_COMPONENT__lib_ubishell)
	r = ubishell_comp_init();
	if (0 != r) {
		dtty_puts("ubishell_comp_init : fail\r\n", 80);
		bsp_abortsystem();
	}
#endif

	usrmain(0, 0x0);

	for (;;);

	return 0;
}
