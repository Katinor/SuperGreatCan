/**
 * @file pal_uart.h
 *
 * @brief PAL uart internal functions prototypes
 *
 * This header contains the function prototypes for transmit,
 * receive functions and macros used in UART module.
 *
 * $Id: pal_uart.h,v 1.2 2012-02-14 05:21:09 essusige Exp $
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
#ifndef PAL_UART_H
#define PAL_UART_H

#include "../../../../../ubiconfig.h"

/* === Includes ============================================================= */

#include "../../../../Include/_app_config.h"
#include "../../../../Include/_pal_config.h"

/* === Types ================================================================ */

#if (defined PAL_UART0)

/*
 * Structure containing the transmit and receive buffer
 * and also the buffer head, tail and count
 */
typedef struct uart_communication_buffer_tag
{
    /* Transmit buffer */
    uint8_t tx_buf[UART_MAX_TX_BUF_LENGTH];

    /* Receive buffer */
    uint8_t rx_buf[UART_MAX_RX_BUF_LENGTH];

    /* Transmit buffer head */
    uint8_t tx_buf_head;

    /* Transmit buffer tail */
    uint8_t tx_buf_tail;

    /* Receive buffer head */
    uint8_t rx_buf_head;

    /* Receive buffer tail */
    uint8_t rx_buf_tail;

    /* Number of bytes in transmit buffer */
    uint8_t tx_count;

    /* Number of bytes in receive buffer */
    uint8_t rx_count;

} uart_communication_buffer_t;

/* Irq handlers of UART */
typedef void (*uart_irq_handler_t) (void);

/* === Externals ============================================================ */


/* === Macros =============================================================== */

/* UART baud rate of in bits per second. */
#define UART_BAUD_9k6               (9600)

/*
 * Value to be loaded in the SCBR register of USART to obtain a baud rate of
 * 9600 bps.
 * As given in datasheet Baud Rate = Selected Clock / 16 /CD
 * hence CD = Baud Rate / Selected Clock / 16
 */
#define UART_BAUD_DIVISOR           (uint16_t) ((F_CPU / UART_BAUD_9k6) / 16)

/* PAL_UART0 */
#ifdef PAL_UART0

/* The pin on which the data is received by UART */
#define PIN_UART_0_RXD              (PIO_PA9A_URXD0)

/* The pin on which the data is transmitted by UART */
#define PIN_UART_0_TXD              (PIO_PA10A_UTXD0)

/* Enables the RX interrupt of PAL_UART0 */
#define ENABLE_UART_0_RX_INT()      (USART0->US_IER = US_IER_RXRDY)

/* Enables the TX interrupt of PAL_UART0 */
#define ENABLE_UART_0_TX_INT()      (USART0->US_IER = US_IER_TXRDY)

/* Disables the RX interrupt of PAL_UART0 */
#define DISABLE_UART_0_RX_INT()     (USART0->US_IDR = US_IDR_RXRDY)

/* Disables the TX interrupt of PAL_UART0 */
#define DISABLE_UART_0_TX_INT()     (USART0->US_IDR = US_IDR_TXRDY)

/* Writes data in the UART data register */
#define WRITE_TO_UART_0(tx_data)   do {                             \
    /*                                                              \
     * Data is written to tranmsit register only if there are is    \
     * no ongoing data transmission.                                \
     */                                                             \
    while ((USART0->US_CSR & US_CSR_TXEMPTY) == 0);       \
    USART0->US_THR = tx_data;                               \
} while (0);

/* Writes data in the UART data register */
#define READ_FROM_UART_0(rx_data)   do {                            \
    /*                                                              \
     * Reading  from receive register is held until there is some   \
     * data received in the receive register.                       \
     */                                                             \
    while ((USART0->US_CSR & US_CSR_RXRDY) == 0);         \
    rx_data = USART0->US_RHR;                               \
} while (0);

#endif  /* PAL_UART0 */

/* PAL_UART1 */
#ifdef PAL_UART1
/* The pin on which the data is received by UART */
#define PIN_UART_1_RXD              (AT91C_PA5_RXD1)

/* The pin on which the data is transmitted by UART */
#define PIN_UART_1_TXD              (AT91C_PA6_TXD1)

/* Enables the RX interrupt of PAL_UART1 */
#define ENABLE_UART_1_RX_INT()      (AT91C_BASE_US1->US_IER = AT91C_US_RXRDY)

/* Enables the TX interrupt of PAL_UART1 */
#define ENABLE_UART_1_TX_INT()      (AT91C_BASE_US1->US_IER = AT91C_US_TXRDY)

/* Disables the RX interrupt of PAL_UART1 */
#define DISABLE_UART_1_RX_INT()     (AT91C_BASE_US1->US_IDR = AT91C_US_RXRDY)

/* Disables the TX interrupt of PAL_UART1 */
#define DISABLE_UART_1_TX_INT()     (AT91C_BASE_US1->US_IDR = AT91C_US_TXRDY)

/* Writes data in the UART data register */
#define WRITE_TO_UART_1(tx_data)   do {                             \
    /*                                                              \
     * Data is written to tranmsit register only if there are is    \
     * no ongoing data transmission.                                \
     */                                                             \
    while ((AT91C_BASE_US1->US_CSR & AT91C_US_TXEMPTY) == 0);       \
    AT91C_BASE_US1->US_THR = tx_data;                               \
} while (0);

/* Writes data in the UART data register */
#define READ_FROM_UART_1(rx_data)   do {                            \
    /*                                                              \
     * Reading  from receive register is held until there is some   \
     * data received in the receive register.                       \
     */                                                             \
    while ((AT91C_BASE_US1->US_CSR & AT91C_US_RXRDY) == 0);         \
    rx_data = AT91C_BASE_US1->US_RHR;                               \
} while (0);

#endif  /* PAL_UART1 */


/* === Prototypes =========================================================== */
#ifdef __cplusplus
extern "C" {
#endif

void sio_uart_0_init(uint32_t baud_rate);
void sio_uart_1_init(uint32_t baud_rate);
uint8_t sio_uart_0_rx(uint8_t *data, uint8_t max_length);
uint8_t sio_uart_1_rx(uint8_t *data, uint8_t max_length);
uint8_t sio_uart_0_tx(uint8_t *data, uint8_t length);
uint8_t sio_uart_1_tx(uint8_t *data, uint8_t length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* #if (defined PAL_UART0)  */

#endif /* PAL_UART_H */
/* EOF */
