/**
 * @file pal_mcu_generic.h
 *
 * @brief PAL configuration generic for ATmega128RFA1
 *
 * This header file contains generic configuration parameters for ATmega128RFA1.
 *
 * $Id: pal_mcu_generic.h,v 1.2 2010-08-01 09:06:26 slb Exp $
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
#ifndef PAL_MCU_GENERIC_H
#define PAL_MCU_GENERIC_H

#include "../../../../../ubiconfig.h"

/* === Includes =============================================================*/

#include "../../../Inc/pal_types.h"

#if (PAL_TYPE == ATMEGA128RFA1)

/*
 * This header file is required since pal_trx_frame_read() and
 * pal_trx_frame_write() depend on memcpy()
 */
#include <string.h>

/* === Types ================================================================*/

/** Enumerations used to idenfify ports
 * Note: ports A and C are not present on the ATmega128RFA1.
 */

typedef enum port_type_tag
{
    PORT_B,
    PORT_D,
    PORT_E,
    PORT_F,
    PORT_G
} port_type_t;

/** Enumerations used to idenfify port directions
 * Note: ports A and C are not present on the ATmega128RFA1.
 */
typedef enum port_pin_direction_type_tag
{
    DDR_B,
    DDR_D,
    DDR_E,
    DDR_F,
    DDR_G
} port_pin_direction_type_t;

/* === Externals ============================================================*/


/* === Macros ===============================================================*/

/*
 * Set TRX GPIO pins.
 */
#define RST_HIGH()                      (TRXPR |= _BV(TRXRST))  /**< Set Reset Bit. */
#define RST_LOW()                       (TRXPR &= ~_BV(TRXRST)) /**< Clear Reset Bit. */
#define SLP_TR_HIGH()                   (TRXPR |= _BV(SLPTR))   /**< Set Sleep/TR Bit. */
#define SLP_TR_LOW()                    (TRXPR &= ~_BV(SLPTR))  /**< Clear Sleep/TR Bit. */


/*
 * IRQ macros for ATmega128RFA1
 */
/** Mapping of Spare IRQ 0 for devices with multiple irqs to PLL Lock IRQ */
#define RFA1_PLL_LOCK_IRQ_HDLR_IDX     TRX_ALTERNATE_0_IRQ_HDLR_IDX
/** Mapping of Spare IRQ 1 for devices with multiple irqs to PLL Unlock IRQ */
#define RFA1_PLL_UNLOCK_IRQ_HDLR_IDX   TRX_ALTERNATE_1_IRQ_HDLR_IDX
/** Mapping of Timestamp IRQ to Rx Start IRQ IRQ */
#define RFA1_RX_START_IRQ_HDLR_IDX     TRX_TSTAMP_IRQ_HDLR_IDX
/** Mapping of Spare IRQ 2 for devices with multiple irqs to Rx End IRQ */
#define RFA1_RX_END_IRQ_HDLR_IDX       TRX_ALTERNATE_2_IRQ_HDLR_IDX
/** Mapping of Spare IRQ 3 for devices with multiple irqs to CCA/ED Measurement Done IRQ */
#define RFA1_CCA_ED_DONE_IRQ_HDLR_IDX  TRX_ALTERNATE_3_IRQ_HDLR_IDX
/** Mapping of Spare IRQ 0 for devices with multiple irqs to TX End IRQ */
#define RFA1_TX_END_IRQ_HDLR_IDX       TRX_MAIN_IRQ_HDLR_IDX
/** Mapping of Spare IRQ 4 for devices with multiple irqs to Address Match IRQ */
#define RFA1_AMI_IRQ_HDLR_IDX          TRX_ALTERNATE_4_IRQ_HDLR_IDX
/** Mapping of Spare IRQ 5 for devices with multiple irqs to Battery Low IRQ */
#define RFA1_BAT_LOW_IRQ_HDLR_IDX      TRX_ALTERNATE_5_IRQ_HDLR_IDX
/** Mapping of Spare IRQ 6 for devices with multiple irqs to Awake IRQ */
#define RFA1_AWAKE_IRQ_HDLR_IDX        TRX_ALTERNATE_6_IRQ_HDLR_IDX
/** Mapping of Spare IRQ 7 for devices with multiple irqs to AES Ready IRQ */
#define RFA1_AES_READY_IRQ_HDLR_IDX    TRX_ALTERNATE_7_IRQ_HDLR_IDX



