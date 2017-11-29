/**
 * @file pal_irq.c
 *
 * @brief PAL IRQ functionality
 *
 * This file contains functions to initialize, enable, disable and install
 * handler for the transceiver interrupts.
 *
 * $Id: pal_irq.c,v 1.2 2010-08-01 11:57:26 slb Exp $
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

#if (AVR == PAL_GENERIC_TYPE) && (ATMEGA1281 == PAL_TYPE) && (RCB_4_0_SENS_TERM_BOARD == BOARD_TYPE)

/* === Includes ============================================================ */

#include <stdint.h>
#include "../../../../Inc/pal.h"
#include "../pal_boardtypes.h"

#if (BOARD_TYPE == RCB_4_0_SENS_TERM_BOARD)

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
        /* Init Main TRX interrupt */
        /* Select rising edge */
        EICRA |= _BV(ISC00) | _BV(ISC01);
        /* clear pending interrupt */
        EIFR = _BV(INTF0);
    }
    else if (trx_irq_num == TRX_TSTAMP_IRQ_HDLR_IDX)
    {
        /* Init RX TIME STAMP interrupt */
        /* The input capture interrupt of timer is disabled */
        TIMSK1 &= ~(_BV(ICIE1));
        /* Rising edge on input capture pin used to trigger IRQ */
        TCCR1B |= (_BV(ICES1));
        /* Input capture flag is cleared */
        TIFR1 |= (_BV(ICF1));
    }
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
ISR(TRX_TSTAMP_ISR_VECTOR)
{
    irq_handler[TRX_TSTAMP_IRQ_HDLR_IDX]();
}


#endif /* RCB_4_0_SENS_TERM_BOARD */

#endif /* (AVR == PAL_GENERIC_TYPE) && (ATMEGA1281 == PAL_TYPE) && (RCB_4_0_SENS_TERM_BOARD == BOARD_TYPE) */

/* EOF */
