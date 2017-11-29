/*
 * lib_motor_driver.c
 *
 *  Created on: 2015. 7. 3.
 *      Author: Administrator
 */

#include "../itf/lib_motor_driver.h"
#include "pio/pio.h"

void motor_init(void)
{
	int i;
	int j;

	pmc_enable_periph_clk(ID_PWM);

	PWM->PWM_DIS = 0xF;

	PWM->PWM_CLK = ((4 << 8) | 0x08 );

	for(i = 0; i < 4; i++){
	PWM->PWM_CH_NUM[i].PWM_CMR = PWM_CMR0_CPRE_MCKA | PWM_CMR0_CPOL;
	//j=PWM->PWM_CH_NUM[i].PWM_CMR;

	PWM->PWM_CH_NUM[i].PWM_CPRD = MAX_DUTY_CYCLE;
	//j=PWM->PWM_CH_NUM[i].PWM_CPRD;

	PWM->PWM_CH_NUM[i].PWM_CDTY = MIN_DUTY_CYCLE;
	//j=PWM->PWM_CH_NUM[i].PWM_CDTY;
	}

	pio_configure(PIOD,PIO_PERIPH_A,1u << 20,PIO_DEFAULT);
	pio_configure(PIOD,PIO_PERIPH_A,1u << 24,PIO_DEFAULT);

	pio_configure(PIOD,PIO_PERIPH_A,1u << 21,PIO_DEFAULT);
	pio_configure(PIOD,PIO_PERIPH_A,1u << 25,PIO_DEFAULT);

	pio_configure(PIOD,PIO_PERIPH_A,1u << 22,PIO_DEFAULT);
	pio_configure(PIOD,PIO_PERIPH_A,1u << 26,PIO_DEFAULT);

	pio_configure(PIOD,PIO_PERIPH_A,1u << 23,PIO_DEFAULT);
	pio_configure(PIOD,PIO_PERIPH_A,1u << 27,PIO_DEFAULT);

	PWM->PWM_ENA = 0xF;
	//i=PWM->PWM_SR;
	for(i = 0; i<4 ; i++)
	{
		motor_set(i, 0);
	}
}

static void motor_pwm_set(Pio *PIOX, Pio *PIOY, unsigned int PinX, unsigned int PinY)
{
	int i;
	PIOX->PIO_PDR =  PinX;
	//i=PIOX->PIO_PSR;

	PIOY->PIO_PER = PinY;
	//i=PIOY->PIO_PSR;

	PIOY->PIO_OER = PinY;
	//i=PIOY->PIO_OSR;

	PIOY->PIO_SODR = PinY;
	//i=PIOY->PIO_ODSR;
}

void motor_set(int port, int speed)
{
	char dir = 0;

	if(speed >= MAX_DUTY_CYCLE)
	{
		speed = MAX_DUTY_CYCLE;
	}
	else if(speed <= MAX_REVERSE_DUTY_CYCLE)
	{
		speed = MAX_REVERSE_DUTY_CYCLE;
	}

	if(speed >= 0){
		dir = 0;
	}
	else if(speed < 0){
		dir = 1;
		speed += 1024;
	}

	if(port == 0){
		(dir == 0) ? motor_pwm_set(PIOD, PIOD, 1u << 24, 1u << 20)
				: motor_pwm_set(PIOD, PIOD, 1u << 20,1u << 24);

	}
	else if(port == 1){
		(dir == 0) ? motor_pwm_set(PIOD, PIOD, 1u << 25, 1u << 21)
				: motor_pwm_set(PIOD, PIOD, 1u << 21,1u << 25);

	}
	else if(port == 2){
		(dir == 0) ? motor_pwm_set(PIOD, PIOD, 1u << 26, 1u << 22)
				: motor_pwm_set(PIOD, PIOD, 1u << 22,1u << 26);

	}
	else if(port == 3){
		(dir == 0) ? motor_pwm_set(PIOD, PIOD, 1u << 27, 1u << 23)
				: motor_pwm_set(PIOD, PIOD, 1u << 23,1u << 27);

	}

	PWM->PWM_CH_NUM[port].PWM_CDTYUPD = speed;

}

void motor_encoder(int pin, int line){

	unsigned int edge = 0;
	unsigned int dir = 0;
	unsigned int last = 0;

	if(line == 0)
		last = a_last;
	else if(line == 1)
		last = b_last;
	else if(line == 2)
		last = c_last;
	else if(line == 3)
		last = d_last;

	dir = pin & 2;
	edge = pin & 1;

	if(edge != last){
		if(edge && !dir){
			if(line == 0)
				a_current_count++;
			else if(line == 1)
				b_current_count++;
			else if(line == 2)
				c_current_count++;
			else if(line == 3)
				d_current_count++;
		}
		else if(edge && dir){
			if(line == 0)
				a_current_count--;
			else if(line == 1)
				b_current_count--;
			else if(line == 2)
				c_current_count--;
			else if(line == 3)
				d_current_count--;
		}
		else if(!edge && dir){
			if(line == 0)
				a_current_count++;
			else if(line == 1)
				b_current_count++;
			else if(line == 2)
				c_current_count++;
			else if(line == 3)
				d_current_count++;
		}
		else if(!edge && !dir){
			if(line == 0)
				a_current_count--;
			else if(line == 1)
				b_current_count--;
			else if(line == 2)
				c_current_count--;
			else if(line == 3)
				d_current_count--;
		}

		if(line == 0)
			a_last = edge;

		else if(line == 1)
			b_last = edge;

		else if(line == 2)
			c_last = edge;

		else if(line == 3)
			d_last = edge;

	}
}

