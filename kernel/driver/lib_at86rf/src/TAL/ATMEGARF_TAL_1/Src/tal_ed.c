/**
 * @file tal_ed.c
 *
 * @brief This file implements ED Scan
 *
 * $Id: tal_ed.c,v 1.4 2010-08-05 05:41:47 slb Exp $
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

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../../PAL/Inc/pal.h"
#include "../../../Include/return_val.h"
#include "../../Inc/tal.h"
#include "../../../Include/ieee_const.h"
#include "../../../Include/_tal_constants.h"
#include "../Inc/atmega128rfa1.h"
#include "../../../Include/_tal_internal.h"
#include "../Inc/tal_irq_handler.h"
#include "../../../Include/mac_build_config.h"

#if (MAC_SCAN_ED_REQUEST_CONFIRM == 1)

/* === TYPES =============================================================== */


/* === MACROS ============================================================== */

/* Constant define for the ED scaling: register value at -35dBm */
#define CLIP_VALUE_REG                  (55)

/*
 * Scan duration formula: \f$aBaseSuperframeDuration (2^SD + 1)\f$
 * where \f$0 <= SD <= 14\f$
 */
#define CALCULATE_SYMBOL_TIME_SCAN_DURATION(SD) \
                        (aBaseSuperframeDuration * ((1UL<<(SD))+1))

/* === GLOBALS ============================================================= */

/**
 * The peak_ed_level is the maximum ED value received from the transceiver for
 * the specified Scan Duration.
 */
static uint8_t max_ed_level;
static uint32_t sampler_counter;

/* === PROTOTYPES ========================================================== */

static void trx_ed_irq_handler_cb(void);

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Starts ED Scan
 *
 * This function starts an ED Scan for the scan duration specified by the
 * MAC layer.
 *
 * @param scan_duration Specifies the ED scan duration in symbols
 *
 * @return MAC_SUCCESS - ED scan duration timer started successfully
 *         TAL_BUSY - TAL is busy servicing the previous request from MAC
 *         TAL_TRX_ASLEEP - Transceiver is currently sleeping
 *         FAILURE otherwise
 */
retval_t tal_ed_start(uint8_t scan_duration)
{
    /*
     * Check if the TAL is in idle state. Only in idle state it can
     * accept and ED request from the MAC.
     */
    if (TAL_IDLE != tal_state)
    {
        if (tal_trx_status == TRX_SLEEP)
        {
            return TAL_TRX_ASLEEP;
        }
        else
        {
            ASSERT("TAL is TAL_BUSY" == 0);
            return TAL_BUSY;
        }
    }

    pal_trx_bit_write(SR_RX_PDT_DIS, RX_DISABLE);
    pal_trx_irq_flag_clr(RFA1_CCA_ED_DONE_IRQ_HDLR_IDX);
    pal_trx_irq_init(RFA1_CCA_ED_DONE_IRQ_HDLR_IDX, (FUNC_PTR)trx_ed_irq_handler_cb);
    pal_trx_irq_enable(RFA1_CCA_ED_DONE_IRQ_HDLR_IDX);

    /* Make sure that receiver is switched on. */
    if (set_trx_state(CMD_RX_ON) != RX_ON)
    {
        /* Restore previous configuration */
        pal_trx_bit_write(SR_RX_PDT_DIS, RX_ENABLE);
        pal_trx_reg_write(RG_IRQ_MASK, TRX_IRQ_DEFAULT);
        pal_trx_irq_disable(RFA1_CCA_ED_DONE_IRQ_HDLR_IDX);

        return FAILURE;
    }

    /* Perform ED in TAL_ED_RUNNING state. */
    tal_state = TAL_ED_RUNNING;

    max_ed_level = 0;   // reset max value

    sampler_counter = CALCULATE_SYMBOL_TIME_SCAN_DURATION(scan_duration) / ED_SAMPLE_DURATION_SYM;

    // write dummy value to start measurement
    pal_trx_reg_write(RG_PHY_ED_LEVEL, 0xFF);

    return MAC_SUCCESS;
}


/**
 * @brief ED Scan Interrupt
 *
 * This function handles an ED done interrupt from the transceiver.
 */
static void trx_ed_irq_handler_cb()
{
    uint8_t ed_value;

    /* Read the ED Value. */
    ed_value = pal_trx_reg_read(RG_PHY_ED_LEVEL);

    /*
     * Update the peak ED value received, if greater than the previously
     * read ED value.
     */
    if (ed_value > max_ed_level)
    {
        max_ed_level = ed_value;
    }

    /* Start next ED sampling */
    sampler_counter--;
    if (sampler_counter > 0)
    {
        // write dummy value to start measurement
        pal_trx_reg_write(RG_PHY_ED_LEVEL, 0xFF);
    }
    else
    {
        tal_state = TAL_ED_DONE;
    }
}


/**
 * @brief Scan done
 *
 * This function updates the max_ed_level and invokes the callback function
 * tal_ed_end_cb().
 *
 * @param parameter unused callback parameter
 */
void ed_scan_done(void)
{
    pal_trx_bit_write(SR_RX_PDT_DIS, RX_ENABLE);
    pal_trx_irq_disable(RFA1_CCA_ED_DONE_IRQ_HDLR_IDX);

    tal_state = TAL_IDLE;   // ed scan is done
    set_trx_state(CMD_RX_AACK_ON);

    /*
     * Scale ED result.
     * Clip values to 0xFF if > -35dBm
     */
    if (max_ed_level > CLIP_VALUE_REG)
    {
        max_ed_level = 0xFF;
    }
    else
    {
        max_ed_level = (uint8_t)(((uint16_t)max_ed_level * 0xFF) / CLIP_VALUE_REG);
    }

#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
    tal_ed_end_cb(max_ed_level);
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
    _g_tal_ed_end_cb_fp(max_ed_level);
#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
}

#endif /* (MAC_SCAN_ED_REQUEST_CONFIRM == 1) */

#endif /* (ATMEGARF_TAL_1 == TAL_TYPE) */

/* EOF */
