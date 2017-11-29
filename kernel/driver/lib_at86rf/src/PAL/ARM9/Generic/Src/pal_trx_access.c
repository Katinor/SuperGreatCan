/**
 * @file pal_trx_access.c
 *
 * @brief Transceiver registers & Buffer accessing functions for
 *        AT91SAM 32bit SPI based MCUs.
 *
 * This file implements functions for reading and writing transceiver
 * registers and transceiver buffer for AT91SAM 32bit SPI based MCUs.
 *
 * $Id: pal_trx_access.c,v 1.4 2012-02-13 01:31:11 essusige Exp $
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

#if (ARM9 == PAL_GENERIC_TYPE)

/* === Includes ============================================================= */

#include <stdint.h>
#include <string.h>
#include "../../../Inc/pal.h"
#include "../../../../Include/return_val.h"

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
	char write_command[2];
	//    ENTER_CRITICAL_REGION();
	SS_LOW();
    /* Prepare the command byte */
    addr |= WRITE_ACCESS_COMMAND;
    write_command[0] = addr;
    write_command[1] = data;


	//   	temp2 = SPI_WriteBuffer(AT91C_BASE_SPI0, &write_command[0], 2);
    if (AT91C_BASE_SPI_USED->SPI_TCR == 0) {

    	AT91C_BASE_SPI_USED->SPI_TPR = (unsigned int) &write_command[0]/*&write_command[0]*/;
    	AT91C_BASE_SPI_USED->SPI_TCR = 2;
    	AT91C_BASE_SPI_USED->SPI_PTCR = AT91C_PDC_TXTEN;
    }
	    // Check if second bank is free
    else if (AT91C_BASE_SPI_USED->SPI_TNCR == 0) {
    	AT91C_BASE_SPI_USED->SPI_TNPR = (unsigned int) &write_command[0]/*&write_command[0]*/;
    	AT91C_BASE_SPI_USED->SPI_TNCR = 2;
    }

    while ((AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_TXEMPTY) == 0);
	SS_HIGH();
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
	char register_value[2];
	char write_command[2];
	char temp;

//    ENTER_CRITICAL_REGION();
	SS_LOW();
    /* Prepare the command byte */
    addr |= READ_ACCESS_COMMAND;
    write_command[0] = addr;
    write_command[1] = SPI_DUMMY_VALUE;

//   	SPI_ClearRDR(AT91C_BASE_SPI0);
    while ((AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_TXEMPTY) == 0);
    temp = AT91C_BASE_SPI0->SPI_RDR & 0xFFFF;
//   	temp2 = SPI_ReadBuffer(AT91C_BASE_SPI0, &register_value[0], 2);
    if (AT91C_BASE_SPI0->SPI_RCR == 0) {

    	AT91C_BASE_SPI0->SPI_RPR = (unsigned int) &register_value[0]/*&register_value[0]*/;
    	AT91C_BASE_SPI0->SPI_RCR = 2;
    	AT91C_BASE_SPI0->SPI_PTCR = AT91C_PDC_RXTEN;
     }
     // Check if second bank is free
     else if (AT91C_BASE_SPI0->SPI_RNCR == 0) {

    	 AT91C_BASE_SPI0->SPI_RNPR = (unsigned int) &register_value[0]/*&register_value[0]*/;
    	 AT91C_BASE_SPI0->SPI_RNCR = 2;
     }

//   	temp2 = SPI_WriteBuffer(AT91C_BASE_SPI0, &write_command[0], 2);
    if (AT91C_BASE_SPI0->SPI_TCR == 0) {

    	AT91C_BASE_SPI0->SPI_TPR = (unsigned int) &write_command[0]/*&write_command[0]*/;
    	AT91C_BASE_SPI0->SPI_TCR = 2;
    	AT91C_BASE_SPI0->SPI_PTCR = AT91C_PDC_TXTEN;
    }
    // Check if second bank is free
    else if (AT91C_BASE_SPI0->SPI_TNCR == 0) {

    	AT91C_BASE_SPI0->SPI_TNPR = (unsigned int) &write_command[0]/*&write_command[0]*/;
    	AT91C_BASE_SPI0->SPI_TNCR = 2;
    }

    while ((AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_TXEMPTY) == 0);
	SS_HIGH();
   	return register_value[1];

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
	char temp_command[length+1], temp_read[length+1];
	unsigned short i, temp;

    ENTER_CRITICAL_REGION();
    temp_command[0] = TRX_CMD_FR;
    for(i=1; i<length+1; i++)
    {
    	temp_command[i] = 0x00;
    }

    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    //Clear SPI Read Buffer
    while ((AT91C_BASE_SPI_USED->SPI_SR & AT91C_SPI_TXEMPTY) == 0);
    temp = AT91C_BASE_SPI_USED->SPI_RDR & 0xFFFF;

    //Set ReadBuffer
    AT91C_BASE_SPI_USED->SPI_RPR = (unsigned int) &temp_read[0]/*&temp_read[0]*/;
    AT91C_BASE_SPI_USED->SPI_RCR = length+1;
    AT91C_BASE_SPI_USED->SPI_PTCR = AT91C_PDC_RXTEN;
   	//Set WriteBuffer
    AT91C_BASE_SPI_USED->SPI_TPR = (unsigned int) &temp_command[0]/* &temp_command[0]*/;
    AT91C_BASE_SPI_USED->SPI_TCR = length+1;
    AT91C_BASE_SPI_USED->SPI_PTCR = AT91C_PDC_TXTEN;
   	//Waiting SPI_TX complete
    while ((AT91C_BASE_SPI_USED->SPI_SR & AT91C_SPI_TXEMPTY) == 0);

    /* Stop the SPI transaction by setting SEL high. */
    SS_HIGH();

    for(i=0; i<length; i++)
    {
    	data[i] = temp_read[i+1];
    }
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
    unsigned char temp_commnad[length+1];
    unsigned char i;
	temp_commnad[0] = TRX_CMD_FW;
	for(i=1; i<length+1; i++)
	{
		temp_commnad[i] = (unsigned char) data[i-1];
	}
    /* Start SPI transaction by pulling SEL low */
    SS_LOW();

    //Set WriteBuffer
	AT91C_BASE_SPI0->SPI_TPR = (unsigned int) &temp_commnad[0]/*&temp_commnad[0]*/;
	AT91C_BASE_SPI0->SPI_TCR = length+1;
	AT91C_BASE_SPI0->SPI_PTCR = AT91C_PDC_TXTEN;
	//Waiting SPI_TX complete
	while ((AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_TXEMPTY) == 0);
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

#endif /* (ARM9 == PAL_GENERIC_TYPE) */

/* EOF */
