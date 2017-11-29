/**
 * @file tal_tx.c
 *
 * @brief This file handles the frame transmission within the TAL.
 *
 * $Id: tal_tx.c,v 1.5 2010-08-05 05:41:48 slb Exp $
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
#include <string.h>
#include <stdbool.h>
#include "../../../PAL/Inc/pal.h"
#include "../../../Include/return_val.h"
#include "../../Inc/tal.h"
#include "../../../Include/ieee_const.h"
#include "../Inc/tal_pib.h"
#include "../Inc/tal_irq_handler.h"
#include "../../../Include/_tal_constants.h"
#include "../Inc/tal_tx.h"
#include "../../../Include/stack_config.h"
#include "../../../Resources/Buffer_Management/Inc/bmm.h"
#include "../../../Resources/Queue_Management/Inc/qmm.h"
#include "../Inc/tal_rx.h"
#include "../../../Include/_tal_internal.h"
#include "../Inc/at86rf230b.h"
#ifdef BEACON_SUPPORT
#include "../Inc/tal_slotted_csma.h"
#endif  /* BEACON_SUPPORT */
#include "../../../Include/mac_build_config.h"

/* === TYPES =============================================================== */


/* === MACROS ============================================================== */

/*
 * Command Frame Identifier for Association Request
 */
#define ASSOCIATION_REQUEST             (0x01)

/*
 * Mask used to check if the frame control has a Source address
 */
#define SRC_ADDR_MASK                   (0x0800)

/*
 * Mask used to check if the frame control has a Destination address
 */
#define DEST_ADDR_MASK                  (0x8000)

/*
 * Mask used to extract TAL main state
 */
#define TAL_MAIN_STATE_MASK             (0x0F)

/*
 * Mask used to extract the TAL tx sub state
 */
#define TAL_TX_SUB_STATE_MASK           (0xF0)

/* === GLOBALS ============================================================= */

#if ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT))
/**
 * Pointer to 15.4 frame structure for beacon frames.
 */
static uint8_t *beacon_frame;
#endif /* ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT)) */

/* === PROTOTYPES ========================================================== */

static uint8_t * frame_create(frame_info_t *frame_info);

#if ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT))
static inline void send_beacon(uint8_t *beacon_frame);
#endif /* ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT)) */

/* === IMPLEMENTATION ====================================================== */

/**
 * @brief Requests to TAL to transmit frame
 *
 * This function is called by the MAC to deliver a frame to the TAL
 * to be transmitted by the transceiver.
 *
 * @param mac_frame_info Pointer to the frame_info_t structure updated by
 *                       the MAC layer
 * @param csma_mode Indicates mode of csma-ca to be performed for this frame
 * @param perform_frame_retry Indicates whether to retries are to be performed for
 *                            this frame
 *
 * @return MAC_SUCCESS  if the TAL has accepted the data from the MAC for frame
 *                 transmission
 *         TAL_BUSY if the TAL is busy servicing the previous MAC request
 */
retval_t tal_tx_frame(frame_info_t *mac_frame_info,
                      csma_mode_t csma_mode,
                      bool perform_frame_retry)
{
    if (tal_state != TAL_IDLE)
    {
        return TAL_BUSY;
    }

    /*
     * Store the pointer to the provided frame structure.
     * This is needed for the callback function.
     */
    mac_frame_ptr = mac_frame_info;

#ifdef TEST_HARNESS
    if (1 == tal_pib_PrivateCCAFailure)
    {
        /*
         * Pretend CCA failure for CCA test purposes.
         * Setting the corresponding TAL states will initiate a
         * tal_tx_frame_done_cb() callback with CCA failure.
         */

        tal_state = TAL_TX_AUTO;
        tal_state |= TAL_TX_ACCESS_FAILURE;
        return MAC_SUCCESS;
    }
#endif

    /* Create the frame to be downloaded to the transceiver. */
    tal_frame_to_tx = frame_create(mac_frame_info);

    /*
     * In case the frame is too large, return immediately indicating
     * invalid status.
     */
    if (tal_frame_to_tx == NULL)
    {
        return MAC_INVALID_PARAMETER;
    }

#ifdef BEACON_SUPPORT
    // check if beacon mode is used
    if (csma_mode == CSMA_SLOTTED)
    {
        slotted_csma_start(perform_frame_retry);
    }
    else
    {
        send_frame(tal_frame_to_tx, csma_mode, perform_frame_retry);
    }
#else   /* No BEACON_SUPPORT */
    send_frame(tal_frame_to_tx, csma_mode, perform_frame_retry);
#endif  /* BEACON_SUPPORT */

    return MAC_SUCCESS;
}


