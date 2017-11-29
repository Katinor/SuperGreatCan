/*
 * lib_i2c.h
 *
 *  Created on: 2015. 7. 8.
 *      Author: minsu
 */

#ifndef APPLICATION_LIB_I2C_ITF_LIB_I2C_H_
#define APPLICATION_LIB_I2C_ITF_LIB_I2C_H_


#include <stdio.h>
#include <stdint.h>

#define BOARD_MCKL               48000000

void lib_i2c_component_init( void );

void i2c_init(void);
int i2c_disable(int port);
int i2c_enable(int port);
int i2c_busy(int port);
int i2c_start_transaction(int port,
						  unsigned int address,
						  int internal_address,
						  int n_internal_address_bytes,
						  unsigned char *data,
						  unsigned int nbytes,
						  int write);
void i2c_timer_isr_entry(void);

#endif /* APPLICATION_LIB_I2C_ITF_LIB_I2C_H_ */
