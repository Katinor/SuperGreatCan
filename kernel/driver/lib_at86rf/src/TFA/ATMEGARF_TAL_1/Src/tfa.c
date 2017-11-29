/**
 * @file tfa.c
 *
 * @brief Implementation of Transceiver Feature Access (TFA) functionality.
 *
 * $Id: tfa.c,v 1.5 2010-08-01 16:46:45 slb Exp $
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

#if	(ATMEGARF_TAL_1 == TAL_TYPE)

#ifdef ENABLE_TFA

/* === INCLUDES ============================================================ */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../../../PAL/Inc/pal.h"
#include "../../../Include/return_val.h"
#if (HIGHEST_STACK_LAYER == TINY_TAL)
/* Utilization of TFA on top of Tiny-TAL */
#include "../../../TAL/ATMEGARF_TAL_1/Inc/tiny_atmegarf_tal_1.h"
#if (TAL_TYPE == ATMEGARF_TAL_1)
#include "../../../TAL/ATMEGARF_TAL_1/Inc/tiny_tal.h"
#endif
#else
/* Utilization of TFA on top of regular TAL */
#include "../../../TAL/Inc/tal.h"
#include "../../../Include/_tal_constants.h"
#include "../../../Include/_tal_internal.h"
#endif
#include "../../../Include/ieee_const.h"
#include "../../../TAL/ATMEGARF_TAL_1/Inc/atmega128rfa1.h"
#include "../../Inc/tfa.h"

/* === TYPES =============================================================== */


/* === MACROS ============================================================== */

/* Constant define for the ED scaling: register value at -35dBm */
#define CLIP_VALUE_REG                  (55)

/* === GLOBALS ============================================================= */

/**
 * TFA PIB attribute to reduce the Rx sensitivity.
 */
static int8_t tfa_pib_rx_sens;

/* === PROTOTYPES ========================================================== */

static void init_tfa_pib(void);
static void write_all_tfa_pibs_to_trx(void);

/* === IMPLEMENTATION ====================================================== */


/**
 * @brief Gets a TFA PIB attribute
 *
 * This function is called to retrieve the transceiver information base
 * attributes.
 *
 * @param[in] tfa_pib_attribute TAL infobase attribute ID
 * @param[out] value TFA infobase attribute value
 *
 * @return MAC_UNSUPPORTED_ATTRIBUTE if the TFA infobase attribute is not found
 *         MAC_SUCCESS otherwise
 */
retval_t tfa_pib_get(tfa_pib_t tfa_pib_attribute, void *value)
{
    switch (tfa_pib_attribute)
    {
        case TFA_PIB_RX_SENS:
            *(uint8_t *)value = tfa_pib_rx_sens;
            break;

        default:
            /* Invalid attribute id */
            return MAC_UNSUPPORTED_ATTRIBUTE;
    }

    return MAC_SUCCESS;
}


/**
 * @brief Sets a TFA PIB attribute
 *
 * This function is called to set the transceiver information base
 * attributes.
 *
 * @param[in] tfa_pib_attribute TFA infobase attribute ID
 * @param[in] value TFA infobase attribute value to be set
 *
 * @return MAC_UNSUPPORTED_ATTRIBUTE if the TFA info base attribute is not found
 *         TAL_BUSY if the TAL is not in TAL_IDLE state.
 *         MAC_SUCCESS if the attempt to set the PIB attribute was successful
 */
retval_t tfa_pib_set(tfa_pib_t tfa_pib_attribute, void *value)
{
    switch (tfa_pib_attribute)
    {
        case TFA_PIB_RX_SENS:
            {
                uint8_t reg_val;

                tfa_pib_rx_sens = *((int *)value);
                reg_val = ((tfa_pib_rx_sens - (RSSI_BASE_VAL)) / 3) + 1;
                pal_trx_bit_write(SR_RX_PDT_LEVEL, reg_val);
            }
            break;

        default:
            /* Invalid attribute id */
            return MAC_UNSUPPORTED_ATTRIBUTE;
    }

    return MAC_SUCCESS;
}


