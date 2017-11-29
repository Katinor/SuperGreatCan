/*
 * lib_ubi_pio.c
 *
 *  Created on: 2015. 4. 28.
 *      Author: geoyun
 */

#include "../itf/lib_ubi_pio.h"

#include <stdio.h>
#include <sam4e.h>

#include "ioport.h"
#include "pio.h"
#include "pio/pio.h"
#include "pio/pio_handler.h"

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"
#include "sysclk.h"

#define	DEBOUNCE_FILTER_FRQ	100

int ubi_pio_interrupt_config(uint8_t pPort, uint8_t pPin, const uint32_t attr,
		void *pio_irq_handler) {
	uint32_t result = 0;

	uint32_t bitmask = 0;

	IRQn_Type pio_IRQn_Type;
	Pio *pio_type;
	uint32_t mask = (1 << pPin);

	uint32_t ul_id;

	void *_intr_pio_handler;

	/* Configure PIO clock. */
	switch (pPort) {
	case PORTA:
		ul_id = ID_PIOA;
		pio_IRQn_Type = PIOA_IRQn;
		pio_type = PIOA;
		_intr_pio_handler = PIOA_Handler;
		break;

	case PORTB:
		ul_id = ID_PIOB;
		pio_IRQn_Type = PIOB_IRQn;
		pio_type = PIOB;
		_intr_pio_handler = PIOB_Handler;
		break;

	case PORTC:
		ul_id = ID_PIOC;
		pio_IRQn_Type = PIOC_IRQn;
		pio_type = PIOC;
		_intr_pio_handler = PIOC_Handler;
		break;

	case PORTD:
		ul_id = ID_PIOD;
		pio_IRQn_Type = PIOD_IRQn;
		pio_type = PIOD;
		_intr_pio_handler = PIOD_Handler;
		break;

	case PORTE:
		ul_id = ID_PIOE;
		pio_IRQn_Type = PIOE_IRQn;
		pio_type = PIOE;
		_intr_pio_handler = PIOE_Handler;
		break;

	default:
		break;
	}

	pmc_enable_periph_clk(ul_id);

	// interrupt init
	if (pio_irq_handler != NULL) {
		/* Adjust pio debounce filter parameters, uses 10 Hz filter. */
		pio_set_debounce_filter(pio_type, mask, DEBOUNCE_FILTER_FRQ);

		intr_connectisr(pio_IRQn_Type, _intr_pio_handler,
				intr_gethighestpriority(), 0);

		pio_handler_set(pio_type, ul_id, mask, attr, pio_irq_handler);
		NVIC_EnableIRQ(pio_IRQn_Type);

		/* Enable PIO line interrupts. */
		pio_enable_interrupt(pio_type, mask);

	}
}


int ubi_pio_interrupt_reconfig(uint8_t pPort, uint8_t pPin, const uint32_t attr)
{


	IRQn_Type pio_IRQn_Type;
	Pio *pio_type;
	uint32_t mask = (1 << pPin);

	uint32_t ul_id;

	/* Configure PIO clock. */
	switch (pPort) {
	case PORTA:
		ul_id = ID_PIOA;
		pio_IRQn_Type = PIOA_IRQn;
		pio_type = PIOA;
		break;

	case PORTB:
		ul_id = ID_PIOB;
		pio_IRQn_Type = PIOB_IRQn;
		pio_type = PIOB;
		break;

	case PORTC:
		ul_id = ID_PIOC;
		pio_IRQn_Type = PIOC_IRQn;
		pio_type = PIOC;
		break;

	case PORTD:
		ul_id = ID_PIOD;
		pio_IRQn_Type = PIOD_IRQn;
		pio_type = PIOD;
		break;

	case PORTE:
		ul_id = ID_PIOE;
		pio_IRQn_Type = PIOE_IRQn;
		pio_type = PIOE;
		break;

	default:
		break;
	}


	pio_configure_interrupt(pio_type, mask, attr);

	return 0;
}

int ubi_pio_input_config(uint8_t pPort, uint8_t pPin, const uint32_t pMode) {

	Pio *pio_type;
	uint32_t mask = (1 << pPin);
	uint32_t ul_id;

	switch (pPort) {
	case PORTA:
		ul_id = ID_PIOA;
		pio_type = PIOA;
		break;

	case PORTB:
		ul_id = ID_PIOB;
		pio_type = PIOB;
		break;

	case PORTC:
		ul_id = ID_PIOC;
		pio_type = PIOC;
		break;

	case PORTD:
		ul_id = ID_PIOD;
		pio_type = PIOD;
		break;

	case PORTE:
		ul_id = ID_PIOE;
		pio_type = PIOE;
		break;

	default:
		break;
	}

	pmc_enable_periph_clk(ul_id);

	pio_set_input(pio_type, mask, pMode);
}

