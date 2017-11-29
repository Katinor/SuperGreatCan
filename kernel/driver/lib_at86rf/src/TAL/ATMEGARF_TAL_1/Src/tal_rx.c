/**
 * @file tal_rx.c
 *
 * @brief This file implements the frame reception functions.
 *
 * $Id: tal_rx.c,v 1.5 2010-08-20 07:35:20 challenger38 Exp $
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
#include "../../Inc/tal.h"
#include "../../../Include/ieee_const.h"
#include "../../../Include/stack_config.h"
#include "../../../Resources/Buffer_Management/Inc/bmm.h"
#include "../../../Resources/Queue_Management/Inc/qmm.h"
#include "../../../Include/_tal_constants.h"
#include "../Inc/tal_pib.h"
#include "../Inc/tal_irq_handler.h"
#include "../Inc/atmega128rfa1.h"
#include "../Inc/tal_rx.h"
#include "../../../Include/_tal_internal.h"
#if (MAC_INDIRECT_DATA_FFD == 1)
#include "../../../Include/indirect_data_structures.h"
#endif  /* (MAC_INDIRECT_DATA_FFD == 1) */
#ifdef BEACON_SUPPORT
#include "../Inc/tal_slotted_csma.h"
#endif  /* BEACON_SUPPORT */

/* === TYPES =============================================================== */


/* === MACROS ============================================================== */

/* Constant defines for the LQI calculation */
#define ED_THRESHOLD                    (30)
#define ED_MAX_VAL                      (-RSSI_BASE_VAL - ED_THRESHOLD)
#define LQI_MAX                         (3)
#define UPPER_CLIPPING_VALUE            (-30)
/* Constant define for the ED scaling: register value at -35dBm */
#define CLIP_VALUE_REG                  (55)

#define US_PER_OCTECT                   (32)

/* === GLOBALS ============================================================= */


/* === PROTOTYPES ========================================================== */

#ifdef RSSI_TO_LQI_MAPPING
static inline uint8_t normalize_lqi(uint8_t ed_value);
#else
static inline uint8_t normalize_lqi(uint8_t lqi, uint8_t ed_value);
#endif

/* === IMPLEMENTATION ====================================================== */

#ifndef SNIFFER
/**
 * @brief Handle received frame interrupt
 *
 * This function handles transceiver interrupts for received frames and
 * uploads the frames from the trx.
 */
void handle_received_frame_irq(void)
{
    uint8_t ed_value;
    uint8_t frame_length;
    uint8_t phr;
    uint8_t *rx_frame_ptr;
    uint8_t *store_info_ptr;

#if (DEBUG > 0)
    /* This should never happen. Therefore it's only used while debugging. */
    if (tal_rx_buffer == NULL)
    {
        ASSERT("no tal_rx_buffer available" == 0);
        return;
    }
#endif

#ifdef PROMISCUOUS_MODE
    if (tal_pib_PromiscuousMode)
    {
        /* Check for valid FCS */
        if (pal_trx_bit_read(SR_RX_CRC_VALID) == CRC16_NOT_VALID)
        {
            return;
        }
    }
#endif

    /* Get ED value; needed to normalize LQI. */
    ed_value = pal_trx_reg_read(RG_PHY_ED_LEVEL);

    /* Upload frame from transceiver. */
    phr = frame_length = pal_trx_reg_read(RG_TST_RX_LENGTH);

    /* Check for valid frame length. */
    if (frame_length > 127)
    {
        return;
    }

    /*
     * The PHY header is also included in the frame, hence the frame length
     * is incremented by 1. In addition to that, the LQI needs to be uploaded, too.
     */
    frame_length += LQI_LEN + LENGTH_FIELD_LEN;
    /* Get a reference to the receive storage location. */
    rx_frame_ptr = BMM_BUFFER_POINTER(tal_rx_buffer);
    rx_frame_ptr = rx_frame_ptr + (LARGE_BUFFER_SIZE - frame_length);
    *rx_frame_ptr++ = phr;
    pal_trx_frame_read(rx_frame_ptr, frame_length - 1);

    store_info_ptr = BMM_BUFFER_POINTER(tal_rx_buffer);

    ((rx_frame_info_t*)store_info_ptr)->frame_length = frame_length;
    ((rx_frame_info_t*)store_info_ptr)->ed_level = ed_value;

    /*
     * The timestamp is stored and will be used while creating the
     * frame_info_t structure to the MAC layer.
     */
    ((rx_frame_info_t*)store_info_ptr)->timestamp = tal_rx_timestamp;

    /* Append received frame to incoming_frame_queue and get new rx buffer. */
    qmm_queue_append(&tal_incoming_frame_queue, tal_rx_buffer);

    /* The previous buffer is eaten up and a new buffer is not assigned yet. */
    tal_rx_buffer = bmm_buffer_alloc(LARGE_BUFFER_SIZE);

    /* Check if receive buffer is available */
    if (NULL == tal_rx_buffer)
    {
        /*
         * Turn off the receiver until a buffer is available again.
         * tal_task() will take care of eventually reactivating it.
         */
        TRX_STATE = CMD_PLL_ON;
        tal_rx_on_required = true;
    }
    else
    {
        /*
         * Trx returns to RX_AACK_ON automatically, if this was its previous state.
         * Keep the following as a reminder, if receiver is used with RX_ON instead.
         */
        //pal_trx_reg_write(RG_TRX_STATE, CMD_RX_AACK_ON);
    }
}
#endif