/** Number of used TRX IRQs in this implementation. */
#define NO_OF_TRX_IRQS                 10

/** Enables the transceiver interrupts. */
#define ENABLE_TRX_IRQ(trx_irq_num)     do {                \
    if (trx_irq_num == RFA1_RX_START_IRQ_HDLR_IDX)          \
    {                                                       \
        /* Enable RX TIME STAMP interrupt */                \
        IRQ_MASK |= _BV(RX_START_EN);                       \
    }                                                       \
    else if (trx_irq_num == RFA1_TX_END_IRQ_HDLR_IDX)       \
    {                                                       \
        /* Enable TRX TX_END interrupt */                   \
        IRQ_MASK |= _BV(TX_END_EN);                         \
    }                                                       \
    else if (trx_irq_num == RFA1_RX_END_IRQ_HDLR_IDX)       \
    {                                                       \
        /* Enable TRX RX_END interrupt */                   \
        IRQ_MASK |= _BV(RX_END_EN);                         \
    }                                                       \
    else if (trx_irq_num == RFA1_CCA_ED_DONE_IRQ_HDLR_IDX)  \
    {                                                       \
        /* Enable TRX CCA_ED_DONE interrupt */              \
        IRQ_MASK |= _BV(CCA_ED_DONE_EN);                    \
    }                                                       \
    else if (trx_irq_num == RFA1_AMI_IRQ_HDLR_IDX)          \
    {                                                       \
        /* Enable TRX CCA_ED_DONE interrupt */              \
        IRQ_MASK |= _BV(AMI_EN);                            \
    }                                                       \
    else if (trx_irq_num == RFA1_BAT_LOW_IRQ_HDLR_IDX)      \
    {                                                       \
         /* Enable TRX BAT LOW interrupt */                 \
         BATMON |= _BV(BAT_LOW_EN);                         \
    }                                                       \
    else if (trx_irq_num == RFA1_AWAKE_IRQ_HDLR_IDX)        \
    {                                                       \
        /* Enable TRX AWAKE interrupt */                    \
        IRQ_MASK |= _BV(AWAKE_EN);                          \
    }                                                       \
    else if (trx_irq_num == RFA1_PLL_LOCK_IRQ_HDLR_IDX)     \
    {                                                       \
        /* Enable TRX PLL LOCK interrupt */                 \
        IRQ_MASK |= _BV(PLL_LOCK_EN);                       \
    }                                                       \
    else if (trx_irq_num == RFA1_PLL_UNLOCK_IRQ_HDLR_IDX)   \
    {                                                       \
        /* Enable TRX PLL UNLOCK interrupt */               \
        IRQ_MASK |= _BV(PLL_UNLOCK_EN);                     \
    }                                                       \
    else if (trx_irq_num == RFA1_AES_READY_IRQ_HDLR_IDX)    \
    {                                                       \
        /* Enable AES interrupt */                          \
        AES_CTRL |= _BV(AES_IM);                            \
    }                                                       \
} while (0)
//#define ENABLE_TRX_IRQ(trx_irq_num)     do { IRQ_MASK |= _BV(TX_END_EN); }while(0)