int ubi_pio_output_config(uint8_t pPort, uint8_t pPin, uint8_t level, uint8_t pullUPen) {

	Pio *pio_type;
	uint32_t mask = (1 << pPin);
	uint32_t ul_id;

	switch (pPort) {
	case PORTA:
		ul_id = ID_PIOA;
		pio_type = PIOA;
		break;

	case PORTB:
		ul_id = ID_PIOB;
		pio_type = PIOB;
		break;

	case PORTC:
		ul_id = ID_PIOC;
		pio_type = PIOC;
		break;

	case PORTD:
		ul_id = ID_PIOD;
		pio_type = PIOD;
		break;

	case PORTE:
		ul_id = ID_PIOE;
		pio_type = PIOE;
		break;

	default:
		break;
	}

	pmc_enable_periph_clk(ul_id);
	pio_set_output(pio_type, mask, level, 0, pullUPen);
}

int ubi_pio_setPin(uint8_t pPort, uint8_t pPin) {
	Pio *pio_type;
	uint32_t mask = (1 << pPin);

	switch (pPort) {
	case PORTA:
		pio_type = PIOA;
		break;

	case PORTB:
		pio_type = PIOB;
		break;

	case PORTC:
		pio_type = PIOC;
		break;

	case PORTD:
		pio_type = PIOD;
		break;

	case PORTE:
		pio_type = PIOE;
		break;

	default:
		break;
	}

	pio_set(pio_type, mask);
}

int ubi_pio_clearPin(uint8_t pPort, uint8_t pPin) {
	Pio *pio_type;
	uint32_t mask = (1 << pPin);

	switch (pPort) {
	case PORTA:
		pio_type = PIOA;
		break;

	case PORTB:
		pio_type = PIOB;
		break;

	case PORTC:
		pio_type = PIOC;
		break;

	case PORTD:
		pio_type = PIOD;
		break;

	case PORTE:
		pio_type = PIOE;
		break;

	default:
		break;
	}

	pio_clear(pio_type, mask);
}

int ubi_pio_getPin(uint8_t pPort, uint8_t pPin, uint8_t default_level) {
	Pio *pio_type;
	uint32_t mask = (1 << pPin);
	pio_type_t ul_type;

	switch (pPort) {
	case PORTA:
		pio_type = PIOA;
		break;

	case PORTB:
		pio_type = PIOB;
		break;

	case PORTC:
		pio_type = PIOC;
		break;

	case PORTD:
		pio_type = PIOD;
		break;

	case PORTE:
		pio_type = PIOE;
		break;

	default:
		break;
	}

	switch (default_level) {
		case UBI_PIO_HIGH_LEVEL:
			ul_type = PIO_OUTPUT_1;
			break;
		case UBI_PIO_LOW_LEVEL:
			ul_type = PIO_OUTPUT_0;
			break;
		case UBI_PIO_INPUT:
			ul_type = PIO_INPUT;
			break;
		default:
			break;
	}

	return pio_get(pio_type, ul_type, mask);
}
uint32_t ubi_pio_getPort(uint8_t pPort, uint8_t default_level)
{
	Pio *pio_type;
	uint32_t result;

	switch (pPort) {
	case PORTA:
		pio_type = PIOA;
		break;

	case PORTB:
		pio_type = PIOB;
		break;

	case PORTC:
		pio_type = PIOC;
		break;

	case PORTD:
		pio_type = PIOD;
		break;

	case PORTE:
		pio_type = PIOE;
		break;

	default:
		break;
	}

	switch (default_level) {
		case UBI_PIO_HIGH_LEVEL:
			result = pio_type->PIO_ODSR;
			break;
		case UBI_PIO_LOW_LEVEL:
			result = pio_type->PIO_ODSR;
			break;
		case UBI_PIO_INPUT:
			result = pio_type->PIO_PDSR;
			break;
		default:
			break;
	}

	return result;
}


int ubi_pio_togglePin(uint8_t pPort, uint8_t pPin) {
	uint32_t ul_pin = pPort*UBI_PORT + pPin;
	pio_toggle_pin(ul_pin);
}
