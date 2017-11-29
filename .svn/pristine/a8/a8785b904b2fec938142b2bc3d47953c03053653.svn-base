/*
 * lib_i2c.c
 *
 *  Created on: 2015. 7. 8.
 *      Author: minsu
 */

#include <stdio.h>
#include <sam4e.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"

#include "../itf/lib_i2c.h"
#include "../../../porting/bsp_armcortexm_sam4eek/itf/sam4e16e.h"

void lib_i2c_component_init( void )
{
	printf("library i2c test\n\r");
}

#define	N_PORTS	4
#define I2C_MAX_PARTIAL_TRANSACTIONS	3

struct i2c_pin_pair{
	unsigned int scl;
	unsigned int sda;
};
/*
 * I2C Sensor port pin assign
 */
static const struct i2c_pin_pair i2c_pin[4] = {
		{1 << 0, 1 << 9},	// SEN_A
		{1 << 10, 1 << 11},	// SEN_B
		{1 << 13, 1 << 14},	// SEN_C
		{1 << 16, 1 << 17}	// SEN_D
};

struct i2c_partial_transaction {
	unsigned char start:1;
	unsigned char restart:1;
	unsigned char stop:1;
	unsigned char tx:1;
	unsigned char last_pt:1;	// Last pt in transaction
	unsigned short nbytes;		// N bytes to transfer
	unsigned char* data;		// Data buffer
};

typedef enum {
	  I2C_UNINITIALIZED = 0,
	  I2C_IDLE,
	  I2C_BEGIN,
	  I2C_RESTART1,
	  I2C_START1,
	  I2C_START2,
	  I2C_START3,
	  I2C_START_RECLOCK1,
	  I2C_LOW0,
	  I2C_LOW1,
	  I2C_HIGH0,
	  I2C_HIGH1,
	  I2C_STOP0,
	  I2C_STOP1,
	  I2C_STOP2,
	  I2C_STOP3,
} i2c_port_state;

struct i2c_port_struct {
	unsigned int scl_pin;
	unsigned int sda_pin;
	unsigned char addr_int[2];	/* Debice address with internal address */
	unsigned char addr;			/* Just device address */
	struct i2c_partial_transaction partial_transaction[I2C_MAX_PARTIAL_TRANSACTIONS];
	struct i2c_partial_transaction *current_pt;

	i2c_port_state state;

	unsigned int nbits;
	unsigned int ack_slot;
	unsigned int ack_slot_pending;
	unsigned char *data;
	unsigned int transmitting;
	unsigned int fault;
	unsigned int n_fault;
	unsigned int n_good;
	unsigned int ack_fail;
	unsigned int ack_good;
	unsigned int pt_num;
	unsigned int pt_begun;
};

static struct i2c_port_struct i2c_port[N_PORTS];

/// Some logging code trying to debug the ultrasonic sensor
#if 0
#define I2C_LOG_N	200
static unsigned char i2c_logb[I2C_LOG_N];
static unsigned int i2c_logn;
static void i2c_log(int port, int state, int inpin)
{
	static unsigned int last_valid;
	if(i2c_logn < I2C_LOG_N && (state > I2C_IDLE || last_valid) && port == 0) {
		i2c_logb[i2c_logn] = state | (inpin ? 0x10 : 0);
		i2c_logn++;
		last_valid = (state > I2C_IDLE);
	}
}
#else
#define	i2c_log(a, b, c) do{} while(0)
#endif

/*
 * The I2C state machines are pumped by a timer interrupt
 * running at 4x the bit speed.
 */
static unsigned int i2c_int_count = 0;

