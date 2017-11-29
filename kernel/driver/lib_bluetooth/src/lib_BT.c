/*
 * BT_api.c
 *
 *  Created on: 2017. 4. 6.
 *      Author: JKwang
 */

/* -------------------------------------------------------------------------
 Include
 ------------------------------------------------------------------------- */
#include "../ubiconfig.h"

// standard c library include
#include <stdio.h>
#include <stdlib.h>
#include <sam4e.h>

// ubinos library include
#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"
#include "itf_ubinos/itf/bsp_fpu.h"

// chipset driver include
#include "lib_ubi_pio/itf/lib_ubi_pio.h"
#include "ioport.h"
#include "pio/pio.h"

// new estk driver include
#include "lib_new_estk_api/itf/new_estk_led.h"
#include "lib_new_estk_api/itf/new_estk_glcd.h"

#include "common/services/gpio/sam_gpio/sam_gpio.h"
#include "lib_new_estk_api/itf/new_estk_ioport_set.h"

#include "lib_bluetooth/itf/lib_BT.h"
#include "lib_bluetooth/itf/BT_Module_Interface.h"


 uint8_t W_packet[BTM_INTERFACE_BUF_SIZE],R_packet[BTM_INTERFACE_BUF_SIZE],current_CMD[BTM_INTERFACE_BUF_SIZE]={0,};

//Main Task Message Queue
extern msgq_pt Main_Msgq;
extern msgq_pt BT_user_event_queue , temp_user_event_queue;


int BT_Main_event_send(uint8_t Main_evt, uint8_t Main_evt_state, uint8_t* msg) {
	BT_Evt_t temp_msg;

	temp_msg.event = Main_evt;
	temp_msg.status = Main_evt_state;
	temp_msg.msg = msg;

	int r;

	r = msgq_send(Main_Msgq, (unsigned char*) &temp_msg);

	return r;
}
int BT_USER_event_send(uint8_t Main_evt, uint8_t Main_evt_state, uint8_t* msg) {
	BT_Evt_t temp_msg;

	temp_msg.event = Main_evt;
	temp_msg.status = Main_evt_state;
	temp_msg.msg = msg;

	int r;

	r = msgq_send(temp_user_event_queue, (unsigned char*) &temp_msg);


	return r;
}

//------------------------------------------------------------default data W/R
void BT_WRITE_CMD(uint8_t* buf, uint8_t CMD) {

	buf[INDEX_CMD] = CMD;

	//back-up W_packet
	for(int i=0 ; i<BTM_INTERFACE_BUF_SIZE; i++)
		current_CMD[i] = buf[i];

	//if gpio_pin is low, Write DATA
	while (gpio_pin_is_high(BT_PIN_INT))
		task_sleep(100);

	SPI_WRITE_DATA(buf, BTM_INTERFACE_BUF_SIZE);
	task_sleep(50);

}
void BT_READ_CMD(uint8_t *buf) {
	//if gpio_pin is high, READ DATA
	while (gpio_pin_is_low(BT_PIN_INT))
		task_sleep(100);

	SPI_READ_DATA(buf, BTM_INTERFACE_BUF_SIZE);
	//printf_glcd
	memcpy(R_packet,buf,BTM_INTERFACE_BUF_SIZE);

	//if gpio_pin is high, READ DATA
	while (gpio_pin_is_high(BT_PIN_INT))
		task_sleep(100);

}
//--------------------------------------------------------------------ACK
void BT_ACK_CHECK(uint8_t* buf) {
	printf("ACK : ");

	//Check ACK Command
	if (buf[INDEX_CMD] == current_CMD[INDEX_CMD] + ACK_SUM) {
		//SPIS Buffer initialization
		memset(buf, 0xFF, BTM_INTERFACE_BUF_SIZE);
	}
	//error handling
	else {
		BT_WRITE_CMD(current_CMD,current_CMD[0]);
	}

	//if gpio_pin is high, READ DATA
	while (gpio_pin_is_high(BT_PIN_INT))
		task_sleep(100);
}

void BT_ACK_SET(uint8_t cmd) {

	//spi buffer initialization
	memset(W_packet, 0xFF, BTM_INTERFACE_BUF_SIZE);

	//Set ACK packet to spi tx buffer
	W_packet[INDEX_CMD] = cmd + ACK_SUM;

	BT_WRITE_CMD(W_packet,W_packet[INDEX_CMD]);

	//Set SPI interrupt pin

}
//-------------------------------------------------------------------- interrupt
void SWITCH_isr1(void) {
	BT_Main_event_send(BT_SW1_EVT, SWITCH_TEST, NULL);
}
void SWITCH_isr2(void) {
	BT_Main_event_send(BT_SW2_EVT, SWITCH_TEST, NULL);
}
//PA17 interrupt
void PIN_isr1(void) {
	BT_Main_event_send(BTM_EVT_READ_INT, BTM_EVT_READ_INT, NULL);
}

//----------------------------------------------------------------------GPIO interrupt
void BT_interrupt_setting(void *pin_isr1) {
	//GPIO_PIN_init
	pio_configure_pin(BT_PIN_INT,
			PIO_INPUT | PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE);
	ubi_pio_interrupt_config( PORTA,
			BT_PIN_INT,														//
			PIO_DEGLITCH | PIO_PULLUP | PIO_IT_RISE_EDGE | PIO_DEBOUNCE,
			pin_isr1);
	pio_disable_interrupt( PIOA, BT_PIN_INT);
	task_sleep(100);
	pio_enable_interrupt( PIOA, BT_PIN_INT);

}