/**
 * @brief Initializes the TFA
 *
 * This function is called to initialize the TFA.
 *
 * @return MAC_SUCCESS if everything went correct;
 *         FAILURE otherwise
 */
retval_t tfa_init(void)
{
    init_tfa_pib();
    write_all_tfa_pibs_to_trx();

    return MAC_SUCCESS;
}


/**
 * @brief Reset the TFA
 *
 * This function is called to reset the TFA.
 *
 * @param set_default_pib Defines whether PIB values need to be set
 *                        to its default values
 */
void tfa_reset(bool set_default_pib)
{
    if (set_default_pib)
    {
        init_tfa_pib();
    }

    write_all_tfa_pibs_to_trx();
}


/**
 * @brief Perform a CCA
 *
 * This function performs a CCA request.
 *
 * @return phy_enum_t PHY_IDLE or PHY_BUSY
 */
phy_enum_t tfa_cca_perform(void)
{
    tal_trx_status_t trx_status;
    uint8_t cca_status;
    uint8_t cca_done;

    /* Ensure that trx is not in SLEEP for register access */
    do
    {
        trx_status = set_trx_state(CMD_TRX_OFF);
    } while (trx_status != TRX_OFF);

    /* no interest in receiving frames while doing CCA */
    pal_trx_bit_write(SR_RX_PDT_DIS, RX_DISABLE); // disable frame reception indication

    /* Set trx to rx mode. */
    do
    {
        trx_status = set_trx_state(CMD_RX_ON);
    } while (trx_status != RX_ON);

    /* Start CCA */
    pal_trx_bit_write(SR_CCA_REQUEST, CCA_START);

    /* wait until CCA is done */
    pal_timer_delay(TAL_CONVERT_SYMBOLS_TO_US(CCA_DURATION_SYM));
    do
    {
      /* poll until CCA is really done */
      cca_done = pal_trx_bit_read(SR_CCA_DONE);
    } while (cca_done != CCA_CALCULATION_DONE);

    set_trx_state(CMD_TRX_OFF);

    /* Check if channel was idle or busy. */
    if (pal_trx_bit_read(SR_CCA_STATUS) == CCA_STATUS_CHANNEL_IS_IDLE)
    {
        cca_status = PHY_IDLE;
    }
    else
    {
        cca_status = PHY_BUSY;
    }

    /* Enable frame reception again. */
    pal_trx_bit_write(SR_RX_PDT_DIS, RX_ENABLE);

    return (phy_enum_t)cca_status;
}


/**
 * @brief Perform a single ED measurement
 *
 * @return ed_value Result of the measurement (transceiver's register value)
 */
uint8_t tfa_ed_sample(void)
{
    trx_irq_reason_t trx_irq_cause;
    uint8_t ed_value;
    tal_trx_status_t trx_status;

    /* Make sure that receiver is switched on. */
    do
    {
        trx_status = set_trx_state(CMD_RX_ON);
    } while (trx_status != RX_ON);

    /*
     * Disable the transceiver interrupts to prevent frame reception
     * while performing ED scan.
     */
    pal_trx_bit_write(SR_RX_PDT_DIS, RX_DISABLE);

    /* Write dummy value to start measurement. */
    pal_trx_reg_write(RG_PHY_ED_LEVEL, 0xFF);

    /* Wait for ED measurement completion. */
    pal_timer_delay(TAL_CONVERT_SYMBOLS_TO_US(ED_SAMPLE_DURATION_SYM));
    do
    {
        trx_irq_cause = (trx_irq_reason_t)pal_trx_reg_read(RG_IRQ_STATUS);
    } while ((trx_irq_cause & TRX_IRQ_CCA_ED_READY) != TRX_IRQ_CCA_ED_READY);

    /* Read the ED Value. */
    ed_value = pal_trx_reg_read(RG_PHY_ED_LEVEL);

        /*
     * Scale ED result.
     * Clip values to 0xFF if > -35dBm
     */
    if (ed_value > CLIP_VALUE_REG)
    {
        ed_value = 0xFF;
    }
    else
    {
        ed_value = (uint8_t)(((uint16_t)ed_value * 0xFF) / CLIP_VALUE_REG);
    }

    /* Clear IRQ register */
    pal_trx_reg_read(RG_IRQ_STATUS);
    /* Enable reception agian */
    pal_trx_bit_write(SR_RX_PDT_DIS, RX_ENABLE);
    /* Switch receiver off again */
    set_trx_state(CMD_TRX_OFF);

    return ed_value;
}