void i2c_timer_isr_C(void)
{
	int i;
	unsigned int codr = 0;
	unsigned int sodr = 0;
	unsigned int oer = 0;
	unsigned int odr = 0;
	unsigned int inputs = PIOC->PIO_PDSR;	// Pin data status register

	struct i2c_port_struct *p = i2c_port;

	unsigned int dummy = TC1->TC_CHANNEL[1].TC_SR;

	i2c_int_count++;

	for(i=0; i<N_PORTS; i++) {
		i2c_log(i, p->state, inputs & p->sda_pin);

		switch(p->state)
		{
		default:
		case I2C_UNINITIALIZED:
			/* Uninitialized */
			break;
		case I2C_IDLE:
			/* Not in a transaction */
			break;
		case I2C_BEGIN:
			/* Star the current partial transaction */
			p->pt_begun |= (1 << p->pt_num);
			oer |= p->sda_pin | p->scl_pin;

			if(p->current_pt && p->current_pt->nbytes)
			{
				p->data = p->current_pt->data;
				p->nbits = p->current_pt->nbytes * 8;
				p->transmitting = p->current_pt->tx;
				p->ack_slot = 0;
				p->ack_slot_pending = 0;
				p->fault = 0;

				if(!p->transmitting)
					*(p->data) = 0;
				if(p->current_pt->restart)
				{
					/* Make sure both SDA and SCL are high */
					sodr |= p->scl_pin | p->sda_pin;
					p->state = I2C_RESTART1;
				}
				else if(p->current_pt->start)
				{
					sodr |= p->sda_pin;
					p->state = I2C_START1;
				}
				else
				{
					codr |= p->scl_pin;
					p->state = I2C_LOW0;
				}
			}
			else
				p->state = I2C_IDLE;
			break;
		case I2C_RESTART1:
			/* Take SCL pin low while SDA pin high */
			codr |= p->scl_pin;
			p->state = I2C_START1;
			break;
		case I2C_START1:
			/* Take SCL pin high while SDA pin high */
			sodr |= p->scl_pin;
			p->state = I2C_START2;
			break;
		case I2C_START2:
			if(inputs & p->sda_pin)
			{
				/* Take SDA pin low while SCL pin is high */
				codr |= p->sda_pin;
				p->state = I2C_START3;
			}
			else
			{
				/* SDA pin was not high, so do a clock */
				codr |= p->scl_pin;
				p->state = I2C_START_RECLOCK1;
			}
			break;
		case I2C_START_RECLOCK1:
			codr |= p->scl_pin;
			p->state = I2C_START1;
			break;
		case I2C_START3:
			/* Take SCL pin low */
			codr |= p->scl_pin;
			p->state = I2C_LOW0;
			break;
		case I2C_LOW0:
			/* SCL is low */
			if(p->ack_slot_pending)
			{
				p->ack_slot = 1;
				p->ack_slot_pending = 0;
			}
			else
				p->ack_slot = 0;

			if(p->nbits || p->ack_slot)
			{
				if(p->ack_slot)
				{
					if(p->transmitting)
						odr |= p->sda_pin;
					else
					{
						oer |= p->sda_pin;
						codr |= p->sda_pin;
					}
				}
				else if(!p->transmitting)
					odr |= p->sda_pin;
				else
				{
					/* Transmitting, and not an ack_slot so send next bit */
					oer |= p->sda_pin;
					p->nbits--;
					if(((*(p->data)) >> (p->nbits & 7)) & 0x01)
						sodr |= p->sda_pin;
					else
						codr |= p->sda_pin;
					if((p->nbits & 7) == 0)
					{
						p->data++;
						if(p->nbits || p->transmitting)
							p->ack_slot_pending = 1;
					}
				}
				p->state = I2C_LOW1;
			}
			else if(p->current_pt->stop)
				p->state = I2C_STOP0;
			else
			{
				p->current_pt++;
				p->pt_num++;
				sodr |= p->sda_pin;
				p->state = I2C_BEGIN;
			}
			break;
		case I2C_LOW1:
			/* Take SCL pin high */
			sodr |= p->scl_pin;
			p->state = I2C_HIGH0;
			break;
		case I2C_HIGH0:
			/* Wait for high pulse width. If someone else is not holding the pin down, then advance */
			if(inputs & p->scl_pin)
				p->state = I2C_HIGH1;
			break;
		case I2C_HIGH1:
			if(p->transmitting && p->ack_slot)
			{
				/* Expect ack from slave */
				if(inputs & p->sda_pin)
				{
					p->n_fault++;
					p->ack_fail++;
					p->fault=1;
					codr |= p->scl_pin;
					p->state = I2C_STOP0;
				}
				else
				{
					p->ack_good++;
					codr |= p->scl_pin;
					p->state = I2C_LOW0;
				}
			}
			else
			{
				/* Read pin if needed, then take SCL low */
				if(!p->transmitting && !p->ack_slot)
				{
					/* Receive a bit. */
					unsigned char *d = p->data;
					p->nbits--;
					if(inputs & p->sda_pin)
						*d |= (1 << (p->nbits & 7));
					if(p->nbits && ((p->nbits & 7) == 0))
					{
						p->data++;
						d = p->data;
						p->ack_slot_pending = 1;
						*d = 0;
					}
				}
				codr |= p->scl_pin;
				p->state = I2C_LOW0;
			}
			break;
		case I2C_STOP0:
			/* Take SDA low (SCL is already low) */
			oer |= p->sda_pin;
			codr |= p->sda_pin;
			p->state = I2C_STOP1;
			break;
		case I2C_STOP1:
			/* Take SCL pin high */
			sodr |= p->scl_pin;
			p->state = I2C_STOP2;
			break;
		case I2C_STOP2:
			/* Take SDA pin high */
			sodr |= p->sda_pin;
			p->state = I2C_STOP3;
			break;
		case I2C_STOP3:
			if(p->current_pt->last_pt)
			{
				p->state = I2C_IDLE;
			}
			else
			{
				p->current_pt++;
				p->pt_num++;
				p->state = I2C_BEGIN;
			}
		}	/// End of Switch

		p++;
	}	// End of For loop

	if(codr)
		PIOC->PIO_CODR |= codr;	// Clear output data register
	if(sodr)
		PIOC->PIO_SODR |= sodr;	// Set output data register
	if(oer)
		PIOC->PIO_OER |= oer;	// Output enable register
	if(odr)
		PIOC->PIO_ODR |= odr;	// Output disable register

}	// End of ISR

