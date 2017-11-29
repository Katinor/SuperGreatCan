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

#include <stdio.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"
#include "itf_ubinos/itf/ubiclib_logm.h"

#include "_bsp_arch.h"

int bsp_getdefaultheapinfo(unsigned int * heapaddr_p,  unsigned int * heapsize_p) {
	unsigned int heapaddr;
	unsigned int heapsize;

	if (NULL == heapaddr_p) {
		return -2;
	}

	if (NULL == heapsize_p) {
		return -3;
	}

	heapaddr = (unsigned int) &_end;
	heapsize = ((unsigned int) &_stack_top) \
			- ABT_STACK_SIZE \
			- UND_STACK_SIZE \
			- FIQ_STACK_SIZE \
			- IRQ_STACK_SIZE \
			- SVC_STACK_SIZE \
			- SYS_STACK_SIZE \
			- heapaddr;

	if ((((unsigned int) &_stack_top) - heapaddr) < heapsize) {
		return BSP_ERR__INVALID_HEAPINFO;
	}

	*heapaddr_p = heapaddr;
	*heapsize_p = heapsize;

	return 0;
}

		volatile unsigned int * _svc_stack = (unsigned int *) -1;

#if (1 == UBI_SOCHECK)
		volatile unsigned int * _svc_stacklimit = (unsigned int *) -1;
	#if (2 != UBI_SOCHECK_TYPE)
		volatile unsigned int * _socheck_stacklimit = (unsigned int *) -1;
	#endif
#endif

#if (1 == UBI_SUCHECK)
		volatile unsigned int * _svc_stacktop_max = (unsigned int *) -1;
		volatile unsigned int * _sucheck_stacktop_max = (unsigned int *) -1;
#endif

#if (1 != EXCLUDE_MISC)

unsigned int bsp_getsocheckmargin(void) {
	return SOCHECK_MARGIN;
}

int bsp_getstacksize(int type, unsigned int * stacksize_p) {
	if (NULL == stacksize_p) {
		return -3;
	}

	switch (type) {
	case 0:
		*stacksize_p = (SVC_STACK_SIZE + ABT_STACK_SIZE + UND_STACK_SIZE + FIQ_STACK_SIZE + IRQ_STACK_SIZE + SYS_STACK_SIZE);
		return 0;

	case 1:
		*stacksize_p = SVC_STACK_SIZE;
		return 0;

	default:
		return -2;
	}
}

int bsp_getmaxstackusage(int type, unsigned int * maxstackusage_p) {
#if (1 == UBI_SUCHECK)
	if (NULL == maxstackusage_p) {
		return -3;
	}

	switch (type) {
	case 0:
#if defined(UBI_COMCONFIG__lib_ubik)
		if (0 < _ubik_intrcount || 0 == _ubik_active) {
			_svc_stacktop_max = _sucheck_stacktop_max;
		}
#else
		_svc_stacktop_max = _sucheck_stacktop_max;
#endif

		*maxstackusage_p = (ABT_STACK_SIZE + UND_STACK_SIZE + FIQ_STACK_SIZE + IRQ_STACK_SIZE + SYS_STACK_SIZE)
			+ ((unsigned int) _svc_stack - (unsigned int) _svc_stacktop_max)
			+  SOCHECK_MARGIN + ARM_CONTEXT_SIZE;

		return 0;

	case 1:
#if defined(UBI_COMCONFIG__lib_ubik)
		if (0 < _ubik_intrcount || 0 == _ubik_active) {
			_svc_stacktop_max = _sucheck_stacktop_max;
		}
#else
		_svc_stacktop_max = _sucheck_stacktop_max;
#endif

		*maxstackusage_p = ((unsigned int) _svc_stack - (unsigned int) _svc_stacktop_max)
			+  SOCHECK_MARGIN + ARM_CONTEXT_SIZE;

		return 0;

	default:
		return -2;
	}
#else
	if (NULL != maxstackusage_p) {
		*maxstackusage_p = 0;
	}

	return -1;
#endif
}

#endif /* (1 != EXCLUDE_MISC) */

#if (1 != EXCLUDE_SPRINTFMEMMAP)

