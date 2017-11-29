/**
 * @file pal_irq.c
 *
 * @brief PAL IRQ functionality
 *
 * This file contains functions to initialize, enable, disable and install
 * handler for the transceiver interrupts.
 *
 * $Id: pal_irq.c,v 1.2 2012-02-14 05:21:09 essusige Exp $
 *
 */
/**
 * @author
 * Atmel Corporation: http://www.atmel.com
 * Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#include "../../../../../../ubiconfig.h"

#if (SAM3S == PAL_GENERIC_TYPE) && (AT91SAM3S == PAL_TYPE) && (REB_4_0_2_REX_ARM_SAM3S_REV_1 == BOARD_TYPE)

/* === Includes ============================================================= */

#include <stdint.h>
//#include "../../../../../../../itf_ubinos/hdr/bsp_intr.h"
#include "../../../../Inc/pal.h"
#include "../pal_boardtypes.h"

/*
 * 15.7.24 기존 경로 "../../../../../../../lib_sam3slib/src/drivers/pio/pio.h"를
 * ../../../../../../../../../kernel/porting/lib_sam4e/sam/drivers/pio/pio.h로 변경
 * Pin 제거 및 신규 코드로 변환
 */
#include "../../../../../../../../../kernel/porting/lib_sam4e/sam/drivers/pio/pio.h"
#include "../../../../../../../../../kernel/porting/lib_sam4e/sam/drivers/pio/pio_handler.h"
#include "../../../../../../../../../kernel/driver/lib_ubi_pio/itf/lib_ubi_pio.h"

const Pin TRX_INTERRUPT_PIN_STRUTURE = {TRX_INTERRUPT_PIN, PIOA, ID_PIOA, PIO_INPUT, PIO_IT_EDGE | PIO_IT_RE_OR_HL};

#if (BOARD_TYPE == REB_4_0_2_REX_ARM_SAM3S_REV_1)

/* === Types ============================================================== */

/**
 * This is a typedef of the function which is called from the transceiver ISR
 */
typedef void (*irq_handler_t)(void);

/* === Globals ============================================================== */

/*
 * Function pointers to store the callback function of
 * the transceiver interrupt
 */
static irq_handler_t irq_handler[NO_OF_TRX_IRQS];

/* === Prototypes =========================================================== */

static void trx_isr(void);

/* === Implementation ======================================================= */

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
    if (TRX_MAIN_IRQ_HDLR_IDX == trx_irq_num)
    {
        /*
         * The AIC is configured and updated with the mode for generating
         * interrupt for IRQ0, which is connected to the transceiver interrupt
         * line. Also the interrupt handler is installed for the same.
         */
    	//NVIC_CONFIGURE(ID_PIOA, 0, trx_isr);

		/* The interrupts for specified peripheral are first disabled. */
		intr_disable(ID_PIOA);
		/*
		 * The edge at which the interrupt is to be triggered and handler for
		 * the same is configured.
		 */
		ubi_pio_interrupt_config(PORTA, TRX_INTERRUPT_PIN_IDX, TRX_INTERRUPT_PIN_STRUTURE.attribute, trx_isr);

		pio_enable_interrupt( PIOA, TRX_INTERRUPT_PIN_IDX );

       	intr_enable(ID_PIOA);

        /*  the mode in function of NVIC_CONFIGURE is AT91C_AIC_SRCTYPE_POSITIVE_EDGE
         * Set the handler function.
         * The handler is set before enabling the interrupt to prepare for
         * spurious interrupts, that can pop up the moment they are enabled.
         */

        irq_handler[trx_irq_num] = (irq_handler_t)trx_irq_cb;

        /* The input capture interrupt of timer is disabled. */
        DISABLE_TRX_IRQ(TRX_MAIN_IRQ_HDLR_IDX);

        /*
         * The interrupt bit corresponding to the trx interrupt is cleared.
         */
        CLEAR_TRX_IRQ(TRX_MAIN_IRQ_HDLR_IDX);
    }
}



/**
 * @brief ISR for IRQ0
 *
 * This function is an ISR for IRQ0 interrupt (transceiver interrupt) of the
 * microcontroller. It calls the user installed handler for transceiver
 * interrupt.
 */
extern sem_pt rf_sem_guard;
void trx_isr(void)
{
	unsigned int reg, status;

    reg = PIOA->PIO_PDSR;
    status = PIOA->PIO_ISR;

//    status = PIOA->PIO_ISR;
//    status &= PIOA->PIO_IMR;
	sem_give(rf_sem_guard);

	if((reg & TRX_INTERRUPT_PIN))
    {
		irq_handler[TRX_MAIN_IRQ_HDLR_IDX]();
    }
}

#endif /* REB_4_0_2_REX_ARM_ESPS_REV_1 */

#endif /* (ARM7 == PAL_GENERIC_TYPE) && (AT91SAM7X256 == PAL_TYPE) && (REB_4_0_2_REX_ARM_ESPS_REV_1 == BOARD_TYPE) */

/* EOF */
