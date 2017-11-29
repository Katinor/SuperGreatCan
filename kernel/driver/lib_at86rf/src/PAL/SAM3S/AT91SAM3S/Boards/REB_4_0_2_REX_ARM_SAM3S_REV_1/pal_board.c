/**
 * @file pal_board.c
 *
 * @brief PAL board specific functionality
 *
 * This file implements PAL board specific functionality.
 *
 * $Id: pal_board.c,v 1.3 2012-03-21 11:22:24 essusige Exp $
 *
 *  @author
 *      Atmel Corporation: http://www.atmel.com
 *      Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#include "../../../../../../ubiconfig.h"

#if (SAM3S == PAL_GENERIC_TYPE) && (AT91SAM3S == PAL_TYPE) && (REB_4_0_2_REX_ARM_SAM3S_REV_1 == BOARD_TYPE)

/* === Includes ============================================================ */

#include <stdbool.h>
#include <stdlib.h>
#include "../../../../Inc/pal.h"
#include "../pal_boardtypes.h"
#include "../../../../../Include/_pal_config.h"
/*
 * 'sys_time' is a entity of timer module which is given to other modules
 * through the interface file below. 'sys_time' is required to obtain the
 * frame timestamp
 * 15.07.24 레지스터 수정
 */
#include "../../../Generic/Inc/pal_timer.h"

#if (BOARD_TYPE == REB_4_0_2_REX_ARM_SAM3S_REV_1)

//#include "../../../../../../../lib_sam3slib/src/drivers/pio/pio.h"

/* === Globals ============================================================== */


/* === Prototypes =========================================================== */

static void initialize_timer_channel(uint8_t timer_clk_src_prescaler,
                                     uint8_t timer_id);

/* === Implementation ======================================================= */

/**
 * @brief Initializes the transceiver interface
 *
 * This function initializes the transceiver interface.
 * This board uses SPI1.
 */
void trx_interface_init(void)
{
	 unsigned int abcdsr;
    /*
     * The PIO control is disabled for the SPI pins MISO, MOSI, SCK and the
     * transceiver interrupt pin and the SPI module control for these pins
     * is enabled.
     *
     * Please note that these SPI pins from SPI1 require setting of
     * Peripheral B.
     */
	 PMC->PMC_PCER0 = _BV(ID_PIOA | ID_PIOB | ID_PIOC);
#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
    PIOA->PIO_BSR = (MISO | MOSI | SCK);
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */

    PIOA->PIO_IDR = (MISO | MOSI | SCK);

    // configure the PIO Peripheral A
    abcdsr = PIOA->PIO_ABCDSR[0];
    PIOA->PIO_ABCDSR[0] &= ((~(MISO | MOSI | SCK))& abcdsr);
    abcdsr = PIOA->PIO_ABCDSR[1];
    PIOA->PIO_ABCDSR[1] &= ((~(MISO | MOSI | SCK))& abcdsr);

    //    PIOA->PIO_ASR = (MISO | MOSI | SCK); // original code

#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
    PIOA->PIO_PDR = (MISO | MOSI | SCK);

    PMC->PMC_PCER0 = _BV(ID_PIOA);
//    PIOA->PIO_ASR = TRX_INTERRUPT_PIN;  // original code

    abcdsr = PIOA->PIO_ABCDSR[0];
    PIOA->PIO_ABCDSR[0] &= ((~(TRX_INTERRUPT_PIN))& abcdsr);
    abcdsr = PIOA->PIO_ABCDSR[1];
    PIOA->PIO_ABCDSR[1] &= ((~(TRX_INTERRUPT_PIN))& abcdsr);
    PIOA->PIO_PDR = TRX_INTERRUPT_PIN; //  original code

    PIOA->PIO_IDR = TRX_INTERRUPT_PIN;
    PIOA->PIO_PUDR = TRX_INTERRUPT_PIN;
    PIOA->PIO_IFDR = TRX_INTERRUPT_PIN;
    PIOA->PIO_ODR = TRX_INTERRUPT_PIN;	//Output disable, PIO Interrupt를 쓰므로 Input으로 설정
    PIOA->PIO_PER = TRX_INTERRUPT_PIN;

    /*
     * Select line will be used as a GPIO. The controller recognizes 1 cycle
     * of SPI transaction as 8 bit, hence deactivates the chip select after 1
     * cycle. But the transceiver needs the chip select to be active for two
     * cycles (In one cycle the transceiver gets to know about the address of
     * register or memory location and the kind of operation to be performed
     * on that memory location. And in the second cycle its performs the
     * specified operation). To achieve this, the chip select line will be
     * manually pulled low and high (after the transaction). Hence the SEL line
     * is configured as PIO and the SPI control of SEL is disabled.
     */
    /* Set SEL as output pin. */
//    PIOB->PIO_OER = SEL;
//    PIOB->PIO_PER = SEL;

    /*
     * Used peripheral interface is SPI1.
     * The clock to the utilized SPI 1 peripheral is enabled.
     */
#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
    PMC->PMC_PCER0 = _BV(AT91C_ID_SPI1);
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
    PMC->PMC_PCER0 = _BV(ID_SPI);
#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */

    /* The controller is configured to be master. */
    SPI->SPI_CR = SPI_CR_SPIDIS;
    /* Execute a software reset of the SPI twice */
    SPI->SPI_CR = SPI_CR_SWRST;
    SPI->SPI_CR = SPI_CR_SWRST;

    SPI->SPI_MR = (SPI_MR_MSTR | SPI_MR_MODFDIS | (SPI_MR_PCS_Msk & SS_ENABLE));
//    SPI->SPI_PTCR = (AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS);
//    SPI->SPI_CR = SPI_SPIEN;
//    PMC->PMC_PCDR0 = _BV(ID_SPI);
    /*
     * SPI mode 0 (clock polarity = 0 and clock phase = 1) is selected. The
     * transaction width is set to 8 bits. The SCBR register of the SPI module
     * is written with the divider required for generation of the baud rate. It
     * is calculated as follows. Baud rate = MCK / SPI_BR_DIVIDER.
     */
    SPI->SPI_CSR[2] = (SPI_CSR_NCPHA |
                            (SPI_CSR_BITS_Msk & SPI_CSR_BITS_8_BIT) |
                            (SPI_CSR_SCBR_Msk & (SPI_BR_DIVIDER << SCBR_FIELD_POS_IN_CSR_REG)));

    /* The SPI is enabled. */
    SPI->SPI_CR = SPI_CR_SPIEN;
}