/**
 * @brief Get the transceiver's supply voltage
 *
 * @return mv Milli Volt; 0 if below threshold, 0xFFFF if above threshold
 */
uint16_t tfa_get_batmon_voltage(void)
{
    tal_trx_status_t previous_trx_status;
    uint8_t vth_val;
    uint8_t i;
    uint16_t mv = 1;    // 1 used as indicator flag
    bool range;

    pal_trx_irq_disable(TRX_MAIN_IRQ_HDLR_IDX);

    previous_trx_status = tal_trx_status;
    if (tal_trx_status == TRX_SLEEP)
    {
        set_trx_state(CMD_TRX_OFF);
    }

    /* Check if supply voltage is within lower range */
    pal_trx_bit_write(SR_BATMON_HR, HIGH_RANGE_DISABLED);
    pal_trx_bit_write(SR_BATMON_VTH, 0x0F);
    pal_timer_delay(5); /* Wait until Batmon has been settled. */
    if (pal_trx_bit_read(SR_BATMON_OK) == BATMON_NOT_VALID)
    {
        /* Lower range */
        /* Check if supply voltage is below lower limit */
        pal_trx_bit_write(SR_BATMON_VTH, 0);
        pal_timer_delay(2); /* Wait until Batmon has been settled. */
        if (pal_trx_bit_read(SR_BATMON_OK) == BATMON_NOT_VALID)
        {
            /* below lower limit */
            mv = 0x0000;
        }
        range = LOW;
    }
    else
    {
        /* Higher range */
        pal_trx_bit_write(SR_BATMON_HR, HIGH_RANGE_ENABLED);
        /* Check if supply voltage is above upper limit */
        pal_trx_bit_write(SR_BATMON_VTH, 0x0F);
        pal_timer_delay(5); /* Wait until Batmon has been settled. */
        if (pal_trx_bit_read(SR_BATMON_OK) == BATMON_VALID)
        {
            /* above upper limit */
            mv = 0xFFFF;
        }
        range = HIGH;
    }

    /* Scan through the current range for the matching threshold. */
    if (mv == 1)
    {
        vth_val = 0x0F;
        for (i = 0; i < 16; i++)
        {
            pal_trx_bit_write(SR_BATMON_VTH, i);
            pal_timer_delay(2); /* Wait until Batmon has been settled. */
            if (pal_trx_bit_read(SR_BATMON_OK) == BATMON_NOT_VALID)
            {
                if (i > 0)
                {
                    vth_val = i - 1;
                }
                else
                {
                    vth_val = i;
                }
                break;
            }
        }

        if (range == HIGH)
        {
            mv = 2550 + (75 * vth_val);
        }
        else
        {
            mv = 1700 + (50 * vth_val);
        }
    }

    pal_trx_reg_read(RG_IRQ_STATUS);
    if (previous_trx_status == TRX_SLEEP)
    {
        set_trx_state(CMD_SLEEP);
    }

    pal_trx_irq_enable(TRX_MAIN_IRQ_HDLR_IDX);

    return mv;
}



/**
 * @brief Initialize the TFA PIB
 *
 * This function initializes the TFA information base attributes
 * to their default values.
 */
static void init_tfa_pib(void)
{
    tfa_pib_rx_sens = TFA_PIB_RX_SENS_DEF;
}


/**
 * @brief Write all shadow PIB variables to the transceiver
 *
 * This function writes all shadow PIB variables to the transceiver.
 * It is assumed that the radio does not sleep.
 */
