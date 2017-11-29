/**
 * @file tal_init.c
 *
 * @brief This file implements functions for initializing TAL.
 *
 * $Id: tal_init.c,v 1.3 2010-07-31 04:46:40 slb Exp $
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

#if (AT86RF230B == TAL_TYPE)

/* === INCLUDES ============================================================ */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../../../PAL/Inc/pal.h"
#include "../../../Include/return_val.h"
#include "../../Inc/tal.h"
#include "../../../Include/ieee_const.h"
#include "../Inc/tal_pib.h"
#include "../../../PAL/Inc/pal.h"
#include "../Inc/tal_irq_handler.h"
#include "../../../Include/stack_config.h"
#include "../../../Resources/Buffer_Management/Inc/bmm.h"
#include "../../../Resources/Queue_Management/Inc/qmm.h"
#include "../../Inc/tal.h"
#include "../../../Include/_tal_internal.h"
#include "../../../Include/_tal_constants.h"
#include "../Inc/at86rf230b.h"
#include "../Inc/tal_config.h"
#if (MAC_INDIRECT_DATA_FFD == 1)
#include "../../../Include/indirect_data_structures.h"
#endif  /* (MAC_INDIRECT_DATA_FFD == 1) */
#ifdef BEACON_SUPPORT
#include "../Inc/tal_slotted_csma.h"
#endif  /* BEACON_SUPPORT */
#ifdef NON_BLOCKING_SPI
#include "../Inc/tal_rx.h"
#endif

/* === GLOBALS ============================================================= */


/* === PROTOTYPES ========================================================== */

static void generate_rand_seed(void);
static retval_t trx_init(void);
static void trx_config(void);
static void trx_config_csma(void);
static retval_t trx_reset(void);

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Initializes the TAL
 *
 * This function is called to initialize the TAL. The transceiver is
 * initialized, the TAL PIBs are set to their default values, and the TAL state
 * machine is set to TAL_IDLE state.
 *
 * @return MAC_SUCCESS  if the transceiver state is changed to TRX_OFF and the
 *                 current device part number and version number are correct;
 *         FAILURE otherwise
 */
