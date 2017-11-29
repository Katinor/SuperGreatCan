/*
 * lib_i2c_master.h
 *
 *  Created on: 2015. 4. 20.
 *      Author: geoyun
 */

#ifndef APPLICATION_LIB_I2C_MASTER_ITF_LIB_I2C_MASTER_H_
#define APPLICATION_LIB_I2C_MASTER_ITF_LIB_I2C_MASTER_H_


#include "sam4e.h"
#include "../itf/lib_i2c_master.h"
#include "twi.h"
#include "sysclk.h"

#define TRUE		0
#define FALSE		-1

#define	NOT_INIT		-3
#define	ALREADY_INIT	-2
#define	ERROR 	-1
#define UBI_I2C_SUCCESS              0
#define UBI_I2C_INVALID_ARGUMENT     1
#define UBI_I2C_ARBITRATION_LOST     2
#define UBI_I2C_NO_CHIP_FOUND        3
#define UBI_I2C_RECEIVE_OVERRUN      4
#define UBI_I2C_RECEIVE_NACK         5
#define UBI_I2C_SEND_OVERRUN         6
#define UBI_I2C_SEND_NACK            7
#define UBI_I2C_BUSY                 8
#define UBI_I2C_ERROR_TIMEOUT        9

#define	TWI_PORT_0		 0
#define	TWI_PORT_1		 1

/*
#define	NOT_INIT		-3
#define	ALREADY_INIT	-2
#define	ERROR 	-1
#define UBI_I2C_SUCCESS              0
#define UBI_I2C_INVALID_ARGUMENT     1
#define UBI_I2C_ARBITRATION_LOST     2
#define UBI_I2C_NO_CHIP_FOUND        3
#define UBI_I2C_RECEIVE_OVERRUN      4
#define UBI_I2C_RECEIVE_NACK         5
#define UBI_I2C_SEND_OVERRUN         6
#define UBI_I2C_SEND_NACK            7
#define UBI_I2C_BUSY                 8
#define UBI_I2C_ERROR_TIMEOUT        9
*/
int ubi_twim_init(uint8_t i2c_port, uint32_t pSpeed);

/*
#define	NOT_INIT		-3
#define	ALREADY_INIT	-2
#define	ERROR 	-1
#define UBI_I2C_SUCCESS              0
#define UBI_I2C_INVALID_ARGUMENT     1
#define UBI_I2C_ARBITRATION_LOST     2
#define UBI_I2C_NO_CHIP_FOUND        3
#define UBI_I2C_RECEIVE_OVERRUN      4
#define UBI_I2C_RECEIVE_NACK         5
#define UBI_I2C_SEND_OVERRUN         6
#define UBI_I2C_SEND_NACK            7
#define UBI_I2C_BUSY                 8
#define UBI_I2C_ERROR_TIMEOUT        9
*/
int ubi_twim_write_bytes(uint8_t i2c_port, uint8_t device_address, uint8_t register_address, uint8_t address_data_size, uint8_t *write_buffer, uint32_t plength);

/*
#define	NOT_INIT		-3
#define	ALREADY_INIT	-2
#define	ERROR 	-1
#define UBI_I2C_SUCCESS              0
#define UBI_I2C_INVALID_ARGUMENT     1
#define UBI_I2C_ARBITRATION_LOST     2
#define UBI_I2C_NO_CHIP_FOUND        3
#define UBI_I2C_RECEIVE_OVERRUN      4
#define UBI_I2C_RECEIVE_NACK         5
#define UBI_I2C_SEND_OVERRUN         6
#define UBI_I2C_SEND_NACK            7
#define UBI_I2C_BUSY                 8
#define UBI_I2C_ERROR_TIMEOUT        9
*/
int ubi_twim_read_bytes(uint8_t i2c_port, uint8_t device_address, uint8_t register_address, uint8_t address_data_size, uint8_t *write_buffer, uint32_t plength);

#endif /* APPLICATION_LIB_I2C_MASTER_ITF_LIB_I2C_MASTER_H_ */
