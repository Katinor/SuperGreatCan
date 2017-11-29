/**
 * @file PAL/MEGA_RF/ATMEGA128RFA1/Boards/RCB_6_3_PLAIN/pal_irq.c
 *
 * @brief IRQ specific functions for RCB_6_3_PLAIN with ATmega128RFA1
 *
 * This file contains functions to initialize, enable, disable and install
 * handler for the transceiver interrupts. It also contains functions to enable,
 * disable and get the status of global interrupt
 *
 * $Id: pal_irq.c,v 1.2 2010-08-01 09:11:50 slb Exp $
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

#if (MEGA_RF == PAL_GENERIC_TYPE) && (ATMEGA128RFA1 == PAL_TYPE) && (RCB_6_3_PLAIN == BOARD_TYPE)

/* === Includes ============================================================ */

#include <stdint.h>
#include "../../../../Inc/pal.h"
#include "../pal_boardtypes.h"

#if (BOARD_TYPE == RCB_6_3_PLAIN)

/**
 * \addtogroup grpPal_ATMEGA128RFA1_RCB_6_3_PLAIN
 * @{
 */

/* === Types =============================================================== */

/**
 * This is a typedef of the function which is called from the transceiver ISR
 */
typedef void (*irq_handler_t)(void);

/* === Globals ============================================================= */

/**
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
    if (trx_irq_num >= NO_OF_TRX_IRQS)
    {
        /* Illegal request. */
        return;
    }
    /*
     * Set the handler function.
     * The handler is set before enabling the interrupt to prepare for spurious
     * interrupts, that can pop up the moment they are enabled
     */
    irq_handler[trx_irq_num] = (irq_handler_t)trx_irq_cb;
    ENABLE_TRX_IRQ(trx_irq_num);
}



#if defined(DOXYGEN)
/**
 * @brief ISR for transceiver's PLL lock interrupt
 */
void TRX24_PLL_LOCK_vect(void);
#else  /* !DOXYGEN */
ISR(TRX24_PLL_LOCK_vect)
{
    irq_handler[RFA1_PLL_LOCK_IRQ_HDLR_IDX]();
}
#endif /* defined(DOXYGEN) */



#if defined(DOXYGEN)
/**
 * @brief ISR for transceiver's PLL unlock interrupt
 */
void TRX24_PLL_UNLOCK_vect(void);
#else  /* !DOXYGEN */
ISR(TRX24_PLL_UNLOCK_vect)
{
    irq_handler[RFA1_PLL_UNLOCK_IRQ_HDLR_IDX]();
}
#endif /* defined(DOXYGEN) */



#if defined(DOXYGEN)
/**
 * @brief ISR for transceiver's rx start interrupt
 *
 * By the time the SFD is detected, the hardware timestamps the
 * current frame in the SCTSR register.
 */
void TRX24_RX_START_vect(void);
#else  /* !DOXYGEN */
ISR(TRX24_RX_START_vect)
{
    irq_handler[RFA1_RX_START_IRQ_HDLR_IDX]();
}
#endif /* defined(DOXYGEN) */



#if defined(DOXYGEN)
/**
 * @brief ISR for transceiver's receive end interrupt
 */
void TRX24_RX_END_vect(void);
#else  /* !DOXYGEN */
ISR(TRX24_RX_END_vect)
{
    irq_handler[RFA1_RX_END_IRQ_HDLR_IDX]();
}
#endif /* defined(DOXYGEN) */



#if defined(DOXYGEN)
/**
 * @brief ISR for transceiver's CCA/ED measurement done interrupt
 */
void TRX24_CCA_ED_DONE_vect(void);
#else  /* !DOXYGEN */
ISR(TRX24_CCA_ED_DONE_vect)
{
    irq_handler[RFA1_CCA_ED_DONE_IRQ_HDLR_IDX]();
}
#endif /* defined(DOXYGEN) */



#if defined(DOXYGEN)
/**
 * @brief ISR for transceiver's transmit end interrupt
 */
void TRX24_TX_END_vect(void);
#else  /* !DOXYGEN */
ISR(TRX24_TX_END_vect)
{
    irq_handler[RFA1_TX_END_IRQ_HDLR_IDX]();
}
#endif /* defined(DOXYGEN) */



#if defined(DOXYGEN)
/**
 * @brief ISR for transceiver's address match interrupt
 */
void TRX24_XAH_AMI_vect(void);
#else  /* !DOXYGEN */
ISR(TRX24_XAH_AMI_vect)
{
    irq_handler[RFA1_AMI_IRQ_HDLR_IDX]();
}
#endif /* defined(DOXYGEN) */



#if defined(DOXYGEN)
/**
 * @brief ISR for transceiver's battery low interrupt
 */
void BAT_LOW_vect(void);
#else  /* !DOXYGEN */
ISR(BAT_LOW_vect)
{
    irq_handler[RFA1_BAT_LOW_IRQ_HDLR_IDX]();
}
#endif /* defined(DOXYGEN) */



#if defined(DOXYGEN)
/**
 * @brief ISR for transceiver's Awake interrupt
 */
void TRX24_AWAKE_vect(void);
#else  /* !DOXYGEN */
ISR(TRX24_AWAKE_vect)
{
    irq_handler[RFA1_AWAKE_IRQ_HDLR_IDX]();
}
#endif /* defined(DOXYGEN) */



#if defined(DOXYGEN)
/**
 * @brief ISR for transceiver's AES interrupt
 */
void AES_READY_vect(void);
#else  /* !DOXYGEN */
ISR(AES_READY_vect)
{
    irq_handler[RFA1_AES_READY_IRQ_HDLR_IDX]();
}
#endif /* defined(DOXYGEN) */

/** @} */

#endif /* RCB_6_3_PLAIN */

#endif /* (MEGA_RF == PAL_GENERIC_TYPE) && (ATMEGA128RFA1 == PAL_TYPE) && (RCB_6_3_PLAIN == BOARD_TYPE) */

/* EOF */