retval_t tal_init(void)
{
    /* Init the PAL and by this means also the transceiver interface */
    if (pal_init() != MAC_SUCCESS)
    {
        return FAILURE;
    }

    if (trx_init() != MAC_SUCCESS)
    {
        return FAILURE;
    }

#if (EXTERN_EEPROM_AVAILABLE == 1)
    pal_ps_get(EXTERN_EEPROM, EE_IEEE_ADDR, 8, &tal_pib_IeeeAddress);
#else
    pal_ps_get(INTERN_EEPROM, EE_IEEE_ADDR, 8, &tal_pib_IeeeAddress);
#endif

    if (trx_reset() != MAC_SUCCESS)
    {
        return FAILURE;
    }

    /* Write the transceiver values except of the CSMA seed. */
    trx_config();

    /*
     * For systems including the AT86RF230B the random seed generation
     * cannot be done using a dedicated hardware feature.
     * Therefore all random seed generation needs to be done by special
     * means (e.g. utilization of ADC) that generate a random value only
     * within a certain range.
     *
     * In case the node already has a valid IEEE address (i.e. an IEEE
     * address which is different from 0x0000000000000000 or
     * 0xFFFFFFFFFFFFFFFF), this IEEE address (the lower 16 bit)
     * shall be used as seed for srand(), since each node should have a unique
     * IEEE address.
     * In this case srand() is called directly and function generate_rand_seed()
     * is not called.
     *
     * Note: This behaviour is different in all other TALs, since in these
     * cases the seed for srand() is always generated based on transceiver
     * hardware support.
     */

#ifndef DISABLE_IEEE_ADDR_CHECK
    if ((tal_pib_IeeeAddress == 0x0000000000000000) ||
        (tal_pib_IeeeAddress == 0xFFFFFFFFFFFFFFFF)
       )
    {
        /*
         * Generate a seed for the random number generator in function rand().
         * This is required (for example) as seed for the CSMA-CA algorithm.
         */
        generate_rand_seed();

        /*
         * Now that we have generated a random seed, we can generate a random
         * IEEE address for this node.
         */
        do
        {
            /*
             * In case no valid IEEE address is available, a random
             * IEEE address will be generated to be able to run the
             * applications for demonstration purposes.
             * In production code this can be omitted.
             */
            /*
             * The proper seed for function rand() has already been generated
             * in function generate_rand_seed().
             */
            uint8_t *ptr_pib = (uint8_t *)&tal_pib_IeeeAddress;

            for (uint8_t i = 0; i < 8; i++)
            {
                *ptr_pib++ = (uint8_t)rand();
                /*
                 * Note:
                 * Even if casting the 16 bit rand value back to 8 bit,
                 * and running the loop 8 timers (instead of only 4 times)
                 * may look cumbersome, it turns out that the code gets
                 * smaller using 8-bit here.
                 * And timing is not an issue at this place...
                 */
            }
        }
        /* Check if a valid IEEE address is available. */
        while ((tal_pib_IeeeAddress == 0x0000000000000000) ||
               (tal_pib_IeeeAddress == 0xFFFFFFFFFFFFFFFF)
              );
    }
    else
    {
        /* Valid IEEE address, so no need to generate a new random seed. */
        uint16_t cur_rand_seed = (uint16_t)tal_pib_IeeeAddress;
        srand(cur_rand_seed);
    }
#else
    /*
     * Now check for a valid IEEE address done, so directly create a seed
     * for srand().
     */
        generate_rand_seed();
#endif

    /* Once we have initialized a proper seed for rand(), we
     * can now initialize the transceiver's CSMA seed. */
    trx_config_csma();

    pal_trx_reg_read(RG_IRQ_STATUS);    /* clear pending irqs, dummy read */

    /*
     * Configure interrupt handling.
     * Install a handler for the transceiver interrupt.
     */
    pal_trx_irq_init(TRX_MAIN_IRQ_HDLR_IDX, (FUNC_PTR)trx_irq_handler_cb);
    pal_trx_irq_enable(TRX_MAIN_IRQ_HDLR_IDX);     /* Enable transceiver interrupts. */

    /* Initialize the buffer management module and get a buffer to store reveived frames. */
    bmm_buffer_init();
    tal_rx_buffer = bmm_buffer_alloc(LARGE_BUFFER_SIZE);

    /* Init incoming frame queue */
    qmm_queue_init(&tal_incoming_frame_queue, TAL_INCOMING_FRAME_QUEUE_CAPACITY);

    /* Handle TAL's PIB values */
    init_tal_pib(); /* implementation can be found in 'tal_pib.c' */
    write_all_tal_pib_to_trx();  /* implementation can be found in 'tal_pib.c' */

    tal_state = TAL_IDLE;   /* reset TAL state machine */
#ifdef BEACON_SUPPORT
    tal_csma_state = CSMA_IDLE;
#endif  /* BEACON_SUPPORT */
    /* The receiver is not requested to be switched on after tal_init. */
    tal_rx_on_required = false;

    return MAC_SUCCESS;
} /* tal_init() */


/**
 * @brief Initializes the transceiver
 *
 * This function is called to initialize the transceiver.
 *
 * @return MAC_SUCCESS  if the transceiver state is changed to TRX_OFF and the
 *                 current device part number and version number are correct;
 *         FAILURE otherwise
 */
