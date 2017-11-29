/**
 * @file pal_trx_access.c
 *
 * @brief Transceiver registers & Buffer accessing functions for
 *        AT91SAM 32bit SPI based MCUs.
 *
 * This file implements functions for reading and writing transceiver
 * registers and transceiver buffer for AT91SAM 32bit SPI based MCUs.
 *
 * $Id: pal_trx_access.c,v 1.1 2012-02-13 01:31:11 essusige Exp $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#include "../../../../../ubiconfig.h"

#if (SAM3S == PAL_GENERIC_TYPE)

/* === Includes ============================================================= */
#include <stdint.h>
#include <string.h>
#include "../../../Inc/pal.h"
#include "../../../../Include/return_val.h"
#include "../../../../../../../../../kernel/porting/bsp_armcortexm_sam4eek/itf/sam4e16e.h"
#include "../../../../../../../../../kernel/porting/bsp_armcortexm_sam4eek/itf/component/spi.h"

/* === Macros =============================================================== */

/*
 * Write access command of the transceiver
 */
#define WRITE_ACCESS_COMMAND            (0xC0)

/*
 * Read access command to the tranceiver
 */
#define READ_ACCESS_COMMAND             (0x80)

/*
 * Frame write command of transceiver
 */
#define TRX_CMD_FW                      (0x60)

/*
 * Frame read command of transceiver
 */
#define TRX_CMD_FR                      (0x20)

/*
 * SRAM write command of transceiver
 */
#define TRX_CMD_SW                      (0x40)

/*
 * SRAM read command of transceiver
 */
#define TRX_CMD_SR                      (0x00)

/* === Prototypes =========================================================== */


/* === Implementation ======================================================= */

/**
 * @brief Writes data into a transceiver register
 *
 * This function writes a value into transceiver register.
 *
 * @param addr Address of the trx register
 * @param data Data to be written to trx register
 *
 */
void pal_trx_reg_write(uint8_t addr, uint8_t data)
{
    ENTER_CRITICAL_REGION();

    /* Prepare the command byte */
    addr |= WRITE_ACCESS_COMMAND;

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    /* Send the Read command byte */
    SPI_WRITE(addr);
//    spi_write( SPI, addr, spi_get_pcs( 0 ), ( SPI->SPI_SR ) & SPI_SR_TXEMPTY );

//    delay_us(1);
    /* Write the byte in the transceiver data register */
    SPI_WRITE(data);
//    spi_write( SPI, data, spi_get_pcs( 0 ), ( SPI->SPI_SR ) & SPI_SR_TXEMPTY );

//    delay_us(1);
    /* Stop the SPI transaction by setting SEL high */
    SS_HIGH();

    LEAVE_CRITICAL_REGION();
}



/**
 * @brief Reads current value from a transceiver register
 *
 * This function reads the current value from a transceiver register.
 *
 * @param addr Specifies the address of the trx register from which
 *             the data shall be read
 *
 * @return Value of the register read
 */
uint8_t pal_trx_reg_read(uint8_t addr)
{
    uint8_t register_value;
    uint8_t p_pcs;
    ENTER_CRITICAL_REGION();

    /* Prepare the command byte */
    addr |= READ_ACCESS_COMMAND;

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();



//    spi_write( SPI, addr, spi_get_pcs( 0 ), ( SPI->SPI_SR ) & SPI_SR_TXEMPTY );
    SPI_WRITE(addr);

    /*
     * Done to clear the RDRF bit in the SPI status register, which will be set
     * as a result of reception of some data from the transceiver as a result
     * of SPI write operation done above.
     */

//    spi_read( SPI, &register_value, &p_pcs );
    SPI_READ(register_value);

//    delay_us(1);
    /* Do dummy write for initiating SPI read */
//    spi_write( SPI, SPI_DUMMY_VALUE, spi_get_pcs( 0 ), ( SPI->SPI_SR ) & SPI_SR_TXEMPTY );
    SPI_WRITE(SPI_DUMMY_VALUE);
    /* Read the byte received */
//    delay_us(1);
    SPI_READ(register_value);
//    spi_read( SPI, &register_value, &p_pcs );
    /* Stop the SPI transaction by setting SEL high */

    SS_HIGH();

    LEAVE_CRITICAL_REGION();

    return register_value;
}



