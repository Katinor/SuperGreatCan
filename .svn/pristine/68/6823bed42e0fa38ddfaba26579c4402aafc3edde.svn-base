/*
 * lib_sam4e16e_board.c
 *
 *  Created on: 2015. 4. 21.
 *      Author: geoyun
 */

#include "../itf/lib_sam4e16e_board.h"


#include <stdio.h>
#include <sam4e.h>

#include "ioport.h"
#include "pio.h"
#include "pio/pio.h"
#include "pio/pio_handler.h"
//#include "pio_handler.h"

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"
#include "sysclk.h"

int sam4e16e_board_init(void)
{
	/* Disable the watchdog */
	WDT->WDT_MR = WDT_MR_WDDIS;

	// all of pio init
	ioport_init();

}
