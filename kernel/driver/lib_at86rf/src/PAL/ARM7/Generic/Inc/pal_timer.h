/**
 * @file pal_timer.h
 *
 * @brief PAL timer internal functions prototypes
 *
 * This header has the timer specific stuctures, macros and
 * internal functions.
 *
 * $Id: pal_timer.h,v 1.6 2012-02-13 01:31:11 essusige Exp $
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
#ifndef PAL_TIMER_H
#define PAL_TIMER_H

#include "../../../../../ubiconfig.h"

/* === Includes ============================================================= */


/* === Types ================================================================ */

/*
 * This defines the structure of the time type.
 */
typedef struct timer_info_tag
{
    /* Timeout in microseconds */
    uint32_t abs_exp_timer;

    /* Callback function to be executed on expiry of the timer */
    void * timer_cb;

    /* Parameter to be passed to the callback function of the expired timer */
    void *param_cb;

    /* Next timer which was started or has expired */
    uint_fast8_t next_timer_in_queue;
} timer_info_t;

/*
 * Type definition for callbacks for timer functions
 */
typedef void (*timer_expiry_cb_t)(void *);

/* === Externals ============================================================ */

extern volatile uint16_t sys_time;

/* === Macros ================================================================ */

/*
 * Value to indicate end of timer in the array or queue
 */
#define NO_TIMER                        (0xFF)

/*
 * Shift mask to obtain the 16-bit system time out of a 32-bit timeout
 */
#define SYS_TIME_SHIFT_MASK             (16)

/*
 * Mask to obtain the 16-bit H/W time out of a 32-bit timeout
 */
#define HW_TIME_MASK                    (0xFFFF)

/*
 * Mask to act on all the timer interrupts at once
 */
#define ALL_TIMER_INTERRUPTS_MASK       (0xFFFFFFFF)

/*
 * Value to be written in the TCCLKS field of the TC_CMR register, to
 * select the prescaler for the main clock which is the timer clock source
 */
#define TC_CLKS_MCK32                   (0x02 << 0)

/*
 * The timer channel 0's input capture pin which is connected to transceiver
 * interrupt
 */
#define TIMER_INPUT_CAPTURE_LINE        (PIO_PA0B_TIOA0)
// TC_CMR0_TCCLKS_TIMER_CLOCK3
/*
 * Period of one timer clock
 */
#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
#define CLOCK_PERIOD                    ((float)(1000000ULL * 32) / F_CPU)
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
#define TIMERTICK_TO_USEC(tick)			( (unsigned int) (tick) * 32 / ((F_CPU) / 1000000ULL) )
#define USEC_TO_TIMERTICK(usec)			( (unsigned int) (usec) * ((F_CPU) / 1000000ULL) / 32 )
#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */

/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

void timer_init(void);
void timer_init_non_generic(void);
void internal_timer_handler(void);
void timer_service(void);
void timer_ch0_irq_handler(void);
#ifdef ENABLE_HIGH_PRIO_TMR
void timer_ch1_irq_handler(void);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* PAL_TIMER_H */
/* EOF */
