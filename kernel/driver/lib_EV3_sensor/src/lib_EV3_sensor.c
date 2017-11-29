/*
 * lib_EV3_sensor.c
 *
 *  Created on: 2015. 7. 8.
 *      Author: minsu
 */
#include <stdio.h>
#include <sam4e.h>
#include <stdbool.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"
#include "../../lib_new_estk_api/itf/new_estk_glcd.h"
#include "../itf/lib_EV3_sensor.h"
#include "../../../porting/bsp_armcortexm_sam4eek/itf/sam4e16e.h"
#include "../../../porting/lib_sam4e/sam/drivers/uart/uart.h"
#include "../../../porting/lib_sam4e/sam/drivers/tc/tc.h"

void initEV3UARTMode(){
	EV3UartMode ev3uartmode;
}

void initEV3UARTSensor(EV3UARTSensor* ev3uartsensor){
	ev3uartsensor->status=RESET;
	ev3uartsensor->speed=2400;
	ev3uartsensor->mode=-1;
	ev3uartsensor->num_samples=1;
}

EV3UartMode* get_mode(EV3UARTSensor* ev3uartsensor,int mode){
	return ev3uartsensor->mode_array[mode];
}

unsigned long get_long(unsigned char* bb, int offset){
	return((unsigned long)bb[offset]) | ((unsigned long)bb[offset+1]<<8) |
			((unsigned long)bb[offset+2]<<16) | ((unsigned long)bb[offset+3]<<24);
}

void get_string(unsigned char* string,unsigned char* bb, int len){
	for(int i=0;i<len;i++){
		if(bb[i]==0)break;
		string[i]=bb[i];
	}
}

float get_float(unsigned char* bb, int offset){
	union Data{
		unsigned long l;
		float f;
	}data;

	data.l = get_long(bb,offset);
	return data.f;
}

int get_int(unsigned char* bb, int offset){
	return ((int)bb[offset]) | ((int)bb[offset+1] << 8);
}

int sample_size(EV3UARTSensor* ev3uartsensor){
	return ev3uartsensor->num_samples;
}

int get_current_mode(EV3UARTSensor* ev3uartsensor){
	return ev3uartsensor->mode;
}

int get_number_of_modes(EV3UARTSensor* ev3uartsensor){
	return ev3uartsensor->modes;
}

int get_type(EV3UARTSensor* ev3uartsensor){
	return ev3uartsensor->type;
}

int get_status(EV3UARTSensor* ev3uartsensor){
	return ev3uartsensor->status;
}

void fetch_sample(EV3UARTSensor* ev3uartsensor,float* sample, int offset){
	for(int i=0;i<(ev3uartsensor->num_samples);i++){
		sample[offset+i] = ev3uartsensor->value[i];
	}
}

void begin(EV3UARTSensor* ev3uartsensor){
	int abcdsr;

	UART0->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

	PIOA->PIO_IDR = 0x00000600;

	PIOA->PIO_PUDR = 0x00000600;
	abcdsr = PIOA->PIO_ABCDSR[0];
	PIOA->PIO_ABCDSR[0] &= (~0x00000600 & abcdsr);
	abcdsr = PIOA->PIO_ABCDSR[1];
	PIOA->PIO_ABCDSR[1] = 0x00000600;
	PIOA->PIO_PDR = 0x00000600;

	PMC->PMC_PCER0 = (1 << ID_UART0);

	UART0->UART_BRGR = CLOCK/(16*(ev3uartsensor->speed)); //baudrate 2400
	UART0->UART_MR = UART_MR_PAR_NO;
	UART0->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
	UART0->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}

void end(){
	UART0->UART_CR = UART_CR_RXDIS | UART_CR_TXDIS;
}

void reset(EV3UARTSensor* ev3uartsensor){
	ev3uartsensor->status = RESET;
	ev3uartsensor->speed=2400;
	end();
	begin(ev3uartsensor);
}

void timer_start(){
//	pmc_enable_periph_clk(ID_TC5);
//	tc_init(TC1,2,TC_CMR_TCCLKS_TIMER_CLOCK1);
//	tc_start(TC1,2);

	pmc_enable_periph_clk(ID_TC1);
	tc_init(TC0,1,TC_CMR_TCCLKS_TIMER_CLOCK1);
	tc_start(TC0,1);
}

void read_timer_value(uint32_t* cv){
//	*cv= TC1->TC_CHANNEL[2].TC_CV;
	*cv= TC0->TC_CHANNEL[1].TC_CV;
}

int exp2(int val){
	switch(val){
	case 0: return 1;
	case 1: return 2;
	case 2: return 4;
	case 3: return 8;
	case 4: return 16;
	case 5: return 32;
	default: return 0;
	}
}