/**
 * @brief Provides timestamp of the last received frame
 *
 * This function provides the timestamp (in microseconds)
 * of the last received frame.
 *
 * @param[out] Timestamp in microseconds
 */
void pal_trx_read_timestamp(uint32_t *timestamp)
{
    /*
     * Everytime a transceiver interrupt is triggred, input capture register of
     * the timer channel 0 is latched with the timer counter value. The
     * 'sys_time' is concatenated to the value of capture register to generate
     * the time stamp of the received frame.
     *      'sys_time'    'Capture register'
     *  |-----------------|-----------------| => 32 bit timestamp
     *        16 bits           16 bits
     */
    *timestamp = (uint32_t)sys_time << (uint32_t)16;
    *timestamp |= TIME_STAMP_REGISTER;

    /* The timeout is pre scaled accroding to the clock period. */
#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
    *timestamp = (uint32_t)(*timestamp / CLOCK_PERIOD);
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
    *timestamp = (uint32_t) TIMERTICK_TO_USEC(*timestamp);
#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
}



/**
 * @brief Calibrates the internal RC oscillator
 *
 * This function calibrates the internal RC oscillator.
 *
 * @return True
 */
bool pal_calibrate_rc_osc(void)
{
    return (true);
}



/**
 * @brief Initializes the GPIO pins
 *
 * This function is used to initialize the port pins used to connect
 * the microcontroller to transceiver.
 */
void gpio_init(void)
{
    /*
     * The clock to PIO A and PIO B are enabled. This is necessary, as only
     * when the clock is provided the PIO starts functioning.
     */
	PMC->PMC_PCER0 = _BV(ID_PIOA | ID_PIOB);

    /* The following pins are output pins.  */
    PIOA->PIO_OER = RST;
    PIOA->PIO_PER = RST;
    PIOA->PIO_OER = SLP_TR;
    PIOA->PIO_PER = SLP_TR;
    PIOA->PIO_OER = SEL;
    PIOA->PIO_PER = SEL;

}



/*
 * This function is called by timer_init() to perform the non-generic portion
 * of the initialization of the timer module.
 */