static retval_t trx_init(void)
{
    tal_trx_status_t trx_status;
    uint8_t poll_counter = 0;

    PAL_RST_HIGH();
    PAL_SLP_TR_LOW();

    pal_timer_delay(P_ON_TO_CLKM_AVAILABLE);

    // apply reset pulse
    PAL_RST_LOW();
    pal_timer_delay(RST_PULSE_WIDTH_US);
    PAL_RST_HIGH();

    /* Verify that TRX_OFF can be written */
    do
    {
        if (poll_counter == 0xFF)
        {
            return FAILURE;
        }
        poll_counter++;
        /* Check if AT86RF230 is connected; omit manufacturer id check */
    } while ((pal_trx_reg_read(RG_VERSION_NUM) != AT86RF230_REV_B) ||
             (pal_trx_reg_read(RG_PART_NUM) != AT86RF230));

    pal_trx_reg_write(RG_TRX_STATE, CMD_TRX_OFF);

    /* verify that trx has reached TRX_OFF */
    poll_counter = 0;
    do
    {
        trx_status = (tal_trx_status_t)pal_trx_bit_read(SR_TRX_STATUS);
        if (poll_counter == 0xFF)
        {
#if (DEBUG > 0)
            pal_alert();
#endif
            return FAILURE;
        }
        poll_counter++;
    } while (trx_status != TRX_OFF);

    tal_trx_status = TRX_OFF;

    return MAC_SUCCESS;
}


/**
 * @brief Configures the transceiver
 *
 * This function is called to configure the transceiver after reset.
 */
static void trx_config(void)
{
    /* Set pin driver strength */
    pal_trx_reg_write(RG_TRX_CTRL_0, ((CLKM_2mA << 6) |
                      (CLKM_2mA << 4) | CLKM_1MHz)); // fast change

    pal_trx_bit_write(SR_AACK_SET_PD, PD_ACK_BIT_SET_ENABLE); /* 1 == frame pending bit is always set to 1 */
    pal_trx_bit_write(SR_TX_AUTO_CRC_ON, TX_AUTO_CRC_ENABLE);    /* enable auto crc */
    pal_trx_reg_write(RG_IRQ_MASK, TRX_IRQ_TRX_END); /* enable TRX_END interrupt */
}



/**
 * @brief Configures the transceiver's CSMA seed
 *
 * This function is called to configure the transceiver's CSMA seed after reset.
 * it needs to be called in conjunction with funciton trx_config(), but
 * it needs be assured that a seed for function rand() had been generated before.
 */
static void trx_config_csma(void)
{
    uint16_t rand_value;

    /*
     * Init the SEED value of the CSMA backoff algorithm.
     */
    rand_value = (uint16_t)rand();
    pal_trx_reg_write(RG_CSMA_SEED_0, (uint8_t)rand_value);
    pal_trx_bit_write(SR_CSMA_SEED_1, (uint8_t)(rand_value >> 8));

    /*
     * To make sure that the CSMA seed is properly set within the transceiver,
     * put the trx to sleep briefly and wake it up again.
     */
    tal_trx_sleep(SLEEP_MODE_1);

    tal_trx_wakeup();
}



/**
 * @brief Reset transceiver
 */
static retval_t trx_reset(void)
{
    tal_trx_status_t trx_status;
    uint8_t poll_counter = 0;
#if (EXTERN_EEPROM_AVAILABLE == 1)
    uint8_t xtal_trim_value;
#endif

    /* Get trim value for 16 MHz xtal; needs to be done before reset */
#if (EXTERN_EEPROM_AVAILABLE == 1)
    pal_ps_get(EXTERN_EEPROM, EE_XTAL_TRIM_ADDR, 1, &xtal_trim_value);
#endif

    /* trx might sleep, so wake it up */
    PAL_SLP_TR_LOW();
    pal_timer_delay(SLEEP_TO_TRX_OFF_US);

    PAL_RST_LOW();
    pal_timer_delay(RST_PULSE_WIDTH_US);
    PAL_RST_HIGH();

    /* verify that trx has reached TRX_OFF */
    do
    {
        trx_status = (tal_trx_status_t)pal_trx_bit_read(SR_TRX_STATUS);
        poll_counter++;
        if (poll_counter > 250)
        {
#if (DEBUG > 0)
            pal_alert();
#endif
            return FAILURE;
        }

    } while (trx_status != TRX_OFF);

    tal_trx_status = TRX_OFF;

    // Write 16MHz xtal trim value to trx.
    // It's only necessary if it differs from the reset value.
#if (EXTERN_EEPROM_AVAILABLE == 1)
    if (xtal_trim_value != 0x00)
    {
        pal_trx_bit_write(SR_XTAL_TRIM, xtal_trim_value);
    }
#endif

    return MAC_SUCCESS;
}