/**
 * @brief Implements the TAL tx state machine.
 *
 * This function implements the TAL tx state machine.
 */
void tx_state_handling(void)
{
    tal_tx_sub_state_t tx_sub_state = (tal_tx_sub_state_t)(tal_state & TAL_TX_SUB_STATE_MASK);

#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
    switch (tal_state & TAL_MAIN_STATE_MASK)
    {
        case TAL_TX_AUTO:
            switch (tx_sub_state)
            {
                case TAL_TX_FRAME_PENDING:
                    tal_state = TAL_IDLE;
                    tal_tx_frame_done_cb(TAL_FRAME_PENDING, mac_frame_ptr);
                    break;

                case TAL_TX_SUCCESS:
                    tal_state = TAL_IDLE;
                    tal_tx_frame_done_cb(MAC_SUCCESS, mac_frame_ptr);
                    break;

                case TAL_TX_ACCESS_FAILURE:
                    tal_state = TAL_IDLE;
                    tal_tx_frame_done_cb(MAC_CHANNEL_ACCESS_FAILURE, mac_frame_ptr);
                    break;

                case TAL_TX_NO_ACK:
                    tal_state = TAL_IDLE;
                    tal_tx_frame_done_cb(MAC_NO_ACK, mac_frame_ptr);
                    break;

                case TAL_TX_FAILURE:
                    tal_state = TAL_IDLE;
                    tal_tx_frame_done_cb(FAILURE, mac_frame_ptr);
                    break;

                default:
                    break;
            }
            break;

        case TAL_TX_BASIC:
            switch (tx_sub_state)
            {
                case TAL_TX_SUCCESS:
                    tal_state = TAL_IDLE;
                    tal_tx_frame_done_cb(MAC_SUCCESS, mac_frame_ptr);
                    break;

                default:
                    break;
            }
            break;

#if ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT))
        case TAL_TX_BEACON:
            switch (tx_sub_state)
            {
                case TAL_TX_SUCCESS:
                    if (tal_csma_state == BACKOFF_WAITING_FOR_BEACON)
                    {
                        tal_csma_state = CSMA_HANDLE_BEACON;
                        tal_state = TAL_SLOTTED_CSMA;
                    }
                    else
                    {
                        tal_state = TAL_IDLE;
                    }
                    break;

                default:
                    break;
            }
            break;
#endif /* ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT)) */
    }
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
    switch (tal_state & TAL_MAIN_STATE_MASK)
    {
        case TAL_TX_AUTO:
            switch (tx_sub_state)
            {
                case TAL_TX_FRAME_PENDING:
                    tal_state = TAL_IDLE;
                    _g_tal_tx_frame_done_cb_fp(TAL_FRAME_PENDING, mac_frame_ptr);
                    break;

                case TAL_TX_SUCCESS:
                    tal_state = TAL_IDLE;
                    _g_tal_tx_frame_done_cb_fp(MAC_SUCCESS, mac_frame_ptr);
                    break;

                case TAL_TX_ACCESS_FAILURE:
                    tal_state = TAL_IDLE;
                    _g_tal_tx_frame_done_cb_fp(MAC_CHANNEL_ACCESS_FAILURE, mac_frame_ptr);
                    break;

                case TAL_TX_NO_ACK:
                    tal_state = TAL_IDLE;
                    _g_tal_tx_frame_done_cb_fp(MAC_NO_ACK, mac_frame_ptr);
                    break;

                case TAL_TX_FAILURE:
                    tal_state = TAL_IDLE;
                    _g_tal_tx_frame_done_cb_fp(FAILURE, mac_frame_ptr);
                    break;

                default:
                    break;
            }
            break;

        case TAL_TX_BASIC:
            switch (tx_sub_state)
            {
                case TAL_TX_SUCCESS:
                    tal_state = TAL_IDLE;
                    _g_tal_tx_frame_done_cb_fp(MAC_SUCCESS, mac_frame_ptr);
                    break;

                default:
                    break;
            }
            break;

#if ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT))
        case TAL_TX_BEACON:
            switch (tx_sub_state)
            {
                case TAL_TX_SUCCESS:
                    if (tal_csma_state == BACKOFF_WAITING_FOR_BEACON)
                    {
                        tal_csma_state = CSMA_HANDLE_BEACON;
                        tal_state = TAL_SLOTTED_CSMA;
                    }
                    else
                    {
                        tal_state = TAL_IDLE;
                    }
                    break;

                default:
                    break;
            }
            break;
#endif /* ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT)) */
    }
#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
} /* tx_state_handling() */


