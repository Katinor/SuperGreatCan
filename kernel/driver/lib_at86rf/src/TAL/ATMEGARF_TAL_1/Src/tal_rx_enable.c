/**
 * @file tal_rx_enable.c
 *
 * @brief File provides functionality supporting RX-Enable feature.
 *
 * $Id: tal_rx_enable.c,v 1.2 2010-07-31 07:04:16 slb Exp $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#include "../../../../ubiconfig.h"

#if (ATMEGARF_TAL_1 == TAL_TYPE)

/* === INCLUDES ============================================================ */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../../../PAL/Inc/pal.h"
#include "../../../Include/return_val.h"
#include "../Inc/tal_tx.h"
#include "../../Inc/tal.h"
#include "../../../Include/ieee_const.h"
#include "../../../Include/_tal_constants.h"
#include "../Inc/atmega128rfa1.h"
#include "../../../Resources/Buffer_Management/Inc/bmm.h"
#include "../../../Resources/Queue_Management/Inc/qmm.h"
#include "../Inc/tal_rx.h"
#include "../../../Include/_tal_internal.h"

/* === TYPES =============================================================== */


/* === MACROS ============================================================== */


/* === GLOBALS ============================================================= */


/* === PROTOTYPES ========================================================== */


/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Switches receiver on or off
 *
 * This function switches the receiver on (PHY_RX_ON) or off (PHY_TRX_OFF).
 *
 * @param state New state of receiver
 *
 * @return TAL_BUSY if the TAL state machine cannot switch receiver on or off,
 *         TRX_OFF if receiver has been switched off, or
 *         RX_ON otherwise.
 *
 * @ingroup apiTalApi
 */
uint8_t tal_rx_enable(uint8_t state)
{
    /*
     * Trx can only be enabled if TAL is not busy;
     * i.e. if TAL is IDLE.
     */
    if (TAL_IDLE != tal_state)
    {
        return TAL_BUSY;
    }

    if (state == PHY_TRX_OFF)
    {
        /*
         * If the rx needs to be switched off, we are not interested in a frame
         * that is currently being received.
         * This must not be a Forced TRX_OFF (CMD_FORCED_TRX_OFF) since this could
         * corrupt an already outoing ACK frame.
         */
         set_trx_state(CMD_TRX_OFF);
         tal_rx_on_required = false;
         return TRX_OFF;
    }
    else
    {
#ifdef SNIFFER
        set_trx_state(CMD_RX_ON);
#else
        if (tal_rx_buffer != NULL)
        {
#ifdef PROMISCUOUS_MODE
            if (tal_pib_PromiscuousMode)
            {
                set_trx_state(CMD_RX_ON);
            }
            else
            {
                set_trx_state(CMD_RX_AACK_ON);
            }
#else   /* Normal operation */
            set_trx_state(CMD_RX_AACK_ON);
#endif
        }
        else
        {
            /*
             * If no rx buffer is available, the corresponding
             * information is stored and will be used by tal_task() to
             * switch on the receiver later.
             *
             * Even if a receive buffer is not available,
             * the TAL returns MAC_SUCCESS. The TAL will try to allocate a receive
             * buffer as soon as possible and will switch on the receiver.
             */
            tal_rx_on_required = true;
        }
#endif
        return RX_ON;   // MAC layer assumes RX_ON as return value
    }
}

#endif /* (ATMEGARF_TAL_1 == TAL_TYPE) */

/* EOF */
