/**
 * @file tal_slotted_csma.h
 *
 * @brief File provides CSMA-CA states.
 *
 * $Id: tal_slotted_csma.h,v 1.2 2010-07-31 07:04:15 slb Exp $
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
#ifndef TAL_SLOTTED_CSMA_H
#define TAL_SLOTTED_CSMA_H

#include "../../../../ubiconfig.h"

#ifdef BEACON_SUPPORT

/* === INCLUDES ============================================================ */

/* === EXTERNALS =========================================================== */

/* === TYPES =============================================================== */

/*
 * CSMA-CA states
 */
typedef enum csma_state_tag
{
    CSMA_IDLE = 0,
    BACKOFF_WAITING_FOR_CCA_TIMER,
    BACKOFF_WAITING_FOR_BEACON,
    CSMA_ACCESS_FAILURE,
    FRAME_SENDING,
    TX_DONE_SUCCESS,
    TX_DONE_FRAME_PENDING,
    TX_DONE_NO_ACK,
    NO_BEACON_TRACKING,
    CSMA_HANDLE_BEACON
} csma_state_t;


/* === MACROS ============================================================== */

/* === PROTOTYPES ========================================================== */

#ifdef __cplusplus
extern "C" {
#endif


void slotted_csma_start(bool perform_frame_retry);
void slotted_csma_state_handling(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BEACON_SUPPORT */

#endif /* TAL_SLOTTED_CSMA_H */

/* EOF */
