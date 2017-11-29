/**
 * @file pal_irq.c
 *
 * @brief PAL IRQ functionality
 *
 * This file contains functions to initialize, enable, disable and install
 * handler for the transceiver interrupts.
 *
 * $Id: pal_irq.c,v 1.2 2010-08-01 10:00:27 slb Exp $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#include "../../../../../../ubiconfig.h"

#if (XMEGA == PAL_GENERIC_TYPE) && (ATXMEGA256A3 == PAL_TYPE) && (REB_4_1_STK600 == BOARD_TYPE)

/* === Includes ============================================================ */

#include <stdint.h>
#include "../../../../Inc/pal.h"
#include "../pal_boardtypes.h"

#if (BOARD_TYPE == REB_4_1_STK600)

/* === Types ============================================================== */

/**
 * This is a typedef of the function which is called from the transceiver ISR
 */
typedef void (*irq_handler_t)(void);

/* === Globals ============================================================= */

/*
 * Function pointers to store the callback function of
 * the transceiver interrupt
 */
static irq_handler_t irq_handler[NO_OF_TRX_IRQS];

/* === Prototypes ========================================================== */

/* === Implementation ====================================================== */

/**
 * @brief Initializes the transceiver interrupts
 *
 * This function sets the microcontroller specific registers
 * responsible for handling the transceiver interrupts
 *
 * @param trx_irq_num Transceiver interrupt line to be initialized
 * @param trx_irq_cb Callback function for the given transceiver
 * interrupt
 */
void pal_trx_irq_init(trx_irq_hdlr_idx_t trx_irq_num, FUNC_PTR trx_irq_cb)
{
    /*
     * Set the handler function.
     * The handler is set before enabling the interrupt to prepare for spurious
     * interrupts, that can pop up the moment they are enabled
     */
    irq_handler[trx_irq_num] = (irq_handler_t)trx_irq_cb;

    if (trx_irq_num == TRX_MAIN_IRQ_HDLR_IDX)
    {
        /* Rising edge on IRQ pin used to trigger IRQ */
        PORTC.PIN2CTRL = PORT_ISC0_bm;

        /* Set pin 2 as source for port interrupt 0 */
        PORTC.INT0MASK = PIN2_bm;

        /* Clear pending interrupts */
        PORTC.INTFLAGS = PORT_INT0IF_bm;
    }
#ifndef ANTENNA_DIVERSITY
    else if (trx_irq_num == TRX_TSTAMP_IRQ_HDLR_IDX)
    {
        /* Rising edge on DIG2 pin used to trigger IRQ */
        PORTC.PIN1CTRL = PORT_ISC0_bm;

        /* Set pin 1 as source for port interrupt 1 */
        PORTC.INT1MASK = PIN1_bm;

        /* Clear pending interrupts */
        PORTC.INTFLAGS = PORT_INT1IF_bm;
    }
#endif
}


/**
 * @brief ISR for transceiver's main interrupt
 */
ISR(TRX_MAIN_ISR_VECTOR)
{
    irq_handler[TRX_MAIN_IRQ_HDLR_IDX]();
}


/**
 * @brief ISR for transceiver's RX TIME STAMP interrupt
 */
#ifndef ANTENNA_DIVERSITY
ISR(TRX_TSTAMP_ISR_VECTOR)
{
    /* Clear capture interrupt. */
    TCC1_INTFLAGS |= TC1_CCAIF_bm;

    irq_handler[TRX_TSTAMP_IRQ_HDLR_IDX]();
}
#endif

#endif /* REB_4_1_STK600 */

#endif /* (XMEGA == PAL_GENERIC_TYPE) && (ATXMEGA256A3 == PAL_TYPE) && (REB_4_1_STK600 == BOARD_TYPE) */

/* EOF */