void send_select(uint8_t mode){
	uint8_t e=1;
	uint8_t tx_buf;
	uint8_t checksum = 0xff ^ CMD_SELECT;
	tx_buf=CMD_SELECT;
	while(e){
		e=uart_write(UART0,tx_buf);
	}
	e=1;
	while(e){
		e=uart_write(UART0,mode);
	}
	e=1;
	checksum ^= mode;
	while(e){
		e=uart_write(UART0,checksum);
	}
}

void set_mode(EV3UARTSensor* ev3uartsensor,int mode){
	send_select(mode);
	ev3uartsensor->mode =  mode;
	ev3uartsensor->num_samples = ev3uartsensor->mode_array[mode]->sets;
}

void send_write(unsigned char* bb, int len){
	unsigned char b = CMD_WRITE | (len << CMD_LLL_SHIFT);
	unsigned char checksum = 0xff ^ b;
	uart_write(UART0,b);
	for(int i=0;i<len;i++){
		uart_write(UART0,bb[i]);
		checksum ^= bb[i];
	}
	uart_write(UART0,checksum);
}

/*-----------------------------------------------------------------------------------------------------------------------*/
void ev3_sensor_init(int port, int mode){

	uint8_t ack_flag=CLEAR;
	timer_start();
	initEV3UARTSensor(&sensor);

	port_select(port);

	begin(&sensor);

	for(;;){
		check_for_initiation(&sensor, mode, &ack_flag );
		if(ack_flag == SET)
			break;
		ack_flag = CLEAR;
	}
}

void port_select(int port){
	switch(port){
		case 0:
			pio_configure(PIOC,PIO_OUTPUT_0,1u<<20,PIO_PULLUP);
			pio_configure(PIOE,PIO_OUTPUT_1,1u<<3,PIO_PULLUP);
			pio_configure(PIOE,PIO_OUTPUT_1,1u<<4,PIO_PULLUP);
			pio_configure(PIOE,PIO_OUTPUT_1,1u<<5,PIO_PULLUP);
			break;
		case 1:
			pio_configure(PIOC,PIO_OUTPUT_1,1u<<20,PIO_PULLUP);
			pio_configure(PIOE,PIO_OUTPUT_0,1u<<3,PIO_PULLUP);
			pio_configure(PIOE,PIO_OUTPUT_1,1u<<4,PIO_PULLUP);
			pio_configure(PIOE,PIO_OUTPUT_1,1u<<5,PIO_PULLUP);
			break;
		case 2:
			pio_configure(PIOC,PIO_OUTPUT_1,1u<<20,PIO_PULLUP);
			pio_configure(PIOE,PIO_OUTPUT_1,1u<<3,PIO_PULLUP);
			pio_configure(PIOE,PIO_OUTPUT_0,1u<<4,PIO_PULLUP);
			pio_configure(PIOE,PIO_OUTPUT_1,1u<<5,PIO_PULLUP);
			break;
		case 3:
			pio_configure(PIOC,PIO_OUTPUT_1,1u<<20,PIO_PULLUP);
			pio_configure(PIOE,PIO_OUTPUT_1,1u<<3,PIO_PULLUP);
			pio_configure(PIOE,PIO_OUTPUT_1,1u<<4,PIO_PULLUP);
			pio_configure(PIOE,PIO_OUTPUT_0,1u<<5,PIO_PULLUP);
			break;
		}
}