void timer_init_non_generic(void)
{
	unsigned int abcdsr;
    initialize_timer_channel(TC_CLKS_MCK32, ID_TC0);

    /* The clock for timer channel 0 is enabled here. */
    PMC->PMC_PCER0 = _BV(ID_TC0);

    /* The clock is enabled at the timer level. */
    TC0->TC_CHANNEL->TC_CCR = TC_CCR_CLKEN;

#ifdef ENABLE_HIGH_PRIO_TMR
    initialize_timer_channel(TC_CLKS_MCK32, AT91C_ID_TC0);
#endif  /* ENABLE_HIGH_PRIO_TMR */

    /*
     * The clock for timer channel 1 will be enabled only when a high
     * priority timer is requested to be started, as until that time, the timer
     * channel 1 is not used at all. Also the overall power consumption
     * can be reduced by clocking a perpheral only when required.
     */

    /* The AIC is set up for the timer interrupts. */
//    NVIC_CONFIGURE(ID_TC0,
//                  AT91C_AIC_SRCTYPE_POSITIVE_EDGE,
//                  timer_ch0_irq_handler);

   	do {
    	    /* The interrupts for specified peripheral are first disabled. */
    		intr_disable(ID_TC0);
    	    /*
    	     * The edge at which the interrupt is to be triggered and handler for
    	     * the same is configured.
    	     */
    		PIOA->PIO_AIMER  = PIO_IT_AIME;
    		PIOA->PIO_REHLSR = PIO_IT_RE_OR_HL;
    		PIOA->PIO_ESR    = PIO_IT_EDGE;

    		intr_connectisr(ID_TC0, (void *)timer_ch0_irq_handler, 0x40, 0);
    	    /* Pending interrupts if any for the peripheral are cleared. */

    		intr_ClearPending(ID_TC0);

    	} while (0);


#ifdef ENABLE_HIGH_PRIO_TMR
    NVIC_CONFIGURE(AT91C_ID_TC0,
                  AT91C_AIC_SRCTYPE_POSITIVE_EDGE,
                  timer_ch1_irq_handler);
#endif  /* ENABLE_HIGH_PRIO_TMR */

    /*
     * The timer overflow interrupts for the timer channel 0 is enabled, as
     * channel 0 is used for regular timer implementation.
     */
    TC0->TC_CHANNEL->TC_IER = TC_IER_COVFS;

    /* The timer channel 0 interrupt in AIC is enabled. */
    intr_enable(ID_TC0);

    /*
     * The PB23 pin of PIOB is configured as TIOA line of the timer channel 0.
     * This line is connected to the transceiver interrupt. Whenever an
     * interrupt occurs, the TC_RA register will latch the TC_CV value, which
     * is useful in timestamp generation.
     *
     * The pin requires setting of Peripheral A.
     */

//    PIOB->PIO_ASR = TIMER_INPUT_CAPTURE_LINE; 	//original code

    abcdsr = PIOB->PIO_ABCDSR[0];
    PIOB->PIO_ABCDSR[0] &= ((~(TIMER_INPUT_CAPTURE_LINE))& abcdsr);
    abcdsr = PIOB->PIO_ABCDSR[1];
    PIOB->PIO_ABCDSR[1] &= ((~(TIMER_INPUT_CAPTURE_LINE))& abcdsr);

    PIOB->PIO_PDR = TIMER_INPUT_CAPTURE_LINE;

    /*
     * Settings are done to load the RA and RB with counter value at rising and
     * falling edge of the TIOA line (transceiver interrupt) respectively. This
     * is useful in frame timestamp generation.
     *
     * Although RB is not used, RB is also configured, as RA can be loaded only
     * if it is not loaded or RB is loaded. This configuration will load RA on
     * the rising edge and RB on the falling edge of the transceiver interrupt
     * respectively, making RA ready for the loading the counter value on next
     * interrupt.
     */
    TC0->TC_CHANNEL->TC_CMR |= ((TC_CMR_LDRA_Msk & TC_CMR_LDRA_RISING) |
                               (TC_CMR_LDRB_Msk & TC_CMR_LDRB_FALLING));

    /* Only the timer channel 0 is triggered. */
    TC0->TC_CHANNEL->TC_CCR = TC_CCR_SWTRG;
}



/**
 * @brief Configures the timer channel
 *
 * This function configures the timer channel. It disables the clock to the
 * timer channel at the timer level, disables all the timer interrupts and
 * programs the prescaler for timer clock.
 *
 * @param timer_clk_src_prescaler Value to be written in the TCCLKS field
 *                                of the TC_CMR register, to select the
 *                                prescaler for the main clock which is
 *                                the timer clock source
 * @param timer_id ID of the timer channel
 */