static void write_all_tfa_pibs_to_trx(void)
{
    pal_trx_bit_write(SR_RX_PDT_LEVEL, tfa_pib_rx_sens);
}




#define NUM_SAMPLES 16


/**
 * @brief Get the temperature value from the integrated sensor
 *
 * @return temperature value in degree Celsius
 */
double tfa_get_temperature(void)
{
    float temp_result = 0;
    float offset = 0;
    double result;
    uint8_t i;
    uint16_t adc_value;

    // Enable ADC
    ADCSRA = (1 << ADEN);

    // Analog channel and gain selection
    /*
     * The MUX5 bit has to written first followed by a
     * write access to the MUX4:0 bits which triggers the update of the internal buffer.
     */
    ADCSRB = (1 << MUX5);
    // Select internal 1.6V reference voltage
    // Select temperature sensor
    ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << MUX3) | (1 << MUX0);

    for ( i = 0; i < NUM_SAMPLES; i++)
    {
        // ADC Control and Status Register A
        // ADC Enable
        // ADC Start Conversion
        // Clear ADIF
        // Prescaler = 32 (500kHz); assume 16 MHz main clock
#if (F_CPU == (16000000UL))
        ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (1 << ADPS2) | (1 <<ADPS0);
#else
#error "unsupported F_CPU"
#endif
        // Wait for conversion to be completed */
        while (1)
        {
            if (ADCSRA & (1 << ADIF))
            {
                break;
            }
        }

        adc_value = ADC;

        /* Averaging */
        temp_result += adc_value;
    }
    temp_result = temp_result / NUM_SAMPLES;

    // Disable ADC for channel change
    ADCSRA &= ~(1 << ADEN);

    // Get offset value
    ADCSRB = 0x00;
    ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << MUX3);

    for (i = 0; i < NUM_SAMPLES; i++)
    {
#if (F_CPU == (16000000UL))
        ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (1 << ADPS2) | (1 <<ADPS0);
#else
#error "unsupported F_CPU"
#endif
        // Wait for conversion to be completed */
        while (1)
        {
            if (ADCSRA & (1 << ADIF))
            {
                break;
            }
        }

        adc_value = ADC;
        /* Averaging */
        if (adc_value > 0x1FF)
        {
            offset -= 0x200 - adc_value;
        }
        else
        {
            offset += adc_value;
        }
    }
    offset = offset / NUM_SAMPLES;

    // Disable ADC
    ADCSRA &= ~(1 << ADEN);

    temp_result -= offset;
    result = ((double)temp_result * 1.13) - 272.8;

    return result;
}


/**
 * @brief Starts continuous transmission on current channel
 */
void tfa_continuous_tx_start(continuous_tx_mode_t tx_mode)
{
    uint8_t txcwdata[127];

    pal_trx_bit_write(SR_TX_AUTO_CRC_ON, TX_AUTO_CRC_DISABLE);
    pal_trx_reg_write(RG_TRX_STATE, CMD_TRX_OFF);
    pal_trx_reg_write(0x0176, 0x0F);     /*TST_CTRL_DIGI*/
    if (tx_mode == CW_MODE)
    {
        pal_trx_bit_write(SR_OQPSK_DATA_RATE, ALTRATE_2MBPS);
        pal_trx_reg_write(RG_RX_CTRL, 0xA7);
    }
    txcwdata[0] = 127;
    pal_trx_frame_write(txcwdata, 127);
    pal_trx_reg_write(RG_PART_NUM, 0x54);
    pal_trx_reg_write(RG_PART_NUM, 0x46);
    set_trx_state(CMD_PLL_ON);
    PAL_SLP_TR_HIGH();
    PAL_SLP_TR_LOW();
}


/**
 * @brief Stops continuous transmission
 */
void tfa_continuous_tx_stop(void)
{
    tal_reset(false);
}



#endif /* #ifdef ENABLE_TFA */

#endif	/* (ATMEGARF_TAL_1 == TAL_TYPE) */

/* EOF */