void check_for_initiation(EV3UARTSensor* ev3uartsensor, int mod, uint8_t* ack_flag){
	uint8_t tx_buf;
	uint8_t rx_buf;
	uint32_t cv=0;
	uint8_t cmd;
	unsigned char checksum;
	unsigned char type;
	unsigned char modes;
	unsigned char views;
	unsigned char lll;
	unsigned char l;
	unsigned char mode;
	unsigned char b;
	float low, high;

	uart_buf_read( &cmd );

	if(ev3uartsensor->status == STARTED){
		switch(cmd){

		case BYTE_ACK:
			tx_buf = BYTE_ACK;
			uart_write(UART0,tx_buf);
			task_sleep(100);
			end();
			begin(ev3uartsensor);

			set_mode(ev3uartsensor,mod);
			read_timer_value(&cv);
			ev3uartsensor->last_nack=cv;
			ev3uartsensor->status = DATA_MODE;
			ev3uartsensor->data_errors = 0;
			ev3uartsensor->consecutive_errors = 0;
			ev3uartsensor->recent_messages = 0;
			*ack_flag=SET;
			ev3uartsensor->seq_num=0;

			//초기화한 후에는 RXRDY 인터럽트를 사용해서 측정 데이터를 받는다.
			if(ev3uartsensor->speed != 2400){
				intr_disable(ID_UART0);
				intr_connectisr(ID_UART0, UartRead_ISR, 0x40, 0);
				intr_enable(ID_UART0);
				uart_enable_interrupt(UART0, UART_IER_RXRDY);
			}
			break;

		case CMD_TYPE:
			checksum = 0xff ^ cmd;
			uart_buf_read( &rx_buf );
			type = rx_buf;
			checksum ^= type;

			uart_buf_read( &rx_buf );

			if(checksum == rx_buf){
				ev3uartsensor->type  = type;
				ev3uartsensor->status = STARTED;
			}
			break;

		case CMD_MODES:
			checksum = 0xff ^ cmd;
			uart_buf_read( &rx_buf );
			modes = rx_buf;
			checksum ^= modes;

			uart_buf_read( &rx_buf );

			views = rx_buf;
			checksum ^= views;

			uart_buf_read( &rx_buf );

			if(checksum == rx_buf){
				ev3uartsensor->views = views;
				ev3uartsensor->modes = modes+1;
				EV3UartMode ev3_uartmode[modes+1];
				for(int i=0;i<=modes;i++){
					ev3uartsensor->mode_array[i] = (ev3_uartmode+i);
				}
			}
			break;

		case CMD_SPEED:
			checksum = 0xff ^ cmd;
			unsigned char bb[4];
			for( int i = 0 ; i < 4; i++ ){
				uart_buf_read( &rx_buf );
				b = rx_buf;
				checksum ^= b;
				bb[i] = b;
			}

			uart_buf_read( &rx_buf );

			if(checksum == rx_buf){
				ev3uartsensor->speed = get_long(bb,0);
			}
			break;
		} // end of switch

		if(( cmd & CMD_MASK ) == CMD_INFO ){
			lll = ( cmd & CMD_LLL_MASK ) >> CMD_LLL_SHIFT;
			l = exp2(lll);
			mode = (cmd & CMD_MMM_MASK);
			checksum = 0xff ^ cmd;

			uart_buf_read( &rx_buf );

			type = rx_buf;
			checksum ^= type;
			unsigned char bb[l];
			for(int i=0;i<l;i++){
				uart_buf_read( &rx_buf );

				unsigned char b = rx_buf;
				checksum ^= b;
				bb[i] = b;
			}

			uart_buf_read( &rx_buf );

			if( checksum != rx_buf )
				printf("Invalid info checksum\r\n");
			else{
				switch( type ){
				case 0x00:
					get_string(ev3uartsensor->mode_array[mode]->name,bb,l);
					break;
				case 0x01:
					low = get_float(bb,0);
					high = get_float(bb,4);
					ev3uartsensor->mode_array[mode]->raw_low = low;
					ev3uartsensor->mode_array[mode]->raw_high = high;
					break;
				case 0x02:
					low = get_float(bb,0);
					high = get_float(bb,4);
					ev3uartsensor->mode_array[mode]->si_low = low;
					ev3uartsensor->mode_array[mode]->si_high = high;
					break;
				case 0x03:
					low = get_float(bb,0);
					high = get_float(bb,4);
					ev3uartsensor->mode_array[mode]->pct_low = low;
					ev3uartsensor->mode_array[mode]->pct_high = high;
					break;
				case 0x04:
					get_string(ev3uartsensor->mode_array[mode]->symbol,bb,l);
					break;
				case 0x80:
					ev3uartsensor->mode_array[mode]->sets = bb[0];
					ev3uartsensor->mode_array[mode]->data_type = bb[1];
					ev3uartsensor->mode_array[mode]->figures = bb[2];
					ev3uartsensor->mode_array[mode]->decimals = bb[3];
					break;
				}
			} // end of else
		}
	} // end of if
	else if(cmd == CMD_TYPE){
		checksum = 0xff ^ cmd;
		uart_buf_read( &rx_buf );
		type = rx_buf;
		checksum ^= type;

		uart_buf_read( &rx_buf );

		if(checksum == rx_buf){
			ev3uartsensor->type  = type;
			ev3uartsensor->status = STARTED;
		}
	}
}

void UartRead_ISR(){

	uint32_t cv=0;
	uint8_t tx;
	uint8_t rx=0;

	uart_read(UART0, &rx);

	if( sensor.status == DATA_MODE ){
		switch(sensor.seq_num){
		case 0:
			if((rx & CMD_MASK) == CMD_DATA)
				sensor.seq_num=1;
			break;
		case 1:
			sensor.value[0] = rx;
			sensor.seq_num=2;
			break;
		case 2:
			sensor.seq_num = 0;
			break;
		}
	}

	read_timer_value(&cv);

	if( sensor.status == DATA_MODE && ( cv - ( sensor.last_nack )) > HEART_BEAT ){
		tx = BYTE_NACK;
		uart_write( UART0, tx );
		read_timer_value( &cv );
		sensor.last_nack = cv;
	}
}

int ev3_sensor_get(int port){
	return sensor.value[0];
}

void uart_buf_read(uint8_t* buf){
	int val = 1;
	while( val )
		val = uart_read(UART0, buf);
}
