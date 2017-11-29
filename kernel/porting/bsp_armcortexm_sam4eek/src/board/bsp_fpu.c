/*
 * bsp_fpu.c
 *
 *  Created on: 2015. 7. 21.
 *      Author: geoyun
 */

#include "../../ubiconfig.h"

#include <stdio.h>
#include <sam4e.h>
#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"

void ubik_fpu_enable(void) {
	SCB->CPACR = (SCB->CPACR & ~(0xF << 20)) | (0xF << 20);
}


void ubik_fpu_disable(void) {
	SCB->CPACR = (SCB->CPACR & ~(0xF << 20)) | (0x0 << 20);
}



