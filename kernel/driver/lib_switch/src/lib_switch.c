/*
 * lib_default.c
 *
 *  Created on: 2015. 9. 4.
 *      Author: Taemin
 */

#include <stdio.h>
#include <sam4e.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"
#include "lib_ubi_pio/itf/lib_ubi_pio.h"
#include "pio/pio.h"
#include "sam4e16e.h"
//#include "../kernel/porting/lib_sam4e/sam/utils/cmsis/sam4e/include/sam4e16e.h"
#include "../kernel/porting/lib_sam4e/sam/boards/sam4e_ek/sam4e_ek.h"

#include "../itf/lib_switch.h"

int switch_get(int switch_num){
	switch(switch_num){
	case SW1:
		return ((PIOA->PIO_PDSR & PIO_PA19) >> PIO_PA19_IDX);
	case SW2:
		return ((PIOA->PIO_PDSR & PIO_PA20) >> PIO_PA20_IDX);
	default :
		printf("wrong switch number\r\n");
		break;
	}
}

void switch_init(void *switch_isr1, void *switch_isr2){
	pio_configure_pin(PIO_PA19_IDX, PIO_INPUT | PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE);
	ubi_pio_interrupt_config( PORTA, GPIO_PUSH_BUTTON_1, PIO_DEGLITCH | PIO_PULLUP | PIO_IT_FALL_EDGE | PIO_DEBOUNCE, switch_isr1 );
	pio_disable_interrupt( PIOA, PIN_PUSHBUTTON_1_MASK );
	task_sleep(100);
	pio_enable_interrupt( PIOA, PIN_PUSHBUTTON_1_MASK );

	pio_configure_pin(PIO_PA20_IDX, PIO_INPUT | PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE);
	ubi_pio_interrupt_config( PORTA, GPIO_PUSH_BUTTON_2, PIO_DEGLITCH | PIO_PULLUP | PIO_IT_FALL_EDGE | PIO_DEBOUNCE, switch_isr2 );
	pio_disable_interrupt( PIOA, PIN_PUSHBUTTON_2_MASK );
	task_sleep(100);
	pio_enable_interrupt( PIOA, PIN_PUSHBUTTON_2_MASK );
}