void i2c_timer_isr_entry(void)
{
	i2c_timer_isr_C();
}

void i2c_init(void)
{
	int i;
	unsigned int dummy;
	struct i2c_port_struct *p = i2c_port;

	for(i=0; i<N_PORTS; i++)
	{
		p->state = I2C_IDLE;
		p->scl_pin = i2c_pin[i].scl;
		p->sda_pin = i2c_pin[i].sda;
		i2c_disable(i);
		p++;
	}

	/* Set up Timer Counter 4 */
	PMC->PMC_PCER0 = 1 << ID_TC4;	/* Power enable Timer */
	PMC->PMC_PCER0 = 1 << ID_PIOC;	/* Power enable peripheral C counter */

	TC1->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKDIS;	// Disable Clock
	TC1->TC_CHANNEL[1].TC_IDR = 0xFFFFFFFF;	// Disable Interrupt Timer
	dummy = TC1->TC_CHANNEL[1].TC_SR;	// Reset status register
	TC1->TC_CHANNEL[1].TC_CMR = TC_CMR_CPCTRG;	// RC compare mode enable
	TC1->TC_CHANNEL[1].TC_RC = (BOARD_MCKL/2) / (4 * 9600);	// Setting baud rate 9600
	TC1->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;	// Enable RC trigger
	TC1->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN;	// Counter clock enable

	/* NVIC Setting */
	intr_connectisr(ID_TC4, i2c_timer_isr_entry, 0x40, 0);	//
	intr_enable(ID_TC4);	// Enable NVIC interrupt

	TC1->TC_CHANNEL[1].TC_CCR = TC_CCR_SWTRG;	// Enable Software Trigger Enable
}