/**
 * @brief Parses received frame and create the frame_info_t structure
 *
 * This function parses the received frame and creates the frame_info_t
 * structure to be sent to the MAC as a parameter of tal_rx_frame_cb().
 *
 * @param buf Pointer to the buffer containing the received frame
 */
void process_incoming_frame(buffer_t *buf)
{
    uint16_t src_address_mode;
    uint16_t dest_address_mode;
    frame_info_t *frame_to_mac;
    uint8_t frame_length;
    uint8_t *rx_frame_ptr;
    uint8_t ed_level;
    uint8_t lqi;
    uint8_t *incoming_frame = BMM_BUFFER_POINTER(buf);

    frame_to_mac = (frame_info_t*)incoming_frame;

    /*
     * The frame is present towards the end of the buffer. Hence, using the
     * frame length as the reference, the location of the received frame within
     * the buffer is computed.
     */
    frame_length = ((rx_frame_info_t *)incoming_frame)->frame_length;
    rx_frame_ptr = incoming_frame + (LARGE_BUFFER_SIZE - frame_length);

    /*
     * Store the last frame length for IFS handling.
     * Substract LQI and length fields.
     */
    last_frame_length = frame_length - 2;

    /* The following handling is based on a frame_length w/o LQI value */
    frame_length -= LQI_LEN;

    ed_level = ((rx_frame_info_t *)incoming_frame)->ed_level;

    /* Copy the timestamp to the structure. */
    frame_to_mac->time_stamp = ((rx_frame_info_t *)incoming_frame)->timestamp;

    /* The first byte of the received frame is the frame length. */
    rx_frame_ptr++;

    frame_to_mac->frame_ctrl = convert_byte_array_to_16_bit(rx_frame_ptr);

#ifdef PROMISCUOUS_MODE
    if (tal_pib_PromiscuousMode)
    {
        frame_to_mac->payload = rx_frame_ptr;

        /* Promiscuous mode requires setting address mode to 0x00 */
        frame_to_mac->frame_ctrl &= (uint16_t)(~FCF_SET_SOURCE_ADDR_MODE(0x03));
        frame_to_mac->frame_ctrl &= (uint16_t)(~FCF_SET_DEST_ADDR_MODE(0x03));

        /*
         * The LQI is accessed by moving to the end of the frame.
         */
        frame_length--;
        rx_frame_ptr += frame_length;
        lqi = *rx_frame_ptr;

        frame_to_mac->payload_length = frame_length - FCS_LEN;

        /*
         * The LQI normalization is done using the ED level measured during
         * the frame reception.
         */
#ifdef RSSI_TO_LQI_MAPPING
        lqi = normalize_lqi(ed_level);
#else
        lqi = normalize_lqi(lqi, ed_level);
#endif

        frame_to_mac->buffer_header = buf;

         /* The callback function implemented by MAC is invoked. */
#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
        tal_rx_frame_cb((frame_info_t *)incoming_frame, lqi);
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
        _g_tal_rx_frame_cb_fp((frame_info_t *)incoming_frame, lqi);
#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */

        return;
    }
#endif   /* #ifdef PROMISCUOUS_MODE */

#ifdef BEACON_SUPPORT
    /*
     * Are we waiting for a beacon for slotted CSMA?
     * Check if received frame is a beacon.
     */
    if ((frame_to_mac->frame_ctrl & FRAME_TYPE_MASK) == BEACON_FRAME)
    {
        /* Debug pin to switch on: define ENABLE_DEBUG_PINS, pal_config.h */
        PIN_BEACON_START();

        if (tal_csma_state == BACKOFF_WAITING_FOR_BEACON)
        {
            /* Debug pin to switch on: define ENABLE_DEBUG_PINS, pal_config.h */
            PIN_WAITING_FOR_BEACON_END();
            tal_pib_BeaconTxTime = TAL_CONVERT_US_TO_SYMBOLS(frame_to_mac->time_stamp);
            tal_csma_state = CSMA_HANDLE_BEACON;
        }

        /* Debug pin to switch on: define ENABLE_DEBUG_PINS, pal_config.h */
        PIN_BEACON_END();
    }
#endif  /* BEACON_SUPPORT */

    rx_frame_ptr++;

    /*
     * The source and the destination address mode is extracted from the
     * frame control field of the received frame.
     */
    dest_address_mode = frame_to_mac->frame_ctrl & DEST_ADDR_MODE_MASK;
    src_address_mode  = frame_to_mac->frame_ctrl & SRC_ADDR_MODE_MASK;
    rx_frame_ptr++;

    /*
     * The frame_info_t is updated with the sequence number of the received
     * frame
     */
    frame_to_mac->seq_num = *rx_frame_ptr;
    rx_frame_ptr++;

    if (dest_address_mode != 0)
    {
        frame_to_mac->dest_panid = convert_byte_array_to_16_bit(rx_frame_ptr);
        rx_frame_ptr += PAN_ID_LEN;

        /*
         * This variable holds the length of the received frame.
         * The length includes the frame header length (ie: MHR),
         * payload length (ie: MPDU), and the length of the FCS field.
         * The length of the payload is extracted by gradually decrementing
         * this variable for each of the elements present within the
         * frame header(MHR) of the received frame.
         */
        frame_length -= PAN_ID_LEN;

        /*
         * The frame_info_t structure is updated with the destination address.
         */
        if (DEST_SHORT_ADDR_MODE == dest_address_mode)
        {
            frame_to_mac->dest_address = convert_byte_array_to_16_bit(rx_frame_ptr);
            rx_frame_ptr += SHORT_ADDR_LEN;

            frame_length -= SHORT_ADDR_LEN;
        }
        else if (DEST_EXT_ADDR_MODE == dest_address_mode)
        {
            frame_to_mac->dest_address = convert_byte_array_to_64_bit(rx_frame_ptr);
            rx_frame_ptr += EXT_ADDR_LEN;

            frame_length -= EXT_ADDR_LEN;
        }
    }

    if (src_address_mode != 0)
    {
        if (!(frame_to_mac->frame_ctrl & FCF_PAN_ID_COMPRESSION))
        {
            /*
             * Source PAN ID is present in the frame only if the intra-PAN bit
             * is zero and src_address_mode is non zero.
             */
            frame_to_mac->src_panid = convert_byte_array_to_16_bit(rx_frame_ptr);
            rx_frame_ptr += PAN_ID_LEN;

            frame_length -= PAN_ID_LEN;
        }
        else
        {
            /*
             * The received frame does not contain a source PAN ID, hence
             * source PAN ID of the frame_info_t is updated with the
             * destination PAN ID.
             */
            frame_to_mac->src_panid = frame_to_mac->dest_panid;
        }

        /* The frame_info_t structure is updated with the source address. */
        if (SRC_SHORT_ADDR_MODE == src_address_mode)
        {
            frame_to_mac->src_address = convert_byte_array_to_16_bit(rx_frame_ptr);
            rx_frame_ptr += SHORT_ADDR_LEN;

            frame_length -= SHORT_ADDR_LEN;
        }
        else if (SRC_EXT_ADDR_MODE == src_address_mode)
        {
            frame_to_mac->src_address = convert_byte_array_to_64_bit(rx_frame_ptr);
            rx_frame_ptr += EXT_ADDR_LEN;

            frame_length -= EXT_ADDR_LEN;
        }
    }

    /* It is a valid frame, hence proceed */

    /* The payload length computed. */
    frame_length -= (PHY_HDR_LEN + FCF_LEN + SEQ_NUM_LEN + FCS_LEN);

    frame_to_mac->payload = rx_frame_ptr;
    frame_to_mac->payload_length = frame_length;

    /*
     * The LQI is stored after the FCS (2 Bytes).
     * The LQI is accessed by moving to the end of the frame.
     */
    rx_frame_ptr += frame_length;
    lqi = *(rx_frame_ptr + FCS_LEN);

    /*
     * The LQI normalization is done using the ED level measured during
     * the frame reception.
     */
#ifdef RSSI_TO_LQI_MAPPING
    lqi = normalize_lqi(ed_level);
#else
    lqi = normalize_lqi(lqi, ed_level);
#endif

    frame_to_mac->buffer_header = buf;

    /* The callback function implemented by MAC is invoked. */
#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
    tal_rx_frame_cb((frame_info_t *)incoming_frame, lqi);
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
    _g_tal_rx_frame_cb_fp((frame_info_t *)incoming_frame, lqi);
#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
} /* process_incoming_frame() */