/** Disables the transceiver interrupts. */
#define DISABLE_TRX_IRQ(trx_irq_num)     do {   			\
    if (trx_irq_num == RFA1_RX_START_IRQ_HDLR_IDX)          \
    {                                                       \
        /* Disable RX TIME STAMP interrupt */               \
        IRQ_MASK &= ~_BV(RX_START_EN);                      \
    }                                                       \
    else if (trx_irq_num == RFA1_TX_END_IRQ_HDLR_IDX)       \
    {                                                       \
        /* Disable TRX TX_END interrupt */                  \
        IRQ_MASK &= ~_BV(TX_END_EN);                        \
    }                                                       \
    else if (trx_irq_num == RFA1_RX_END_IRQ_HDLR_IDX)       \
    {                                                       \
        /* Disable TRX RX_END interrupt */                  \
        IRQ_MASK &= ~_BV(RX_END_EN);                        \
    }                                                       \
    else if (trx_irq_num == RFA1_CCA_ED_DONE_IRQ_HDLR_IDX)  \
    {                                                       \
        /* Disable TRX CCA_ED_DONE interrupt */             \
        IRQ_MASK &= ~_BV(CCA_ED_DONE_EN);                   \
    }                                                       \
    else if (trx_irq_num == RFA1_AMI_IRQ_HDLR_IDX)          \
    {                                                       \
        /* Disable TRX CCA_ED_DONE interrupt */             \
        IRQ_MASK &= ~_BV(AMI_EN);                           \
    }                                                       \
    else if (trx_irq_num == RFA1_BAT_LOW_IRQ_HDLR_IDX)      \
    {                                                       \
         /* Disable TRX BAT LOW interrupt */                \
         BATMON &= ~_BV(BAT_LOW_EN);                        \
    }                                                       \
    else if (trx_irq_num == RFA1_AWAKE_IRQ_HDLR_IDX)        \
    {                                                       \
        /* Disable TRX AWAKE interrupt */                   \
        IRQ_MASK &= ~_BV(AWAKE_EN);                         \
    }                                                       \
    else if (trx_irq_num == RFA1_PLL_LOCK_IRQ_HDLR_IDX)     \
    {                                                       \
        /* Disable TRX PLL LOCK interrupt */                \
        IRQ_MASK &= ~_BV(PLL_LOCK_EN);                      \
    }                                                       \
    else if (trx_irq_num == RFA1_PLL_UNLOCK_IRQ_HDLR_IDX)   \
    {                                                       \
        /* Disable TRX PLL UNLOCK interrupt */              \
        IRQ_MASK &= ~_BV(PLL_UNLOCK_EN);                    \
    }                                                       \
    else if (trx_irq_num == RFA1_AES_READY_IRQ_HDLR_IDX)    \
    {                                                       \
        /* Disable AES interrupt */                         \
        AES_CTRL &= ~_BV(AES_IM);                           \
    }                                                       \
} while (0)

/** Clears the transceiver interrupts. */
/*
 * Note: TRX AES RDY is cleared on read/write of
 * AES_STATE, which has to be done in the AES
 * application.
 */
#define CLEAR_TRX_IRQ(trx_irq_num)     do {                 \
    if (trx_irq_num == RFA1_RX_START_IRQ_HDLR_IDX)          \
    {                                                       \
        /* Clear RX TIME STAMP interrupt */                 \
        IRQ_STATUS = _BV(RX_START);                         \
    }                                                       \
    else if (trx_irq_num == RFA1_TX_END_IRQ_HDLR_IDX)       \
    {                                                       \
        /* Clear TRX TX_END interrupt */                    \
        IRQ_STATUS = _BV(TX_END);                           \
    }                                                       \
    else if (trx_irq_num == RFA1_RX_END_IRQ_HDLR_IDX)       \
    {                                                       \
        /* Clear TRX RX_END interrupt */                    \
        IRQ_STATUS = _BV(RX_END);                           \
    }                                                       \
    else if (trx_irq_num == RFA1_CCA_ED_DONE_IRQ_HDLR_IDX)  \
    {                                                       \
        /* Clear TRX CCA_ED_DONE interrupt */               \
        IRQ_STATUS = _BV(CCA_ED_DONE_EN);                   \
    }                                                       \
    else if (trx_irq_num == RFA1_AMI_IRQ_HDLR_IDX)          \
    {                                                       \
        /* Clear TRX CCA_ED_DONE interrupt */               \
        IRQ_STATUS = _BV(AMI);                              \
    }                                                       \
    else if (trx_irq_num == RFA1_BAT_LOW_IRQ_HDLR_IDX)      \
    {                                                       \
         /* Clear TRX BAT LOW interrupt */                  \
         BATMON = _BV(BAT_LOW);                             \
    }                                                       \
    else if (trx_irq_num == RFA1_AWAKE_IRQ_HDLR_IDX)        \
    {                                                       \
        /* Clear TRX AWAKE interrupt */                     \
        IRQ_STATUS = _BV(AWAKE);                            \
    }                                                       \
    else if (trx_irq_num == RFA1_PLL_LOCK_IRQ_HDLR_IDX)     \
    {                                                       \
        /* Clear TRX PLL LOCK interrupt */                  \
        IRQ_STATUS = _BV(PLL_LOCK);                         \
    }                                                       \
    else if (trx_irq_num == RFA1_PLL_UNLOCK_IRQ_HDLR_IDX)   \
    {                                                       \
        /* Clear TRX PLL UNLOCK interrupt */                \
        IRQ_STATUS = _BV(PLL_UNLOCK);                       \
    }                                                       \
} while (0)

