/**
 * @file tal_ed_end_cb.c
 *
 * @brief This file contains user call back function for
 * tal_ed_end_cb.
 *
 * $Id: tal_ed_end_cb.c,v 1.4 2010-08-05 05:41:48 slb Exp $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#include "../../../ubiconfig.h"

/* === Includes ============================================================= */

#include <stdint.h>
#include <stdbool.h>
#include "../Inc/tal.h"

/* === Macros ============================================================== */

/* === Globals ============================================================= */

/* === Prototypes ========================================================== */

/* === Implementation ====================================================== */

void tal_ed_end_cb(uint8_t energy_level)
{
    energy_level = energy_level;    /* Keep compiler happy. */
}

#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
tal_ed_end_cb_ft _g_tal_ed_end_cb_fp = tal_ed_end_cb;

int tal_ed_end_cb_set( tal_ed_end_cb_ft cb ) {
	_g_tal_ed_end_cb_fp = cb;
	return 0;
}
#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */

/* EOF */