/**
 * @brief Creates MAC frame
 *
 * This function is called to create a MAC frame using the information
 * passed to tal_tx_frame().
 *
 * @param tal_frame_info Pointer to the frame_info_t structure updated by
 *                       the MAC layer
 *
 * @return Pointer to the created frame header if the frame length is not
 *         larger than aMaxPHYPacketSize, NULL otherwise
 */
static uint8_t *frame_create(frame_info_t *tal_frame_info)
{
    uint8_t frame_length;
    uint16_t frame_format_mask;
    uint8_t *frame_header;

    /*
     * Start creating the frame header backwards starting from the payload.
     * Note: This approach does not require copying the payload again
     * into the frame buffer.
     */
    frame_header = tal_frame_info->payload;

    /* Start with the frame length set to the payload length. */
    frame_length = tal_frame_info->payload_length;

    /* Get the source addressing information. */
    frame_format_mask = tal_frame_info->frame_ctrl & SRC_ADDR_MODE_MASK;

    /*
     * As the frame creation is done backwards, the source address field
     * of the frame header is updated with the source address
     */
    if (SRC_EXT_ADDR_MODE == frame_format_mask)
    {
        frame_header -= EXT_ADDR_LEN;

        convert_64_bit_to_byte_array(tal_frame_info->src_address,
                                     frame_header);

        frame_length += EXT_ADDR_LEN;
    }
    else if (SRC_SHORT_ADDR_MODE == frame_format_mask)
    {
        frame_header -= SHORT_ADDR_LEN;

        convert_16_bit_to_byte_array(tal_frame_info->src_address,
                                     frame_header);

        frame_length += SHORT_ADDR_LEN;
    }

    if (frame_format_mask)
    {
        if ((tal_frame_info->frame_ctrl & FCF_PAN_ID_COMPRESSION) &&
            (tal_frame_info->frame_ctrl & DEST_ADDR_MODE_MASK)
           )
        {
            /*
             * If the Intra-PAN bit is set to 1
             * and both the destination and source addresses are present,
             * then do NOT include the source PAN ID.
             */
        }
        else
        {
            /* Add source PAN ID. */
            frame_header -= PAN_ID_LEN;

            convert_16_bit_to_byte_array(tal_frame_info->src_panid,
                                         frame_header);

            frame_length += PAN_ID_LEN;
        }
    }

    /* Get the destination addressing information. */
    frame_format_mask = tal_frame_info->frame_ctrl & DEST_ADDR_MODE_MASK;

    /*
     * The destination address shall be included in the MAC frame only,
     * if the destination addressing mode subfield of the
     * frame control field is nonzero.
     */
    if (DEST_EXT_ADDR_MODE == frame_format_mask)
    {
        frame_header -= EXT_ADDR_LEN;

        convert_64_bit_to_byte_array(tal_frame_info->dest_address,
                                     frame_header);

        frame_length += EXT_ADDR_LEN;
    }
    else if (DEST_SHORT_ADDR_MODE == frame_format_mask)
    {
        frame_header -= SHORT_ADDR_LEN;

        convert_16_bit_to_byte_array(tal_frame_info->dest_address,
                                     frame_header);

        frame_length += SHORT_ADDR_LEN;
    }

    /*
     * The destination PAN ID shall be included in the MAC frame only,
     * if the destination addressing mode subfield of the
     * frame control field is nonzero.
     */
    if (frame_format_mask)
    {
        frame_header -= PAN_ID_LEN;

        convert_16_bit_to_byte_array(tal_frame_info->dest_panid,
                                     frame_header);

        frame_length += PAN_ID_LEN;
    }

    /*
     * As the frame header creation is done backwards
     * and the next field to be updated is the sequence number,
     * update the sequence number into the frame header
     * by decrementing frame_header by one.
     */
    frame_header--;

    *frame_header = tal_frame_info->seq_num;

    /*
     * The frame length is incremented to includ the length of the
     * sequence number.
     */
    frame_length++;

    /*
     * The next field to be updated is the frame control field of the
     * frame header.
     */
    frame_header -= FCF_LEN;

    convert_16_bit_to_byte_array(tal_frame_info->frame_ctrl,
                                 frame_header);

    /* Include space for FCS and FCF field. */
    frame_length += FCF_LEN + FCS_LEN;

    /*
     * The PHY header contains the length of the frame to be transmitted.
     * This is appended to the created frame.
     */
    frame_header--;

    *frame_header = frame_length;

    /*
     * Frame length is not supposed to be longer than 127 octets
     * (aMaxPHYPacketSize), otherwise the transmission status is undefined.
     */
    if (frame_length > aMaxPHYPacketSize)
    {
        return NULL;
    }

    return (frame_header);
}  /* frame_create() */


