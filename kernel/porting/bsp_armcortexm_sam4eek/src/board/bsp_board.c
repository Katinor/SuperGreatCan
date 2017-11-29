/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

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

#include <stdio.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"

#include "_bsp_board.h"

#if (1 != EXCLUDE_MISC)

int bsp_getcpuid(unsigned char * buf, int max) {
	unsigned char tmp_a[4];

	if (NULL == buf) {
		return -2;
	}
	if (4 > max) {
		return -3;
	}

	#undef CHIPID_CIDR
	*((unsigned int *) tmp_a) = (CHIPID->CHIPID_CIDR);

#if (UBI_LITTLE_ENDIAN == 1)
	buf[0] = tmp_a[3];
	buf[1] = tmp_a[2];
	buf[2] = tmp_a[1];
	buf[3] = tmp_a[0];
#else
	buf[0] = tmp_a[0];
	buf[1] = tmp_a[1];
	buf[2] = tmp_a[2];
	buf[3] = tmp_a[3];
#endif

	return 4;
}

int bsp_getcpuidsize(void) {
	return 4;
}

int bsp_getcpuclockfreq(unsigned int * freq_p) {
	if (NULL == freq_p) {
		return -2;
	}

    return -1;
}

int bsp_getcpuclockfreqk(unsigned int * freqk_p) {
    unsigned int mclock_sel = 0;
    unsigned int mclock_div = 0;
    unsigned int plla_mult = 0;
    unsigned int plla_div = 0;

    if (NULL == freqk_p) {
		return -2;
    }

    mclock_sel = (PMC->PMC_MCKR) & 0x03;
    switch (mclock_sel) {
    case 0:	// slow clock
    	*freqk_p = SLOW_CLOCK_FREQ;
    	return 0;
    case 1:	// main clock
    	*freqk_p =  MAIN_CLOCK_FREQ;
    	return 0;
    case 3: // PLLB clock
    	break;
    case 2: // PLLA clock
    	break;
    default:
    	return -1;
    }

    mclock_div = (PMC->PMC_MCKR >> 4) & 0x07;
    mclock_div = 1 << mclock_div;
    if (128 <= mclock_div) {
        return -1; // reserved
    }

    plla_mult   = (PMC->CKGR_PLLAR & 0x07FF0000) >> 16;
    plla_div    = (PMC->CKGR_PLLAR & 0x000000FF) >>  0;
    if (plla_div == 0) {
        return -1; // divider output is 0
    }
    /* 48MHz = (( 12MHz * (7 + 1) / 1 ) / 2) */
    *freqk_p =  ((MAIN_CLOCK_FREQ * (plla_mult + 1)) / plla_div) / mclock_div;
    return 0;
}

int bsp_getmckfreq(unsigned int * freq_p) {
	return bsp_getcpuclockfreq(freq_p);
}

int bsp_getmckfreqk(unsigned int * freqk_p) {
	return bsp_getcpuclockfreqk(freqk_p);
}

#endif /* (1 != EXCLUDE_MISC) */


#if (CPU_MODEL == CPU_MODEL__SAM3S4C)

	/* EFC command */
	#define EFC_FCMD_GETD    0x00
	#define EFC_FCMD_WP      0x01
	#define EFC_FCMD_WPL     0x02
	#define EFC_FCMD_EWP     0x03
	#define EFC_FCMD_EWPL    0x04
	#define EFC_FCMD_EA      0x05
	#define EFC_FCMD_SLB     0x08
	#define EFC_FCMD_CLB     0x09
	#define EFC_FCMD_GLB     0x0A
	#define EFC_FCMD_SFB     0x0B
	#define EFC_FCMD_CFB     0x0C
	#define EFC_FCMD_GFB     0x0D
	#define EFC_FCMD_STUI    0x0E
	#define EFC_FCMD_SPUI    0x0F

	#define MC0_FMR_ADDR				((volatile unsigned int *) &(EFC->EEFC_FMR))
	#define MC0_FCR_ADDR				((volatile unsigned int *) &(EFC->EEFC_FCR))
	#define MC0_FSR_ADDR				((volatile unsigned int *) &(EFC->EEFC_FSR))
	#define MC0_FRR_ADDR				((volatile unsigned int *) &(EFC->EEFC_FRR))

	#define AT91C_MC_FCMD_START_PROG	EFC_FCMD_EWP
	#define AT91C_MC_FCMD_ERASE_ALL		EFC_FCMD_EA
	#define AT91C_MC_FCMD_SET_GP_NVM	EFC_FCMD_SFB
	#define AT91C_MC_FCMD_CLR_GP_NVM	EFC_FCMD_CFB
	#define AT91C_MC_FCMD_GET_GP_NVM	EFC_FCMD_GFB

	#define AT91C_MC_FRDY				EEFC_FSR_FRDY
	#define AT91C_MC_KEY 				EEFC_FCR_FKEY
	#define AT91C_MC_PROGE				EEFC_FSR_FCMDE
	#define AT91C_MC_LOCKE 				EEFC_FSR_FLOCKE
	#define AT91C_MC_PROGE				EEFC_FSR_FCMDE
	#define AT91C_MC_LOCKE 				EEFC_FSR_FLOCKE

