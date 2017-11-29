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

#if (UBI_CPU_TYPE__CORTEX_M3 != UBI_CPU_TYPE && UBI_CPU_TYPE__CORTEX_M4 != UBI_CPU_TYPE )
	#if ( defined(__thumb__) && (UBI_CPU_TYPE__CORTEX_M3 != UBI_CPU_TYPE && UBI_CPU_TYPE__CORTEX_M4 != UBI_CPU_TYPE) )
		#error "This file has to be compiled with ARM operating state option, '-marm'."
	#endif
#endif

#include "_ubik_arch.h"

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"


void ubik_entercrit(void) {
	if (0 == _ubik_intrcount) {
		ARM_INTERRUPT_DISABLE();
		_ubik_critcount++;
	}
}

void ubik_exitcrit(void) {
	if (0 == _ubik_intrcount) {
		if (0 == _ubik_critcount) {
			dtty_puts("\r\nubik_exitcrit fail (_ubik_critcount is already 0)\r\n", 80);
			bsp_abortsystem();
		}
		_ubik_critcount--;
		if (0 == _ubik_critcount && 0 != _ubik_active) {
			ARM_INTERRUPT_ENABLE();
		}
	}
}

#define save_context() {													\
	__asm__ __volatile__ (													\
		/* Set r3 to task stack */											\
		"mov		r3, sp											\n\t"	\
		"nop														\n\t"	\
																			\
		/* Save r4-r11  into task stack */									\
		"stmfd		r3!, {r4-r11}									\n\t"	\
																			\
		/* Save _ubik_critcount into task stack */							\
		"ldr		r2, =_ubik_critcount							\n\t"	\
		"ldr		r2, [r2]										\n\t"	\
		"stmfd		r3!, {r2}										\n\t"	\
																			\
		/* Update _task_prev->stacktop */									\
		"ldr		r2, =_task_cur									\n\t"	\
		"ldr		r2, [r2]										\n\t"	\
		"add		r2, r2, #4										\n\t"	\
		"str		r3, [r2]										\n\t"	\
																			\
		/* Set sp to r3 */											\
		"mov		sp, r3											\n\t"	\
		"nop														\n\t"	\
	);																		\
}

#define restore_context() {													\
	__asm__ __volatile__ (													\
		/* Set r3 to _task_cur->stacktop */									\
		"ldr		r0, =_task_cur									\n\t"	\
		"ldr		r0, [r0]										\n\t"	\
		"add 		r0, #0x4										\n\t"	\
		"ldr		r3, [r0]										\n\t"	\
		"nop														\n\t"	\
																			\
		/* Restore _ubik_critcount from task stack */						\
		"ldmfd		r3!, {r2}										\n\t"	\
		"ldr		r0, =_ubik_critcount							\n\t"	\
		"str		r2, [r0]										\n\t"	\
																			\
		/* Restore r4-r11 from task stack */								\
		"ldmfd		r3!, {r4-r11}									\n\t"	\
																			\
		/* Set sp to task stack */											\
		"mov		sp, r3											\n\t"	\
		"nop														\n\t"	\
	);																		\
}

#define restore_primask() {													\
	__asm__ __volatile__ (													\
		"cmp		r2, #0x0										\n\t"	\
		"ite		eq												\n\t"	\
		"moveq		r0, #0x0										\n\t"	\
		"movne		r0, #0x1										\n\t"	\
		"msr		primask, r0										\n\t"	\
	);																		\
}

void ubik_irq_handler(void) {
	__asm__ __volatile__ (
		/*Set r0 to exceptiontype */
		"mrs		r0, ipsr										\n\t"
	);

	save_context();

	__asm__ __volatile__ (
		"push		{lr}											\n\t"
		"bl			bsp_exception_handler							\n\t"
		"pop		{lr}											\n\t"
	);

	restore_context();
	restore_primask();

	__asm__ __volatile__ (
		"bx			lr												\n\t"
	);
}

void ubik_swi_handler(void) {
	__asm__ __volatile__ (
		/* Set r1 to task stack */
		"mov		r1, sp											\n\t"

		/*Set r1 to swino */
		"ldr		r1, [r1, #24]									\n\t"
		"ldrb		r1, [r1, #-2]									\n\t"
		/*Set r0 to exceptiontype */
		"mrs		r0, ipsr										\n\t"
	);

	save_context();

	__asm__ __volatile__ (
		"push		{lr}											\n\t"
		"bl			bsp_exception_handler							\n\t"
		"pop		{lr}											\n\t"
	);

	restore_context();
	restore_primask();

	__asm__ __volatile__ (
		"bx			lr												\n\t"
	);
}

void ubik_psv_handler(void) {
	__asm__ __volatile__ (
		/*Set r0 to exceptiontype */
		"mrs		r0, ipsr										\n\t"
	);

	save_context();

	__asm__ __volatile__ (
		"push		{lr}											\n\t"
		"bl			bsp_exception_handler							\n\t"
		"pop		{lr}											\n\t"
	);

	restore_context();
	restore_primask();

	__asm__ __volatile__ (
		"mov		lr, 0xFFFFFFF9									\n\t"
		"bx			lr												\n\t"
	);
}

void task_comp_start(void) {
	_ubik_active = 1;

	_task_cur->state = TASK_STATE__RUNNING;

	_bsp_initfirsttask();

	restore_context();

	__asm__ __volatile__ (
		"ldr		r0, [sp, #28]									\n\t"
		"msr		apsr_nzcvq, r0									\n\t"
		"ldr		r0, [sp, #24]									\n\t"
		"orr		r0, r0, #0x00000001								\n\t"
		"str		r0, [sp, #28]									\n\t"
		"pop		{r0-r3, r12, lr}								\n\t"
		"add		sp, sp, #4										\n\t"
		"pop		{r12}											\n\t"
	);

	arm_set_pendsv();
	ARM_INTERRUPT_ENABLE();
	while (arm_get_pendsv());

	__asm__ __volatile__ (
		"bx			r12												\n\t"
	);
}