int i2c_busy(int port)
{
	// Exception handling for port number
	if(port >= 0 && port < N_PORTS)
		return (i2c_port[port].state > I2C_IDLE);
	return -1;
}

int i2c_disable(int port)
{
	// Exception handling for port number
	if(port >= 0 && port < N_PORTS)
	{
		struct i2c_port_struct *p = &i2c_port[port];
		unsigned int pinmask = p->scl_pin | p->sda_pin;

		/*
		 * Original: PIOC->PIO_ODR = pinmask;
		 * 센서의 GPIO와 겹치는 문제 발생 -> PIO Output을 disable함으로써,
		 * NXT_SENSOR 사용이 컨트롤이 안되는 문제가 발생
		 * 나중에, I2C Disable시 문제의 가능성이 있을것으로 생각됨!!
		 */
		PIOC->PIO_CODR |= pinmask;
		return 0;
	}
	// Return error code: Invalid port number
	return -1;
}

int i2c_enable(int port)
{
	// Exception handling for port number
	if(port >= 0 && port < N_PORTS)
	{
		struct i2c_port_struct *p = &i2c_port[port];
		unsigned int pinmask = p->scl_pin | p->sda_pin;
		p->state = I2C_IDLE;
		/*
		 * Set clock pin for output, open collector driver, with pull ups enabled.
		 * Set data to be enabled for output with pull ups disabled.
		 */
		PIOC->PIO_SODR |= pinmask;	// Output set high register
		PIOC->PIO_OER |= pinmask;	// Output enable register
		PIOC->PIO_MDER |= p->scl_pin;	// Multi-driver enable register
		PIOC->PIO_PUDR |= p->sda_pin;	// Pull up disable register
		PIOC->PIO_PUER |= p->scl_pin;	// Pull up enable register
		return 0;
	}
	// Return error code: Invalid port number
	return -1;
}

int i2c_start_transaction(int port,
						  unsigned int address,
						  int internal_address,
						  int n_internal_address_bytes,
						  unsigned char *data,
						  unsigned int nbytes,
						  int write)
{
	struct i2c_port_struct *p;
	struct i2c_partial_transaction *pt;

	/* Exception Handling */
	if(port < 0 || port >= N_PORTS)
		// Return error code: Invalid port number
		return -1;
	if(i2c_busy(port))
		return -1;

	p = &i2c_port[port];
	p->pt_num = 0;
	p->pt_begun = 0;
	pt = p->partial_transaction;
	p->current_pt = pt;

	memset(pt, 0, sizeof(p->partial_transaction));

	if(n_internal_address_bytes > 0)
	{
		// Set up command to write the internal address to the device
		p->addr_int[0] = (address << 1);	// This is a write
		p->addr_int[1] = internal_address;

		// Set up first partial transactions: start address and internal address if required

		pt->start = 1;
		// We add an extra stop for the odd LEGO i2c sensor, but only on a read
		pt->stop = (write ? 0 : 1);
		pt->tx = 1;
		pt->data = p->addr_int;
		pt->nbytes = 2;
		pt++;
	}
	if(n_internal_address_bytes == 0 || !write)
	{
		// Set up second partial transaction: restart and address
		pt->start = (n_internal_address_bytes > 0) ? 0 : 1;
		pt->restart = !pt->start;
		pt->stop = 0;
		pt->tx = 1;
		p->addr = (address << 1) | (write ? 0 : 1);
		pt->data = &p->addr;
		pt->nbytes = 1;
		pt++;
	}

	// Set up third partial transaction: data and stop
	pt->start = 0;
	pt->stop = 1;
	pt->tx = (write ? 1 : 0);
	pt->data = data;
	pt->nbytes = nbytes;
	pt->last_pt = 1;

	// Start the transaction
	p->state = I2C_BEGIN;
	return 0;
}


