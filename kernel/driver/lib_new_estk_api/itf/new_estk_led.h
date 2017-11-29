/*
 * new_estk_led.h
 *
 *  Created on: 2015. 7. 8.
 *      Author: jaegeun
 */

#ifndef APPLICATION_LIB_I2C_MASTER_ITF_NEW_ESTK_LED_H_
#define APPLICATION_LIB_I2C_MASTER_ITF_NEW_ESTK_LED_H_


#include <stdio.h>
#include <stdint.h>

#include "pio/pio.h"
#include "sam4e16e.h"

#define LED1		(PIO_PA0_IDX)
#define LED1_FLAG	(PIO_OUTPUT_1 | PIO_DEFAULT)
#define LED2		(PIO_PA1_IDX)
#define LED2_FLAG	(PIO_OUTPUT_1 | PIO_DEFAULT)
#define LED3		(PIO_PA2_IDX)
#define LED3_FLAG	(PIO_OUTPUT_1 | PIO_DEFAULT)

#define LED_BLINK_DELAY_INTERVAL	50

void led_init(void);
void led_off(uint8_t led_port);
void led_on(uint8_t led_port);
void led_toggle(uint8_t led_port);
void led_blink(uint8_t led_port);

#endif /* APPLICATION_LIB_I2C_MASTER_ITF_NEW_ESTK_LED_H_ */