#ifdef RSSI_TO_LQI_MAPPING
/**
 * @brief Normalize LQI
 *
 * This function normalizes the LQI value based on the RSSI/ED value.
 *
 * @param ed_value Read ED value
 *
 * @return The calculated/normalized LQI value: ppduLinkQuality
 */
static inline uint8_t normalize_lqi(uint8_t ed_value)
{
#if defined(EDLEVEL_TO_LQI_MAPPING)
    return ed_value;
#else
    /*
     * Scale ED result.
     * Clip values to 0xFF if > -35dBm
     */
    if (ed_value > CLIP_VALUE_REG)
    {
        return 0xFF;
    }
    else
    {
        return (uint8_t)(((uint16_t)ed_value * 0xFF) / CLIP_VALUE_REG);
    }
#endif /* defined(EDLEVEL_TO_LQI_MAPPING)*/
}

#else

/**
 * @brief Normalize LQI
 *
 * This function normalizes the LQI value based on the ED and
 * the originally appended LQI value.
 *
 * @param lqi Measured LQI
 * @param ed_value Read ED value
 *
 * @return The calculated LQI value: ppduLinkQuality
 */
static inline uint8_t normalize_lqi(uint8_t lqi, uint8_t ed_value)
{
    uint16_t link_quality;
    uint8_t lqi_star;

#ifdef HIGH_DATA_RATE_SUPPORT
    if (tal_pib_CurrentPage == 0)
    {
#endif
        if (ed_value > ED_MAX_VAL)
        {
            ed_value = ED_MAX_VAL;
        }
        else if (ed_value == 0)
        {
            ed_value = 1;
        }

        lqi_star = lqi >> 6;
        link_quality = (uint16_t)lqi_star * (uint16_t)ed_value * 255 / (ED_MAX_VAL * LQI_MAX);

        if (link_quality > 255)
        {
            return 255;
        }
        else
        {
            return (uint8_t)link_quality;
        }
#ifdef HIGH_DATA_RATE_SUPPORT
    }
    else    /* if (tal_pib_CurrentPage == 0) */
    {
        /* High data rate modes do not provide a valid LQI value. */
        if (ed_value > ED_MAX_VAL)
        {
            return 0xFF;
        }
        else
        {
            return (ed_value * (255 / ED_MAX_VAL));
        }
    }
#endif
}
#endif /* #ifdef RSSI_TO_LQI_MAPPING */


/*  EOF */

