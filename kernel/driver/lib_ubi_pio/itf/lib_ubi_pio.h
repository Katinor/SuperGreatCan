/*
 * lib_ubi_pio.h
 *
 *  Created on: 2015. 4. 28.
 *      Author: geoyun
 */

#ifndef APPLICATION_LIB_I2C_MASTER_ITF_LIB_UBI_PIO_H_
#define APPLICATION_LIB_I2C_MASTER_ITF_LIB_UBI_PIO_H_


#include <stdio.h>
#include <stdint.h>
#include "pio/pio.h"

typedef struct {

    /// Bitmask indicating which pin(s) to configure.
	uint32_t mask;
    /// Pointer to the PIO controller which has the pin(s).
    Pio    *pio;
    /// Peripheral ID of the PIO controller which has the pin(s).
    uint32_t id;
    /// Pin type.
    pio_type_t type;
    /// Pin attribute.
    uint32_t attribute;
} Pin;

typedef struct{
	uint8_t port;
	uint8_t pin_num;
	uint32_t attr;
}PinUbi;

#define UBI_PIO_DEFAULT			PIO_DEFAULT
#define	UBI_PIO_PULL_UP			PIO_PULLUP
//#define UBI_PIO_PULL_DOWN		( 1 << 2 )
//#define UBI_PIO_PULL_UPNDOWN_MASK	(UBI_PIO_PULL_UP | UBI_PIO_PULL_DOWN)

#define	UBI_PIO_DEGLITCH		PIO_DEGLITCH
#define	UBI_PIO_OPENDRAIN		PIO_OPENDRAIN
#define UBI_PIO_DEBOUNCE		PIO_DEBOUNCE

#define	UBI_PIO_IT_AIME			PIO_IT_AIME
#define	UBI_PIO_IT_RE_OR_HL		PIO_IT_RE_OR_HL
#define UBI_PIO_IT_EDGE			PIO_IT_EDGE

#define UBI_PIO_IT_LOWLEVEL		PIO_IT_LOW_LEVEL
#define	UBI_PIO_IT_HIGHLEVEL	PIO_IT_HIGH_LEVEL
#define	UBI_PIO_IT_FALL_EDGE	PIO_IT_FALL_EDGE
#define	UBI_PIO_IT_RISE_EDGE	PIO_IT_RISE_EDGE
#define UBI_PIO_IT_BOTH_EDGE	( UBI_PIO_IT_FALL_EDGE | UBI_PIO_IT_RISE_EDGE)
//#define	UBI_PIO_IT_TRI_MASK		(UBI_PIO_IT_LOWLEVEL | UBI_PIO_IT_HIGHLEVEL | UBI_PIO_IT_FALL_EDGE | UBI_PIO_IT_RISE_EDGE)

#define	PUSH_BUTTON_DEBOUNCE_HZ	 10

#define	PORTA	0
#define	PORTB	1
#define	PORTC	2
#define	PORTD	3
#define	PORTE	4

#define	NOT_INIT		-3
#define	ALREADY_INIT	-2
#define	ERROR 	-1
#define	OK		0

int ubi_pio_interrupt_config(uint8_t pPort, uint8_t pPin, const uint32_t attr, void *pio_irq_handler);
int ubi_pio_interrupt_reconfig(uint8_t pPort, uint8_t pPin, const uint32_t attr);
int ubi_pio_input_config(uint8_t pPort, uint8_t pPin, const uint32_t attr);

#define UBI_PIO_INPUT		2
#define UBI_PIO_HIGH_LEVEL	1
#define	UBI_PIO_LOW_LEVEL	0
int ubi_pio_output_config(uint8_t pPort, uint8_t pPin, uint8_t level, uint8_t pullUPen);

int ubi_pio_setPin(uint8_t pPort, uint8_t pPin);
int ubi_pio_clearPin(uint8_t pPort, uint8_t pPin);
int ubi_pio_getPin(uint8_t pPort, uint8_t pPin, uint8_t default_level);
uint32_t ubi_pio_getPort(uint8_t pPort, uint8_t default_level);

#define	UBI_PORT	32
int ubi_pio_togglePin(uint8_t pPort, uint8_t pPin);

#endif /* APPLICATION_LIB_I2C_MASTER_ITF_LIB_UBI_PIO_H_ */