static void ESTK_motor_encoder_ISR(uint32_t ul_mask, uint32_t ul_id){

	unsigned int val_output = 0;
	unsigned int pin = 0;

	if(ul_id == PIO_PC29){
		val_output = PIOC->PIO_PDSR;
		pin = ((val_output >> 29) & 1) | ((val_output >> 29) & 2);
		motor_encoder(pin, 0);
	}
	else if(ul_id == PIO_PD18){
		val_output = PIOD->PIO_PDSR;
		pin = ((val_output >> 18) & 1) | ((val_output >> 18) & 2);
		motor_encoder(pin, 1);
	}
	else if(ul_id == PIO_PD28){
		val_output = PIOD->PIO_PDSR;
		pin = ((val_output >> 28) & 1) | ((val_output >> 28) & 2);
		motor_encoder(pin, 2);
	}
	else if(ul_id == PIO_PD30){
		val_output = PIOD->PIO_PDSR;
		pin = ((val_output >> 30) & 1) | ((val_output >> 30) & 2);
		motor_encoder(pin, 3);
	}
}

void encoder_init(void){

	pio_configure(ESTK_MOT0_ENCODER_INT.pio,ESTK_MOT0_ENCODER_INT.type,ESTK_MOT0_ENCODER_INT.mask,ESTK_MOT0_ENCODER_INT.attribute);
	pio_configure(ESTK_MOT1_ENCODER_INT.pio,ESTK_MOT1_ENCODER_INT.type,ESTK_MOT1_ENCODER_INT.mask,ESTK_MOT1_ENCODER_INT.attribute);
	pio_configure(ESTK_MOT2_ENCODER_INT.pio,ESTK_MOT2_ENCODER_INT.type,ESTK_MOT2_ENCODER_INT.mask,ESTK_MOT2_ENCODER_INT.attribute);
	pio_configure(ESTK_MOT3_ENCODER_INT.pio,ESTK_MOT3_ENCODER_INT.type,ESTK_MOT3_ENCODER_INT.mask,ESTK_MOT3_ENCODER_INT.attribute);


	pmc_enable_periph_clk(ID_PIOC);
	if (ESTK_motor_encoder_ISR != NULL){
	pio_set_debounce_filter(ESTK_MOT0_ENCODER_INT.pio,ESTK_MOT0_ENCODER_INT.mask,100);
	intr_connectisr(PIOC_IRQn,PIOC_Handler,0x40,0);
	pio_handler_set(ESTK_MOT0_ENCODER_INT.pio,ID_PIOC,ESTK_MOT0_ENCODER_INT.mask,ESTK_MOT0_ENCODER_INT.attribute,ESTK_motor_encoder_ISR);
	NVIC_EnableIRQ(PIOC_IRQn);
	pio_enable_interrupt(ESTK_MOT0_ENCODER_INT.pio,ESTK_MOT0_ENCODER_INT.mask);
	}

	pmc_enable_periph_clk(ID_PIOD);
	if (ESTK_motor_encoder_ISR != NULL){
	pio_set_debounce_filter(ESTK_MOT1_ENCODER_INT.pio,ESTK_MOT1_ENCODER_INT.mask,100);
	intr_connectisr(PIOD_IRQn,PIOD_Handler,0x40,0);
	pio_handler_set(ESTK_MOT1_ENCODER_INT.pio,ID_PIOD,ESTK_MOT1_ENCODER_INT.mask,ESTK_MOT1_ENCODER_INT.attribute,ESTK_motor_encoder_ISR);
	NVIC_EnableIRQ(PIOD_IRQn);
	pio_enable_interrupt(ESTK_MOT1_ENCODER_INT.pio,ESTK_MOT1_ENCODER_INT.mask);
	}

	if (ESTK_motor_encoder_ISR != NULL){
	pio_set_debounce_filter(ESTK_MOT2_ENCODER_INT.pio,ESTK_MOT2_ENCODER_INT.mask,100);
	intr_connectisr(PIOD_IRQn,PIOD_Handler,0x40,0);
	pio_handler_set(ESTK_MOT2_ENCODER_INT.pio,ID_PIOD,ESTK_MOT2_ENCODER_INT.mask,ESTK_MOT2_ENCODER_INT.attribute,ESTK_motor_encoder_ISR);
	NVIC_EnableIRQ(PIOD_IRQn);
	pio_enable_interrupt(ESTK_MOT2_ENCODER_INT.pio,ESTK_MOT2_ENCODER_INT.mask);
	}

	if (ESTK_motor_encoder_ISR != NULL){
	pio_set_debounce_filter(ESTK_MOT3_ENCODER_INT.pio,ESTK_MOT3_ENCODER_INT.mask,100);
	intr_connectisr(PIOD_IRQn,PIOD_Handler,0x40,0);
	pio_handler_set(ESTK_MOT3_ENCODER_INT.pio,ID_PIOD,ESTK_MOT3_ENCODER_INT.mask,ESTK_MOT3_ENCODER_INT.attribute,ESTK_motor_encoder_ISR);
	NVIC_EnableIRQ(PIOD_IRQn);
	pio_enable_interrupt(ESTK_MOT3_ENCODER_INT.pio,ESTK_MOT3_ENCODER_INT.mask);
	}
}

int encoder_get(int port)
{
	int count = 0;

	if(port == 0)
		count = a_current_count;
	else if(port == 1)
		count = b_current_count;
	else if(port == 2)
		count = c_current_count;
	else if(port == 3)
		count = d_current_count;

	return count;
}

void encoder_reset(int port){
	int in=port;

	switch(in){
		case 0:
			a_current_count=0;
			break;
		case 1:
			b_current_count=0;
			break;
		case 2:
			c_current_count=0;
			break;
		case 3:
			d_current_count=0;
			break;
	}
}