int bsp_sprintmemmap(char * buf, int max) {
	int r = 0;
	unsigned int size = 0;
	unsigned int rom = 0;
	unsigned int ram = 0;
	unsigned int ramatrom = 0;
	unsigned int sysstack = 0;

	if (NULL == buf) {
		return -2;
	}

	if (max <= 0) {
		return 0;
	}

	extern char exc_vectors_start 	__asm__ ("__exc_vectors_start__");
	extern char exc_vectors_end 	__asm__ ("__exc_vectors_end__");
	size = (unsigned int) &exc_vectors_end - (unsigned int) &exc_vectors_start;
	if (r < max) r += snprintf(&buf[r], max-r, "exc_vectors section start            : 0x%08x\r\n", (unsigned int) &exc_vectors_start);
	if (r < max) r += snprintf(&buf[r], max-r, "exc_vectors section size             : 0x%08x (%8d bytes)\r\n", size, size);
	rom += size;

	extern char startup_start 		__asm__ ("__startup_start__");
	extern char startup_end 		__asm__ ("__startup_end__");
	size = (unsigned int) &startup_end - (unsigned int) &startup_start;
	if (r < max) r += snprintf(&buf[r], max-r, "startup section start                : 0x%08x\r\n", (unsigned int) &startup_start);
	if (r < max) r += snprintf(&buf[r], max-r, "startup section size                 : 0x%08x (%8d bytes)\r\n", size, size);
	rom += size;

	extern char text_start 			__asm__ ("__text_start__");
	extern char text_end 			__asm__ ("__text_end__");
	size = (unsigned int) &text_end - (unsigned int) &text_start;
	if (r < max) r += snprintf(&buf[r], max-r, "text section start                   : 0x%08x\r\n", (unsigned int) &text_start);
	if (r < max) r += snprintf(&buf[r], max-r, "text section size                    : 0x%08x (%8d bytes)\r\n", size, size);
	rom += size;

	extern char rodata_start	 	__asm__ ("__rodata_start__");
	extern char rodata_end 			__asm__ ("__rodata_end__");
	size = (unsigned int) &rodata_end - (unsigned int) &rodata_start;
	if (r < max) r += snprintf(&buf[r], max-r, "rodata section start                 : 0x%08x\r\n", (unsigned int) &rodata_start);
	if (r < max) r += snprintf(&buf[r], max-r, "rodata section size                  : 0x%08x (%8d bytes)\r\n", size, size);
	rom += size;

	if (r < max) r += snprintf(&buf[r], max-r, "\r\n");

	extern char data_start 			__asm__ ("__data_start__");
	extern char data_end 			__asm__ ("__data_end__");
	size = (unsigned int) &data_end - (unsigned int) &data_start;
	if (r < max) r += snprintf(&buf[r], max-r, "data section start                   : 0x%08x\r\n", (unsigned int) &data_start);
	if (r < max) r += snprintf(&buf[r], max-r, "data section size                    : 0x%08x (%8d bytes)\r\n", size, size);
	ramatrom += size;

	if (r < max) r += snprintf(&buf[r], max-r, "\r\n");

	extern char bss_start 			__asm__ ("__bss_start__");
	extern char bss_end 			__asm__ ("__bss_end__");
	size = (unsigned int) &bss_end - (unsigned int) &bss_start;
	if (r < max) r += snprintf(&buf[r], max-r, "bss section start                    : 0x%08x\r\n", (unsigned int) &bss_start);
	if (r < max) r += snprintf(&buf[r], max-r, "bss section size                     : 0x%08x (%8d bytes)\r\n", size, size);
	ram += size;

	extern char common_start 		__asm__ ("__common_start__");
	extern char common_end 			__asm__ ("__common_end__");
	size = (unsigned int) &common_end - (unsigned int) &common_start;
	if (r < max) r += snprintf(&buf[r], max-r, "common section start                 : 0x%08x\r\n", (unsigned int) &common_start);
	if (r < max) r += snprintf(&buf[r], max-r, "common section size                  : 0x%08x (%8d bytes)\r\n", size, size);
	ram += size;

	if (r < max) r += snprintf(&buf[r], max-r, "\r\n");

	sysstack = ABT_STACK_SIZE + UND_STACK_SIZE + FIQ_STACK_SIZE + IRQ_STACK_SIZE + SVC_STACK_SIZE + SYS_STACK_SIZE;
	if (r < max) r += snprintf(&buf[r], max-r, "system stack size                    : 0x%08x (%8d bytes)\r\n", sysstack, sysstack);

	if (r < max) r += snprintf(&buf[r], max-r, "\r\n");

	#if (MEMMAP__SRAM == MEMMAP)
	rom += ramatrom;
	ram += rom;
	ram += sysstack;
	#else
	rom += ramatrom;
	ram += ramatrom;
	ram += sysstack;
	#endif

	if (r < max) r += snprintf(&buf[r], max-r, "rom usage                            : 0x%08x (%8d bytes)\r\n", rom, rom);
	if (r < max) r += snprintf(&buf[r], max-r, "ram usage (static)                   : 0x%08x (%8d bytes)\r\n", ram, ram);

	return r;
}