/** Enables the global interrupts. */
#define ENABLE_GLOBAL_IRQ()             sei()

/** Disables the global interrupts. */
#define DISABLE_GLOBAL_IRQ()            cli()

/** This macro saves the global interrupt status. */
#define ENTER_CRITICAL_REGION()         {uint8_t sreg = SREG; cli()

/** This macro restores the global interrupt status. */
#define LEAVE_CRITICAL_REGION()         SREG = sreg;}


/**
 * @brief Writes data into a transceiver register
 *
 * This macro writes a value into transceiver register.
 *
 * @param addr Address of the trx register
 * @param data Data to be written to trx register
 *
 * @ingroup apiPalApi
 */
#define pal_trx_reg_write(addr, data) \
    (*(volatile uint8_t *)(addr)) = (data)

/**
 * @brief Reads current value from a transceiver register
 *
 * This macro reads the current value from a transceiver register.
 *
 * @param addr Specifies the address of the trx register
 * from which the data shall be read
 *
 * @return value of the register read
 * @ingroup apiPalApi
 */
#define pal_trx_reg_read(addr) \
    (*(volatile uint8_t *)(addr))

/**
 * @brief Reads frame buffer of the transceiver
 *
 * This macro reads the frame buffer of the transceiver.
 *
 * @param[out] data Pointer to the location to store frame
 * @param[in] length Number of bytes to be read from the frame
 * buffer.
 * @ingroup apiPalApi
 */
#define pal_trx_frame_read(data, length) \
    memcpy((data), (void *)&TRXFBST, (length))

/**
 * @brief Writes data into frame buffer of the transceiver
 *
 * This macro writes data into the frame buffer of the transceiver
 *
 * @param[in] data Pointer to data to be written into frame buffer
 * @param[in] length Number of bytes to be written into frame buffer
 * @ingroup apiPalApi
 */
#define pal_trx_frame_write(data, length) \
    memcpy((void *)&TRXFBST, (data), (length))

#ifndef DOXYGEN
#define _pal_trx_bit_read(addr, mask, pos) \
    (((*(volatile uint8_t *)(addr)) & (mask)) >> (pos))
#endif
/**
 * @brief Subregister read
 *
 * @param   arg Subregister
 *
 * @return  Value of the read subregister
 * @ingroup apiPalApi
 */
#define pal_trx_bit_read(arg) \
    _pal_trx_bit_read(arg)

#ifndef DOXYGEN
#define _pal_trx_bit_write(addr, mask, pos, val) do {   \
        (*(volatile uint8_t *)(addr)) =                 \
            ((*(volatile uint8_t *)(addr)) & ~(mask)) | \
            (((val) << (pos)) & (mask));                \
    } while (0)
#endif
/**
 * @brief Subregister write
 *
 * @param[in]   arg1  Subregister
 * @param[out]  val  Data, which is muxed into the register
 * @ingroup apiPalApi
 */
#define pal_trx_bit_write(arg1, val) \
    _pal_trx_bit_write(arg1, val)

/** The smallest timeout in microseconds */
#define MIN_TIMEOUT                     (0x80)

/** The largest timeout in microseconds */
#define MAX_TIMEOUT                     (0x7FFFFFFF)


/** Maximum numbers of software timers running at a time. */
#define MAX_NO_OF_TIMERS                (25)
#if (MAX_NO_OF_TIMERS > 255)
#error "MAX_NO_OF_TIMERS must not be larger than 255"
#endif

/*
 *  This macro is a place holder for delay function for high speed processors
 */
#if ((F_CPU == (16000000UL)) || (F_CPU == (15384600UL)))
#define PAL_WAIT_1_US()                 {nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); \
                                         nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); }
#elif (F_CPU == (8000000UL))
#define PAL_WAIT_1_US()                 {nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); }
#else
#error "Unsupported F_CPU value"
#endif




/* === Prototypes ===========================================================*/

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* (PAL_TYPE == ATMEGA128RFA1) */

#endif /* PAL_MCU_GENERIC_H */

/* EOF */