#elif (CPU_MODEL == CPU_MODEL__SAM4SD32C)

	/* EFC command */
	#define EFC_FCMD_GETD    0x00
	#define EFC_FCMD_WPL     0x02
	#define EFC_FCMD_EWP     0x03
	#define EFC_FCMD_EWPL    0x04
	#define EFC_FCMD_EA      0x05
	#define EFC_FCMD_SLB     0x08
	#define EFC_FCMD_CLB     0x09
	#define EFC_FCMD_GLB     0x0A
	#define EFC_FCMD_SFB     0x0B
	#define EFC_FCMD_CFB     0x0C
	#define EFC_FCMD_GFB     0x0D
	#define EFC_FCMD_STUI    0x0E
	#define EFC_FCMD_SPUI    0x0F

	#define MC0_FMR_ADDR				((volatile unsigned int *) &(EFC->EEFC_FMR))
	#define MC0_FCR_ADDR				((volatile unsigned int *) &(EFC->EEFC_FCR))
	#define MC0_FSR_ADDR				((volatile unsigned int *) &(EFC->EEFC_FSR))
	#define MC0_FRR_ADDR				((volatile unsigned int *) &(EFC->EEFC_FRR))

	#define AT91C_MC_FCMD_START_PROG	EFC_FCMD_EWP
	#define AT91C_MC_FCMD_ERASE_ALL		EFC_FCMD_EA
	#define AT91C_MC_FCMD_SET_GP_NVM	EFC_FCMD_SFB
	#define AT91C_MC_FCMD_CLR_GP_NVM	EFC_FCMD_CFB
	#define AT91C_MC_FCMD_GET_GP_NVM	EFC_FCMD_GFB

	#define AT91C_MC_FRDY				EEFC_FSR_FRDY
	#define AT91C_MC_KEY 				EEFC_FCR_FKEY(0x5Au)
	#define AT91C_MC_PROGE				EEFC_FSR_FCMDE
	#define AT91C_MC_LOCKE 				EEFC_FSR_FLOCKE
#elif (CPU_MODEL == CPU_MODEL__SAM4E16E)

/* EFC command */
#define EFC_FCMD_GETD    0x00
#define EFC_FCMD_WPL     0x02
#define EFC_FCMD_EWP     0x03
#define EFC_FCMD_EWPL    0x04
#define EFC_FCMD_EA      0x05
#define EFC_FCMD_SLB     0x08
#define EFC_FCMD_CLB     0x09
#define EFC_FCMD_GLB     0x0A
#define EFC_FCMD_SFB     0x0B
#define EFC_FCMD_CFB     0x0C
#define EFC_FCMD_GFB     0x0D
#define EFC_FCMD_STUI    0x0E
#define EFC_FCMD_SPUI    0x0F

#define MC0_FMR_ADDR				((volatile unsigned int *) &(EFC->EEFC_FMR))
#define MC0_FCR_ADDR				((volatile unsigned int *) &(EFC->EEFC_FCR))
#define MC0_FSR_ADDR				((volatile unsigned int *) &(EFC->EEFC_FSR))
#define MC0_FRR_ADDR				((volatile unsigned int *) &(EFC->EEFC_FRR))

#define AT91C_MC_FCMD_START_PROG	EFC_FCMD_EWP
#define AT91C_MC_FCMD_ERASE_ALL		EFC_FCMD_EA
#define AT91C_MC_FCMD_SET_GP_NVM	EFC_FCMD_SFB
#define AT91C_MC_FCMD_CLR_GP_NVM	EFC_FCMD_CFB
#define AT91C_MC_FCMD_GET_GP_NVM	EFC_FCMD_GFB

