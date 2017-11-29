/**
 * @file tal_tx_frame_done_cb.c
 *
 * @brief This file contains user call back function for
 * tal_tx_frame_done_cb.
 *
 * $Id: tal_tx_frame_done_cb.c,v 1.4 2010-08-05 05:41:48 slb Exp $
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

void tal_tx_frame_done_cb(retval_t status, frame_info_t *frame)
{
    /* Keep compiler happy. */
    status = status;
    frame = frame;
}

#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
tal_tx_frame_done_cb_ft _g_tal_tx_frame_done_cb_fp = tal_tx_frame_done_cb;

int tal_tx_frame_done_cb_set( tal_tx_frame_done_cb_ft cb ) {
	_g_tal_tx_frame_done_cb_fp = cb;
	return 0;
}
#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */

/* EOF */