#endif /* (1 != EXCLUDE_SPRINTFMEMMAP) */

#if (1 != EXCLUDE_PRINTFMEMMAP)

int bsp_printmemmap(void) {
	unsigned int size = 0;
	unsigned int rom = 0;
	unsigned int ram = 0;
	unsigned int ramatrom = 0;
	unsigned int sysstack = 0;

	extern char exc_vectors_start 	__asm__ ("__exc_vectors_start__");
	extern char exc_vectors_end 	__asm__ ("__exc_vectors_end__");
	size = (unsigned int) &exc_vectors_end - (unsigned int) &exc_vectors_start;
	printf("exc_vectors section start            : 0x%08x\r\n", (unsigned int) &exc_vectors_start);
	printf("exc_vectors section size             : 0x%08x (%8d bytes)\r\n", size, size);
	rom += size;

	extern char startup_start 		__asm__ ("__startup_start__");
	extern char startup_end 		__asm__ ("__startup_end__");
	size = (unsigned int) &startup_end - (unsigned int) &startup_start;
	printf("startup section start                : 0x%08x\r\n", (unsigned int) &startup_start);
	printf("startup section size                 : 0x%08x (%8d bytes)\r\n", size, size);
	rom += size;

	extern char text_start 			__asm__ ("__text_start__");
	extern char text_end 			__asm__ ("__text_end__");
	size = (unsigned int) &text_end - (unsigned int) &text_start;
	printf("text section start                   : 0x%08x\r\n", (unsigned int) &text_start);
	printf("text section size                    : 0x%08x (%8d bytes)\r\n", size, size);
	rom += size;

	extern char rodata_start	 	__asm__ ("__rodata_start__");
	extern char rodata_end 			__asm__ ("__rodata_end__");
	size = (unsigned int) &rodata_end - (unsigned int) &rodata_start;
	printf("rodata section start                 : 0x%08x\r\n", (unsigned int) &rodata_start);
	printf("rodata section size                  : 0x%08x (%8d bytes)\r\n", size, size);
	rom += size;

	printf("\r\n");

	extern char data_start 			__asm__ ("__data_start__");
	extern char data_end 			__asm__ ("__data_end__");
	size = (unsigned int) &data_end - (unsigned int) &data_start;
	printf("data section start                   : 0x%08x\r\n", (unsigned int) &data_start);
	printf("data section size                    : 0x%08x (%8d bytes)\r\n", size, size);
	ramatrom += size;

	printf("\r\n");

	extern char bss_start 			__asm__ ("__bss_start__");
	extern char bss_end 			__asm__ ("__bss_end__");
	size = (unsigned int) &bss_end - (unsigned int) &bss_start;
	printf("bss section start                    : 0x%08x\r\n", (unsigned int) &bss_start);
	printf("bss section size                     : 0x%08x (%8d bytes)\r\n", size, size);
	ram += size;

	extern char common_start 		__asm__ ("__common_start__");
	extern char common_end 			__asm__ ("__common_end__");
	size = (unsigned int) &common_end - (unsigned int) &common_start;
	printf("common section start                 : 0x%08x\r\n", (unsigned int) &common_start);
	printf("common section size                  : 0x%08x (%8d bytes)\r\n", size, size);
	ram += size;

	printf("\r\n");

	sysstack = ABT_STACK_SIZE + UND_STACK_SIZE + FIQ_STACK_SIZE + IRQ_STACK_SIZE + SVC_STACK_SIZE + SYS_STACK_SIZE;
	printf("system stack size                    : 0x%08x (%8d bytes)\r\n", sysstack, sysstack);

	printf("\r\n");

	#if (MEMMAP__SRAM == MEMMAP)
	rom += ramatrom;
	ram += rom;
	ram += sysstack;
	#else
	rom += ramatrom;
	ram += ramatrom;
	ram += sysstack;
	#endif

	printf("rom usage                            : 0x%08x (%8d bytes)\r\n", rom, rom);
	printf("ram usage (static)                   : 0x%08x (%8d bytes)\r\n", ram, ram);

	return 0;
}

#endif /* (1 != EXCLUDE_PRINTFMEMMAP) */