static void initialize_timer_channel(uint8_t timer_clk_src_prescaler,
                                     uint8_t timer_id)
{
    uint32_t tc_status;
//    AT91PS_TC tc_pt;

    if (ID_TC0 == timer_id)
    {
		/* The clock and the interrupts of the timer channel are disabled. */
    	TC0->TC_CHANNEL->TC_CCR = TC_CCR_CLKDIS;
    	TC0->TC_CHANNEL->TC_IDR = ALL_TIMER_INTERRUPTS_MASK;

		/* The status register is read to clear any pending interrupt. */
		tc_status = TC0->TC_CHANNEL->TC_SR;

		/*
		 * Done to avoid compiler warning about variable being not used after
		 * setting.
		 */
		tc_status = tc_status;

		/* The prescaler for the timer clock source (main clock) is selected. */
		TC0->TC_CHANNEL->TC_CMR = timer_clk_src_prescaler;

	#ifdef ENABLE_HIGH_PRIO_TMR
		else
		{
			/* The clock and the interrupts of the timer channel are disabled. */
			TC1->TC_CHANNEL->TC_CCR = AT91C_TC_CLKDIS;
			TC1->TC_CHANNEL->TC_IDR = ALL_TIMER_INTERRUPTS_MASK;

			/* The status register is read to clear any pending interrupt. */
			tc_status = TC1->TC_CHANNEL->TC_SR;

			/*
			 * Done to avoid compiler warning about variable being not used after
			 * setting.
			 */
			tc_status = tc_status;

			/* The prescaler for the timer clock source (main clock) is selected. */
			TC1->TC_CHANNEL->TC_CMR = timer_clk_src_prescaler;
		}
	#endif  /* ENABLE_HIGH_PRIO_TMR */
    }

}



/**
 * @brief Initialize LEDs
 */
void pal_led_init(void)
{
    /* Set LED pins to output. */
    PIOA->PIO_OER = LED_PINS;
    PIOA->PIO_SODR = LED_PINS;
}



/**
 * @brief Control LED status
 *
 * @param led_no LED ID
 * @param led_setting LED_ON, LED_OFF, LED_TOGGLE
 */
void pal_led(led_id_t led_no, led_action_t led_setting)
{
    uint32_t led_pin;

    switch (led_no)
    {
        case LED_0: led_pin = PIO_PA19; break;
        case LED_1: led_pin = PIO_PA20; break;
        default: return;
    }

    /* LEDs are low active. */
    if (led_setting == LED_ON)
    {
    	PIOA->PIO_CODR = led_pin;
    }
    else if (led_setting == LED_OFF)
    {
    	PIOA->PIO_SODR = led_pin;
    }
    else    /* LED_TOGGLE */
    {
        if (PIOA->PIO_ODSR & led_pin)
        {
        	PIOA->PIO_CODR = led_pin;
        }
        else
        {
        	PIOA->PIO_SODR = led_pin;
        }
    }
}


/**
 * @brief Initialize the 5-way joystick
 *
 * This funcitons initializes the 5-way Joystick on the AT91SAM7X-EK board.
 * For more information please check
 * AT91SAM7X-EK Evaluation Board for AT91SAM7X and AT91SAM7XC User Guide.
 */
void pal_button_init(void)
{
    PMC->PMC_PCER0 = 1 << ID_PIOA;

	PIOA->PIO_IDR =
	PIOA->PIO_PUER =
	PIOA->PIO_IFER =
	PIOA->PIO_ODR =
	PIOA->PIO_PER = LED_PINS;
}


/**
 * @brief Reads status of joystick
 *
 * This function reads the current status of the 5-way joystick
 * and detects whether the joystick has been moved or pushed.
 * Currently no further distinction is done, whether the button has been
 * been moved to a specific direction or pushed, only a boolean result that
 * some action has been done is reported back.
 *
 * @param button_no Dummy parameter
 *
 * @return Status of joystick action
 */

button_state_t pal_button_read(button_id_t button_no)
{
    /* Keep compiler happy. */
    button_no = button_no;

    if (~LED_PORT->PIO_PDSR & LED_PINS)
    {
        return BUTTON_PRESSED;
    }
    else
    {
        return BUTTON_OFF;
    }
}


#endif /* REB_4_0_2_REX_ARM_ESPS_REV_1 */

#endif /* (ARM7 == PAL_GENERIC_TYPE) && (AT91SAM7X256 == PAL_TYPE) && (REB_4_0_2_REX_ARM_ESPS_REV_1 == BOARD_TYPE) */

/* EOF */
