/**
 * @file tal_tx.h
 *
 * @brief File contains the TAL sub-state macros and functions that
 * perform frame transmission.
 *
 * $Id: tal_tx.h,v 1.2 2010-07-31 07:00:46 slb Exp $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef TAL_TX_H
#define TAL_TX_H

#include "../../../../ubiconfig.h"

/* === INCLUDES ============================================================ */

#include "../../Inc/tal.h"

/* === EXTERNALS =========================================================== */


/* === TYPES =============================================================== */


/* === MACROS ============================================================== */


/* === PROTOTYPES ========================================================== */

#ifdef __cplusplus
extern "C" {
#endif


void handle_tx_end_irq(bool underrun_occured);
void send_frame(uint8_t *frame_tx, csma_mode_t csma_mode, bool tx_retries);
void tx_done_handling(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TAL_TX_H */

/* EOF */