/**
 * @brief Reads frame buffer of the transceiver
 *
 * This function reads the frame buffer of the transceiver.
 *
 * @param[out] data Pointer to the location to store frame
 * @param[in] length Number of bytes to be read from the frame buffer.
 */
void pal_trx_frame_read(uint8_t *data, uint8_t length)
{
    uint8_t dummy_rx_data;

    ENTER_CRITICAL_REGION();

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    /* Send the command byte */
    SPI_WRITE(TRX_CMD_FR);

    /*
     * Done to clear the RDRF bit in the SPI status register, which will be set
     * as a result of reception of some data from the transceiver as a result
     * of SPI write operation done above.
     */
    SPI_READ(dummy_rx_data);

    /*
     * Done to avoid compiler warning about variable being not used after
     * setting.
     */
    dummy_rx_data = dummy_rx_data;
    /* Initiate DMA transfer for ARM7 (synchronous read and write). */

    /* Disable both read and write. */
    SPI->SPI_PTCR = SPI_PTCR_RXTDIS | SPI_PTCR_TXTDIS;

    /*
     * Comment from ARM example code:
     * "MOSI should hold high during read, or there will be wrong data
     * in received data."
     */
    memset(data, 0xFF,length);

    SPI->SPI_RPR = SPI->SPI_TPR = (uint32_t)data;
    SPI->SPI_RCR = SPI->SPI_TCR = length;

    /* Enable read and write. */
    SPI->SPI_PTCR = SPI_PTCR_RXTEN | SPI_PTCR_TXTEN;

    /* Wait for end of read; send counter should not matter. */
    while (SPI->SPI_RCR);

    /* Stop the SPI transaction by setting SEL high. */
    SS_HIGH();

    LEAVE_CRITICAL_REGION();
}



/**
 * @brief Writes data into frame buffer of the transceiver
 *
 * This function writes data into the frame buffer of the transceiver
 *
 * @param[in] data Pointer to data to be written into frame buffer
 * @param[in] length Number of bytes to be written into frame buffer
 */
void pal_trx_frame_write(uint8_t *data, uint8_t length)
{
    ENTER_CRITICAL_REGION();

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    /* Send the command byte */
    SPI_WRITE(TRX_CMD_FW);

    /* DMA transfer for ARM7 */
    SPI->SPI_PTCR = SPI_PTCR_TXTDIS;

    /* Start DMA. */
    SPI->SPI_TPR = (uint32_t)data;
    SPI->SPI_TCR = length;
    SPI->SPI_PTCR = PERIPH_PTCR_TXTEN;

    /* Wait for finishing the transfer. */
    while (!(SPI->SPI_SR & SPI_SR_TDRE) ||
           !(SPI->SPI_SR & SPI_SR_TXEMPTY));

    /* Stop the SPI transaction by setting SEL high. */
    SS_HIGH();

    LEAVE_CRITICAL_REGION();
}



/**
 * @brief Subregister read
 *
 * @param addr Offset of the register
 * @param mask Bit mask of the subregister
 * @param pos  Bit position of the subregister
 *
 * @return  value of the read bit(s)
 */
uint8_t pal_trx_bit_read(uint8_t addr, uint8_t mask, uint8_t pos)
{
    uint8_t ret;

    ret = pal_trx_reg_read(addr);
    ret &= mask;
    ret >>= pos;

    return ret;
}



/**
 * @brief Subregister write
 *
 * @param[in] reg_addr Offset of the register
 * @param[in] mask Bit mask of the subregister
 * @param[in] pos Bit position of the subregister
 * @param[out] new_value Data, which is muxed into the register
 */