/**
 * @brief Resets TAL state machine and forces transceiver off
 *
 * This function resets the TAL state machine. The transceiver is turned off
 * using FORCE_TRX_OFF and tal_state is initialized to TAL_IDLE.
 * It aborts any ongoing transaction. Used for debugging purposes only.
 */
#if (DEBUG > 0)
void tal_trx_state_reset(void)
{
    set_trx_state(CMD_FORCE_TRX_OFF);
    tal_state = TAL_IDLE;
}
#endif


/**
 * @brief Resets TAL state machine and sets the default PIB values if requested
 *
 * @param set_default_pib Defines whether PIB values need to be set
 *                        to its default values
 */
retval_t tal_reset(bool set_default_pib)
{
    if (set_default_pib)
    {
        /* Set the default PIB values */
        init_tal_pib(); /* implementation can be found in 'tal_pib.c' */
    }
    else
    {
        /* nothing to do - the current TAL PIB attribute values are used */
    }

    if (trx_reset() != MAC_SUCCESS)
    {
        return FAILURE;
    }
    trx_config();
    trx_config_csma();

#if (MAC_SCAN_ED_REQUEST_CONFIRM == 1)
    /* Stop the ED sample timer. */
#ifdef ENABLE_HIGH_PRIO_TMR
    pal_stop_high_priority_timer(TAL_ED_SAMPLE_TIMER);
#endif  /* ENABLE_HIGH_PRIO_TMR */
#endif  /* (MAC_SCAN_ED_REQUEST_CONFIRM == 1) */

#if (NUMBER_OF_TAL_TIMERS > 0)
    /* Clear all running TAL timers. */
    {
        uint8_t timer_id;

        ENTER_CRITICAL_REGION();
        for (timer_id = TAL_FIRST_TIMER_ID; timer_id <= TAL_LAST_TIMER_ID;
             timer_id++)
        {
            pal_timer_stop(timer_id);
        }
        LEAVE_CRITICAL_REGION();
    }
#endif

    /* Clear TAL Incoming Frame queue and free used buffers. */
    if (tal_incoming_frame_queue.size > 0)
    {
        buffer_t *frame;

        while (tal_incoming_frame_queue.size > 0)
        {
            frame = qmm_queue_remove(&tal_incoming_frame_queue, NULL);
            if (NULL != frame)
            {
                bmm_buffer_free(frame);
            }
        }
    }

    /*
     * Write all PIB values to the transceiver
     * that are needed by the transceiver itself.
     */
    write_all_tal_pib_to_trx(); /* implementation can be found in 'tal_pib.c' */

    tal_state = TAL_IDLE;
#ifdef BEACON_SUPPORT
    tal_csma_state = CSMA_IDLE;
#endif  /* BEACON_SUPPORT */
    tal_rx_on_required = false;

    /*
     * Configure interrupt handling.
     * Install a handler for the transceiver interrupt.
     */
    pal_trx_irq_init(TRX_MAIN_IRQ_HDLR_IDX, (FUNC_PTR)trx_irq_handler_cb);
    /* The pending transceiver interrupts on the microcontroller are cleared. */
    pal_trx_irq_flag_clr(TRX_MAIN_IRQ_HDLR_IDX);
    pal_trx_irq_enable(TRX_MAIN_IRQ_HDLR_IDX);     /* Enable transceiver interrupts. */

#ifdef ENABLE_FTN_PLL_CALIBRATION
    {
        /* Handle PLL calibration and filter tuning. */
        retval_t timer_status;

        /* Calibration timer has already been stopped within this function. */

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

    return MAC_SUCCESS;
}



/**
 * @brief Generates a 16-bit random number used as initial seed for srand()
 */
static void generate_rand_seed(void)
{
    uint16_t seed;
    /*
     * For AT86RF230B  bysed systems the random seed needs to be generated
     * by the PAL, since there is no tranceiver based random number generator
     * available.
     */
    seed = pal_generate_rand_seed();

    /* Set the seed for the random number generator. */
    srand(seed);
}

#endif /* (AT86RF230B == TAL_TYPE) */

/* EOF */

