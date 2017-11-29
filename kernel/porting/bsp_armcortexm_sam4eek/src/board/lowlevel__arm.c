/*
  Copyright (C) 2011 RTLab
  Contact: ubinos.org@gmail.com

  Contributors: Dong Hoon Kim, Sung Ho Park

  This file is part of the bsp_armcortexm_sam3sek component of the Ubinos.

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

#if ( defined(__thumb__) && (UBI_CPU_TYPE__CORTEX_M3 != UBI_CPU_TYPE && UBI_CPU_TYPE__CORTEX_M4 != UBI_CPU_TYPE) )
	#error "This file has to be compiled with ARM operating state option, '-marm'."
#endif

#if ( (defined(__SOCHECK__) && !defined(__NOSOCHECK__)) || (defined(__SUCHECK__) && !defined(__NOSUCHECK__)) )
	#error "This file has to be compiled without stack overflow check and stack usage check option."
#endif

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"

#include "_bsp_board.h"

#define CLOCK_TIMEOUT	5000

void lowlevel_init(void) {
	unsigned int * dst_p;
	unsigned int * src_p;

	/* Set 1 FWS for Embedded Flash Access */
	EFC->EEFC_FMR = (10 << 8);
	ARM_DATASYNC();

	/* Disable watchdog */
	WDT->WDT_MR = 		WDT_MR_WDDIS;
	ARM_DATASYNC();

	/* Enable NRST reset */
	RSTC->RSTC_MR =		((unsigned int) 0xA5 << 24) |
						RSTC_MR_URSTEN;
	ARM_DATASYNC();

	/* Select external slow clock */
	/* 32KHz */
	unsigned int timeout = 0;
	if ((SUPC->SUPC_SR & SUPC_SR_OSCSEL) != SUPC_SR_OSCSEL_CRYST) {
	SUPC->SUPC_CR =		((unsigned int) 0xA5 << 24) |
						SUPC_CR_XTALSEL_CRYSTAL_SEL;
	ARM_DATASYNC();
	while (!(SUPC->SUPC_SR & SUPC_SR_OSCSEL_CRYST) && (timeout++ == CLOCK_TIMEOUT));
	}

	PMC->PMC_WPMR = 0x504D4300;
	/* Initialize main oscillator */
	/* 12MHz */
	/* Start up time : 8 * 1/32K * 48 = 12ms */
	//Edit by YuJin Park
	if(!(PMC->CKGR_MOR & CKGR_MOR_MOSCSEL)) {
	PMC->CKGR_MOR =		((unsigned int) 0x37 << 16) |
						((unsigned int) 0x08/*0x30*/ <<  8) |
						CKGR_MOR_MOSCRCEN |
						CKGR_MOR_MOSCXTEN;
	ARM_DATASYNC();
	while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));
	}

	/* Switch main clock to 3-20MHz Xtal oscillator */
	PMC->CKGR_MOR = 	((unsigned int) 0x37 << 16) |
						((unsigned int) CKGR_MOR_MOSCXTST(0x8) & (0x8 << 8))/*((unsigned int) 0x30 <<  8)*/ |
						CKGR_MOR_MOSCRCEN |
						CKGR_MOR_MOSCXTEN |
						CKGR_MOR_MOSCSEL;
	ARM_DATASYNC();
	while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));

	/* Switch master clock to main clock */
	ARM_DATASYNC();
	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
	/* Initialize PLLA */
	/* 12M * (7+1) / 1 = 96MHz */
	/* Lock time : 8 * 1/32K * 8 = 2ms */
	PMC->CKGR_PLLAR =   ((1 << 29) | (0x7 << 16) | (0x01 << 8)/*(0x8 << 8)*/ | (0x1 << 0));
	ARM_DATASYNC();
	while (!(PMC->PMC_SR & PMC_SR_LOCKA));

	/* Switch master clock to fast clock */
	/* PLLA / 2 = 48Mhz */
//	PMC->PMC_MCKR = 	PMC_MCKR_PRES_CLK_2 |
//						PMC_MCKR_CSS_MAIN_CLK;
//	ARM_DATASYNC();
//	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
//	PMC->PMC_MCKR = 	PMC_MCKR_PRES_CLK_2 |
//						PMC_MCKR_CSS_PLLA_CLK;
	PMC->PMC_MCKR = ((PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK) & ~PMC_MCKR_CSS_Msk)  | PMC_MCKR_CSS_MAIN_CLK;
	ARM_DATASYNC();
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

	PMC->PMC_MCKR = (PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK);
	ARM_DATASYNC();
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

	/* Copy exception vectors into ram */
	extern unsigned int exc_vectors_dst_start	__asm__ ("__exc_vectors_dst_start__");
	extern unsigned int exc_vectors_src_start	__asm__ ("__exc_vectors_src_start__");
	extern unsigned int exc_vectors_src_end 	__asm__ ("__exc_vectors_src_end__");
	dst_p = &exc_vectors_dst_start;
	src_p = &exc_vectors_src_start;
	if (dst_p != src_p) {
		while (src_p < &exc_vectors_src_end) {
			*(dst_p++) = *(src_p++);
		}
	}

	/* Copy segments into ram */
	extern unsigned int ram_section_dst_start	__asm__ ("__ram_section_dst_start__");
	extern unsigned int ram_section_src_start	__asm__ ("__ram_section_src_start__");
	extern unsigned int ram_section_src_end 	__asm__ ("__ram_section_src_end__");
	dst_p = &ram_section_dst_start;
	src_p = &ram_section_src_start;
	if (dst_p != src_p) {
		while (src_p < &ram_section_src_end) {
			*(dst_p++) = *(src_p++);
		}
	}

	/* Initialize the bss segment */
	extern unsigned int bss_start	__asm__ ("__bss_start__");
	extern unsigned int bss_end		__asm__ ("__bss_end__");
	dst_p = &bss_start;
	while (dst_p < &bss_end) {
		*(dst_p++) = 0x00000000;
	}

#if (1 != EXCLUDE_COMMON_SEGMENT_INIT)
	/* Initialize the common segment */
	extern unsigned int common_start	__asm__ ("__common_start__");
	extern unsigned int common_end		__asm__ ("__common_end__");
	dst_p = &common_start;
	while (dst_p < &common_end) {
		*(dst_p++) = 0x00000000;
	}
#endif

#if (1 != EXCLUDE_SEGMENT_END_BOUNDARY_CHECK)
	/* Init segment end boundary value */
	_data_segment_end_boundary = SEGMENT_END_BOUNDARY_VALUE;
	_bss_segment_end_boundary = SEGMENT_END_BOUNDARY_VALUE;
	_common_segment_end_boundary = SEGMENT_END_BOUNDARY_VALUE;
#endif
	ARM_DATASYNC();

	/* Set the vector table base address */
	extern unsigned int exc_vectors_start __asm__ ("__exc_vectors_start__");
	SCB->VTOR = ((unsigned int) &exc_vectors_start) & 0x3FFFFF80;
	ARM_DATASYNC();


}