void pal_trx_bit_write(uint8_t reg_addr, uint8_t mask, uint8_t pos, uint8_t new_value)
{
    uint8_t current_reg_value;

    current_reg_value = pal_trx_reg_read(reg_addr);
    current_reg_value &= (uint8_t)~(uint32_t)mask;  // Implicit casting required to avoid IAR Pa091.
    new_value <<= pos;
    new_value &= mask;
    new_value |= current_reg_value;

    pal_trx_reg_write(reg_addr, new_value);
}



#if defined(ENABLE_TRX_SRAM) || defined(DOXYGEN)
/**
 * @brief Writes data into SRAM of the transceiver
 *
 * This function writes data into the SRAM of the transceiver
 *
 * @param addr Start address in the SRAM for the write operation
 * @param data Pointer to the data to be written into SRAM
 * @param length Number of bytes to be written into SRAM
 */
void pal_trx_sram_write(uint8_t addr, uint8_t *data, uint8_t length)
{
    ENTER_CRITICAL_REGION();

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    /* Send the command byte */
    SPI_WRITE(TRX_CMD_SW);

    /* Send the address from which the write operation should start */
    SPI_WRITE(addr);

    do
    {
        /* Upload the user data to transceiver data register */
        SPI_WRITE(*data);
        data++;

    } while (--length > 0);

    /* Stop the SPI transaction by setting SEL high */
    SS_HIGH();

    LEAVE_CRITICAL_REGION();
}
#endif  /* #if defined(ENABLE_TRX_SRAM) || defined(DOXYGEN) */



#if defined(ENABLE_TRX_SRAM) || defined(DOXYGEN)
/**
 * @brief Reads data from SRAM of the transceiver
 *
 * This function reads from the SRAM of the transceiver
 *
 * @param[in] addr Start address in SRAM for read operation
 * @param[out] data Pointer to the location where data stored
 * @param[in] length Number of bytes to be read from SRAM
 */
void pal_trx_sram_read(uint8_t addr, uint8_t *data, uint8_t length)
{
    uint8_t dummy_rx_data;

    PAL_WAIT_500_NS();

    ENTER_CRITICAL_REGION();

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    /* Send the command byte */
    SPI_WRITE(TRX_CMD_SR);

    /*
     * Done to clear the RDRF bit in the SPI status register, which will be set
     * as a result of reception of some data from the transceiver as a result
     * of SPI write operation done above.
     */
    SPI_READ(dummy_rx_data);

    /* Send the address from which the read operation should start */
    SPI_WRITE(addr);

    /*
     * Done to clear the RDRF bit in the SPI status register, which will be set
     * as a result of reception of some data from the transceiver as a result
     * of SPI write operation done above.
     */
    SPI_READ(dummy_rx_data);

    /*
     * Done to avoid compiler warning about variable being not used after
     * setting.
     */
    dummy_rx_data = dummy_rx_data;

    do
    {
        /* Do dummy write for initiating SPI read */
        SPI_WRITE(SPI_DUMMY_VALUE);

        /* Upload the received byte in the user provided location */
        SPI_READ(*data);
        data++;

    } while (--length > 0);

    SS_HIGH();

    LEAVE_CRITICAL_REGION();
}
#endif  /* #if defined(ENABLE_TRX_SRAM) || defined(DOXYGEN) */

#endif /* (ARM7 == PAL_GENERIC_TYPE) */

void delay_us(unsigned int time_us){
    unsigned int i;
	/* user_defined busywaiting function */
    // 4*(11 cycles) + 4 cycles = 48 cycles = 1 us for 48MHz
    for(i = 0; i < time_us; i++)			// 4 cycle
    {
    	//for(j = 0; j < 4; j++)				// 4 cyclec
    	//{
    		__NOP();__NOP();
    		__NOP();__NOP();
    		__NOP();__NOP();
    		__NOP();
    	//}
    }
}

/* EOF */