#define AT91C_MC_FRDY				EEFC_FSR_FRDY
//#define AT91C_MC_KEY 				EEFC_FCR_FKEY(0x5Au)
#define AT91C_MC_KEY 				EEFC_FCR_FKEY_Msk & EEFC_FCR_FKEY_PASSWD // EEFC_FCR_FKEY_Msk & EEFC_FCR_FKEY(0x5Au)
#define AT91C_MC_PROGE				EEFC_FSR_FCMDE
#define AT91C_MC_LOCKE 				EEFC_FSR_FLOCKE
#else
	#error "Unsupported CPU_MODEL"
#endif

int at91_getnvm(int cmd) {
	int r;
	int bitno;
	unsigned int reg;
	volatile unsigned int * fcr;
	volatile unsigned int * fsr;
	volatile unsigned int * frr;
	unsigned int i;

	if (0 > cmd || cmd > 3) {
		return -2;
	}

	fcr = MC0_FCR_ADDR;
	fsr = MC0_FSR_ADDR;
	frr = MC0_FRR_ADDR;

	r = -1;
	for (i=0; i<4; i++) {
		reg = *fsr;
		if (1 == (reg & AT91C_MC_FRDY)) {
			r = 0;
			break;
		}
		bsp_busywait(18 * bsp_getbusywaitcountperms());
	}
	if (0 != r) {
		return -1;
	}

	bitno = cmd;

	reg = 0;
	reg |= ((0x0000005A << 24) & AT91C_MC_KEY);
	reg |= (bitno << 8);
	reg |= (AT91C_MC_FCMD_GET_GP_NVM);
	*fcr = reg;

	r = -1;
	for (i=0; i<4; i++) {
		reg = *fsr;
		if (1 == (reg & AT91C_MC_FRDY)) {
			r = 0;
			break;
		}
		bsp_busywait(7 * bsp_getbusywaitcountperms());
	}
	if (0 != r) {
		return -1;
	}

	if (1 == (reg & AT91C_MC_LOCKE)) {
		return -1;
	}
	if (1 == (reg & AT91C_MC_PROGE)) {
		return -1;
	}

	reg = *frr;
	reg = (reg >> (bitno)) & 0x1;
	r = (int) reg;

	return r;
}

int at91_setnvm(int cmd, int value) {
	int r;
	int bitno;
	unsigned int reg;
	//volatile unsigned int * fmr;
	volatile unsigned int * fcr;
	volatile unsigned int * fsr;
	unsigned int i;

	if (0 > cmd || cmd > 3) {
		return -2;
	}

	if (0 != value && 1 != value) {
		return -3;
	}

	if (AT91_NVM_CMD__SECURITY == cmd && 0 == value) {
		return -3;
	}

	//fmr = MC0_FMR_ADDR;
	fcr = MC0_FCR_ADDR;
	fsr = MC0_FSR_ADDR;

	r = -1;
	for (i=0; i<4; i++) {
		reg = *fsr;
		if (1 == (reg & AT91C_MC_FRDY)) {
			r = 0;
			break;
		}
		bsp_busywait(18 * bsp_getbusywaitcountperms());
	}
	if (0 != r) {
		return -1;
	}

	bitno = cmd;

	reg = 0;
	reg |= ((0x0000005A << 24) & AT91C_MC_KEY);
	reg |= (bitno << 8);
	if (value == 1) {
		reg |= (AT91C_MC_FCMD_SET_GP_NVM);
	}
	else {
		reg |= (AT91C_MC_FCMD_CLR_GP_NVM);
	}
	*fcr = reg;

	r = -1;
	for (i=0; i<4; i++) {
		reg = *fsr;
		if (1 == (reg & AT91C_MC_FRDY)) {
			r = 0;
			break;
		}
		bsp_busywait(7 * bsp_getbusywaitcountperms());
	}
	if (0 != r) {
		return -1;
	}

	if (1 == (reg & AT91C_MC_LOCKE)) {
		return -1;
	}
	if (1 == (reg & AT91C_MC_PROGE)) {
		return -1;
	}

	return r;
}

void arm_set_pendsv(void) {
	SCB->ICSR = (1 << 28);
	ARM_DATASYNC();
}

unsigned int arm_get_pendsv(void) {
	register unsigned int value;
	value = SCB->ICSR;
	return (value >> 28);
}

void arm_set_svcpend(void) {
	SCB->SHCSR = (1 << 15);
	ARM_DATASYNC();
}

unsigned int arm_get_svcpend(void) {
	register unsigned int value;
	value = SCB->SHCSR;
	return (value >> 15);
}