/**
 * @brief Sends frame
 *
 * @param frame_tx Pointer to prepared frame
 * @param use_csma Flag indicating if CSMA is requested
 * @param tx_retries Flag indicating if transmission retries are requested
 *                   by the MAC layer
 */
void send_frame(uint8_t *frame_tx, csma_mode_t csma_mode, bool tx_retries)
{
    tal_trx_status_t trx_status;

#if (DEBUG > 0)
    if ((tx_retries) && ((csma_mode == NO_CSMA_NO_IFS) || (csma_mode == NO_CSMA_WITH_IFS)))
    {
        ASSERT((tx_retries == true) && ((csma_mode == NO_CSMA_NO_IFS) || (csma_mode == NO_CSMA_WITH_IFS)));
    }
#endif

    // configure tx according to tx_retries
    if (tx_retries)
    {
        pal_trx_bit_write(SR_MAX_FRAME_RETRIES, tal_pib_MaxFrameRetries);
    }
    else if ((csma_mode != NO_CSMA_NO_IFS) && (csma_mode != NO_CSMA_WITH_IFS)) // only necessary while using auto modes
    {
        pal_trx_bit_write(SR_MAX_FRAME_RETRIES, 0);
    }

    /* Prepare trx for transmission depending on the csma mode. */
    if ((csma_mode == NO_CSMA_NO_IFS) || (csma_mode == NO_CSMA_WITH_IFS))
    {
        do
        {
            trx_status = set_trx_state(CMD_PLL_ON);
        } while (trx_status != PLL_ON);

        tal_state = TAL_TX_BASIC;

        /* Handle interframe spacing */
        if (csma_mode == NO_CSMA_WITH_IFS)
        {
            if (last_frame_length > aMaxSIFSFrameSize)
            {
                pal_timer_delay(TAL_CONVERT_SYMBOLS_TO_US(macMinLIFSPeriod_def)
                                - IRQ_PROCESSING_DLY_US - PRE_TX_DURATION_US);
            }
            else
            {
                pal_timer_delay(TAL_CONVERT_SYMBOLS_TO_US(macMinSIFSPeriod_def)
                                - IRQ_PROCESSING_DLY_US - PRE_TX_DURATION_US);
            }
        }
    }
    else
    {
        do
        {
            trx_status = set_trx_state(CMD_TX_ARET_ON);
        } while (trx_status != TX_ARET_ON);

        tal_state = TAL_TX_AUTO;
    }

    pal_trx_irq_disable(TRX_MAIN_IRQ_HDLR_IDX);

    /* Toggle the SLP_TR pin triggering transmission. */
    PAL_SLP_TR_HIGH();
    PAL_WAIT_65_NS();
    PAL_SLP_TR_LOW();

    /*
     * Send the frame to the transceiver.
     * Note: The PhyHeader is the first byte of the frame to
     * be sent to the transceiver and this contains the frame
     * length.
     */
    pal_trx_frame_write(frame_tx, frame_tx[0]-1);

#ifndef NON_BLOCKING_SPI
    pal_trx_irq_enable(TRX_MAIN_IRQ_HDLR_IDX);
#endif
}


/**
 * @brief Handles interrupts issued due to end of transmission
 */
