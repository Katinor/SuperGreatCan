/**
 * @file app_config.h
 *
 * @brief These are application-specific resources which are used
 *        in the application in addition to the underlaying stack.
 *
 * $Id: app_config.h,v 1.1 2012-02-14 05:21:09 essusige Exp $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "../../../../ubiconfig.h"

/* === Includes ============================================================= */

#include "../../../Include/stack_config.h"

/* === Macros =============================================================== */

#if (NUMBER_OF_TOTAL_STACK_TIMERS == 0)
#define APP_FIRST_TIMER_ID          (0)
#else
#define APP_FIRST_TIMER_ID          (LAST_STACK_TIMER_ID + 1)
#endif

/* === Types ================================================================ */

/* Timer ID's used by the Application */
typedef enum
{
    /* App Timers start from APP_FIRST_TIMER_ID */

    /* App timer 1 */
    T_APP_TIMER = (APP_FIRST_TIMER_ID)
} app_timer_t;

#define NUMBER_OF_APP_TIMERS        (1)

#define TOTAL_NUMBER_OF_TIMERS      (NUMBER_OF_APP_TIMERS + NUMBER_OF_TOTAL_STACK_TIMERS)

/* Additional buffers used by the application */
#define NUMBER_OF_LARGE_APP_BUFS    (10)
#define NUMBER_OF_SMALL_APP_BUFS    (0)

#define TOTAL_NUMBER_OF_LARGE_BUFS  (NUMBER_OF_LARGE_APP_BUFS + NUMBER_OF_LARGE_STACK_BUFS)
#define TOTAL_NUMBER_OF_SMALL_BUFS  (NUMBER_OF_SMALL_APP_BUFS + NUMBER_OF_SMALL_STACK_BUFS)

#define TOTAL_NUMBER_OF_BUFS        (TOTAL_NUMBER_OF_LARGE_BUFS + TOTAL_NUMBER_OF_SMALL_BUFS)

/*
 * USB transmit buffer size
 */
#define USB_TX_BUF_SIZE             (10)

/*
 * USB receive buffer size
 */
#define USB_RX_BUF_SIZE             (10)

/*
 * USB-specific definitions
 */

/*
 * USB Vendor ID (16-bit number)
 */
#define USB_VID                 0x03EB /* Atmel's USB vendor ID */

/*
 * USB Product ID (16-bit number)
 */
#define USB_PID                 0x2018 /* RZ USB stick product ID */

/*
 * USB Release number (BCD format, two bytes)
 */
#define USB_RELEASE             { 0x00, 0x01 } /* 01.00 */

/*
 * Maximal number of UTF-16 characters used in any of the strings
 * below.  This is only used for compilers that cannot handle the
 * initialization of flexible array members within structs.
 */
#define USB_STRING_SIZE         10

/*
 * String representation for the USB vendor name.
 */
#define USB_VENDOR_NAME L"ATMEL"

/*
 * String representation for the USB product name.
 */
#define USB_PRODUCT_NAME L"RZUSBSTICK"

/*
 * UART transmit buffer size
 */
#define UART_MAX_TX_BUF_LENGTH      (10)

/*
 * UART receive buffer size
 */
#define UART_MAX_RX_BUF_LENGTH      (10)

/* Offset of IEEE address storage location within EEPROM */
#define EE_IEEE_ADDR                (0)

/* === Externals ============================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* APP_CONFIG_H */
/* EOF */
