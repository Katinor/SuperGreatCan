/**
 * @file tal_pwr_mgmt.c
 *
 * @brief This file implements TAL power management functionality
 *        of the transceiver.
 *
 * $Id: tal_pwr_mgmt.c,v 1.2 2010-07-31 07:00:46 slb Exp $
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

#if (AT86RF231 == TAL_TYPE)

/* === INCLUDES ============================================================ */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../../../PAL/Inc/pal.h"
#include "../../../Include/return_val.h"
#include "../../Inc/tal.h"
#include "../../../Include/_tal_constants.h"
#include "../Inc/at86rf231.h"
#include "../../../Include/_tal_internal.h"
#ifdef ENABLE_STB
#include "../../../SAL/Inc/sal_types.h"
#if (SAL_TYPE == AT86RF2xx)
#include "../../../STB/Inc/stb.h"
#endif
#endif

/* === TYPES =============================================================== */


/* === MACROS ============================================================== */


/* === GLOBALS ============================================================= */


/* === PROTOTYPES ========================================================== */


/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Sets the transceiver to sleep
 *
 * This function sets the transceiver to sleep state.
 *
 * @param mode Defines sleep mode of transceiver SLEEP or PHY_TRX_OFF)
 *
 * @return   TAL_BUSY - The transceiver is busy in TX or RX
 *           MAC_SUCCESS - The transceiver is put to sleep
 *           TAL_TRX_ASLEEP - The transceiver is already asleep
 *           MAC_INVALID_PARAMETER - The specified sleep mode is not supported
 */
retval_t tal_trx_sleep(sleep_mode_t mode)
{
    tal_trx_status_t trx_status;

    /* Current transceiver only supports SLEEP_MODE_1 mode. */
    if (SLEEP_MODE_1 != mode)
    {
        return MAC_INVALID_PARAMETER;
    }

    if (tal_trx_status == TRX_SLEEP)
    {
        return TAL_TRX_ASLEEP;
    }

    /* Device can be put to sleep only when the TAL is in IDLE state. */
    if (TAL_IDLE != tal_state)
    {
        return TAL_BUSY;
    }

    tal_rx_on_required = false;

    /*
     * First set trx to TRX_OFF.
     * If trx is busy, like ACK transmission, do not interrupt it.
     */
    do
    {
        trx_status = set_trx_state(CMD_TRX_OFF);
    } while (trx_status != TRX_OFF);

    pal_timer_source_select(TMR_CLK_SRC_DURING_TRX_SLEEP);

    trx_status = set_trx_state(CMD_SLEEP);

#ifdef ENABLE_FTN_PLL_CALIBRATION
    /*
     * Stop the calibration timer now.
     * The timer will be restarted during wake-up.
     */
    pal_timer_stop(TAL_CALIBRATION);
#endif  /* ENABLE_FTN_PLL_CALIBRATION */

    if (trx_status == TRX_SLEEP)
    {
#ifdef ENABLE_STB
#if (SAL_TYPE == AT86RF2xx)
        stb_restart();
#endif
#endif
        return MAC_SUCCESS;
    }
    else
    {
        /* State could not be set due to TAL_BUSY state. */
        return TAL_BUSY;
    }
}


/**
 * @brief Wakes up the transceiver from sleep
 *
 * This function awakes the transceiver from sleep state.
 *
 * @return   TAL_TRX_AWAKE - The transceiver is already awake
 *           MAC_SUCCESS - The transceiver is woken up from sleep
 *           FAILURE - The transceiver did not wake-up from sleep
 */
retval_t tal_trx_wakeup(void)
{
    tal_trx_status_t trx_status;

    if (tal_trx_status != TRX_SLEEP)
    {
        return TAL_TRX_AWAKE;
    }

#ifdef ENABLE_FTN_PLL_CALIBRATION
    {
        retval_t timer_status;

        /*
         * Calibration timer has been stopped when going to sleep,
         * so it needs to be restarted.
         * All other state changes except via sleep that are ensuring
         * implicit filter tuning and pll calibration are ignored.
         * Therefore the calibration timer needs to be restarted for
         * to those cases.
         * This is handled in file tal.c.
         */

        /* Start periodic calibration timer.*/
        timer_status = pal_timer_start(TAL_CALIBRATION,
                                       TAL_CALIBRATION_TIMEOUT_US,
                                       TIMEOUT_RELATIVE,
                                       (FUNC_PTR)calibration_timer_handler_cb,
                                       NULL);

        if (timer_status != MAC_SUCCESS)
        {
            ASSERT("PLL calibration timer start problem" == 0);
        }
    }
#endif  /* ENABLE_FTN_PLL_CALIBRATION */

    trx_status = set_trx_state(CMD_TRX_OFF);

    if (trx_status == TRX_OFF)
    {
        pal_timer_source_select(TMR_CLK_SRC_DURING_TRX_AWAKE);
        return MAC_SUCCESS;
    }
    else
    {
        return FAILURE;
    }
}

#endif /* #if (AT86RF231 == TAL_TYPE) */

/* EOF */