void handle_tx_end_irq(bool underrun_occured)
{
    uint8_t trac_status;

    if (tal_state == TAL_TX_AUTO)
    {
        if (underrun_occured)
        {
            trac_status = TRAC_INVALID;
        }
        else
        {
            trac_status = pal_trx_bit_read(SR_TRAC_STATUS);
        }

        /* Map status message of transceiver to TAL constants. */
        switch (trac_status)
        {
            case TRAC_SUCCESS_DATA_PENDING:
                tal_state |= TAL_TX_FRAME_PENDING;
                break;

            case TRAC_SUCCESS:
                tal_state |= TAL_TX_SUCCESS;
                break;

            case TRAC_CHANNEL_ACCESS_FAILURE:
                tal_state |= TAL_TX_ACCESS_FAILURE;
                break;

            case TRAC_NO_ACK:
                tal_state |= TAL_TX_NO_ACK;
                break;

            case TRAC_INVALID:
                tal_state |= TAL_TX_FAILURE;
                break;

            default:
                ASSERT("not handled trac status" == 0);
                tal_state |= TAL_TX_FAILURE;
                break;
        }
    }
    else if (tal_state == TAL_TX_BASIC)
    {
        tal_state |= TAL_TX_SUCCESS;
    }
#if ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT))
    else    // TAL_TX_BEACON
    {
        // debug pin to switch on: define ENABLE_DEBUG_PINS, pal_config.h
        PIN_BEACON_END();

        tal_state |= TAL_TX_SUCCESS;
    }
#endif /* ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT)) */
}


/**
 * @brief Prepares the beacon frame to be sent at the start of superframe
 *
 * This function prepares the beacon frame to be sent at the start of
 * the superframe
 *
 * @param mac_frame_info Pointer to the frame_info_t structure
 */
#if ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT))
void tal_prepare_beacon(frame_info_t *mac_frame_info)
{
    /*
     * Create the beacon frame and buffer the beacon frame to be sent later
     * when tal_tx_beacon() is called.
     */
    beacon_frame = frame_create(mac_frame_info);
}
#endif /* ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT)) */


/**
 * @brief Triggers actual beacon frame transmission
 */
#if ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT))
void tal_tx_beacon(void)
{
    /*
     * Avoid that the beacon is transmitted while transmitting
     * a frame using slotted CSMA.
     */
    if ((tal_csma_state == FRAME_SENDING_WITH_ACK) ||
        (tal_csma_state == FRAME_SENDING_NO_ACK) ||
        (tal_csma_state == WAITING_FOR_ACK))
    {
        ASSERT("trying to transmit while slotted CSMA transmits or wait for an ACK" == 0);
        return;
    }

    /* Send the pre-created beacon frame to the transceiver. */
    send_beacon(beacon_frame);
    tal_state = TAL_TX_BEACON;
}
#endif /* ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT)) */



/**
 * @brief Sends the beacon frame
 */
#if ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT))
static inline void send_beacon(uint8_t *beacon_frame)
{
    tal_trx_status_t trx_status;

    // debug pin to switch on: define ENABLE_DEBUG_PINS, pal_config.h
    PIN_BEACON_START();

    // @TODO wait for talbeaconTxTime
    do
    {
        trx_status = set_trx_state(CMD_FORCE_PLL_ON);
#if (DEBUG > 1)
        if (trx_status != PLL_ON)
        {
            ASSERT("Force PLL_ON failed for beacon transmission" == 0);
        }
#endif
    } while (trx_status != PLL_ON);


    pal_trx_irq_disable(TRX_MAIN_IRQ_HDLR_IDX);

    /* Toggle the SLP_TR pin triggering transmission. */
    PAL_SLP_TR_HIGH();
    PAL_WAIT_65_NS();
    PAL_SLP_TR_LOW();

    /*
     * Send the frame to the transceiver.
     * Note: The PHY Header is the first byte of the frame to
     * be sent to the transceiver and this contains the frame
     * length.
     */
    pal_trx_frame_write(beacon_frame, beacon_frame[0]-1);

#ifndef NON_BLOCKING_SPI
    pal_trx_irq_enable(TRX_MAIN_IRQ_HDLR_IDX);
#endif
}
#endif /* ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT)) */

#endif /* (AT86RF230B == TAL_TYPE) */

/* EOF */

