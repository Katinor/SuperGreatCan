/*
 * lib_i2c_master.c
 *
 *  Created on: 2015. 4. 20.
 *      Author: geoyun
 */

#include "sam4e.h"
#include "ioport.h"
#include "../itf/lib_i2c_master.h"
#include "../itf/lib_sam4e16e_board.h"
//#include "twi.h"
#include "twi/twi.h"



/** TWI0 pins definition */
#define TWI0_DATA_GPIO   PIO_PA3_IDX
#define TWI0_DATA_FLAGS  (IOPORT_MODE_MUX_A)
#define TWI0_CLK_GPIO    PIO_PA4_IDX
#define TWI0_CLK_FLAGS   (IOPORT_MODE_MUX_A)

/** TWI1 pins definition */
#define TWI1_DATA_GPIO   PIO_PB4_IDX
#define TWI1_DATA_FLAGS  (IOPORT_MODE_MUX_A)
#define TWI1_CLK_GPIO    PIO_PB5_IDX
#define TWI1_CLK_FLAGS   (IOPORT_MODE_MUX_A)


struct _ubi_twi_state{
	Twi *pTwi;
	uint8_t init_flag;

}typedef ubi_twi_state;

ubi_twi_state ubi_twi0 = {TWI0, FALSE};
ubi_twi_state ubi_twi1 = {TWI1, FALSE};


#define	DEFAULT_I2C_SPEED	100000

static int __ubi_twim_basic_init(Twi *pTwi, uint32_t pSpeed);
static int __ubi_twim_basic_write_bytes(Twi *pTwi, uint8_t device_address, uint8_t register_address, uint8_t address_data_size, uint8_t *write_buffer, uint32_t plength);
static int __ubi_twim_basic_read_bytes(Twi *pTwi, uint8_t device_address, uint8_t register_address, uint8_t address_data_size, uint8_t *write_buffer, uint32_t plength);


int ubi_twim_init(uint8_t i2c_port, uint32_t pSpeed)
{
	int result = UBI_I2C_SUCCESS;
	uint32_t speed;

	/* if speed value is 0, set default speed */
	if (pSpeed == 0)
		speed = DEFAULT_I2C_SPEED;
	else
		speed = pSpeed;

	/* peripheral io init */
	sam4e16e_board_init();

	switch (i2c_port) {
		case TWI_PORT_0:

			if(ubi_twi0.init_flag != TRUE)
			{
				sysclk_enable_peripheral_clock(ID_TWI0);

				ioport_set_pin_peripheral_mode(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
				ioport_set_pin_peripheral_mode(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);

				result = __ubi_twim_basic_init(ubi_twi0.pTwi, speed);
				if (result != UBI_I2C_SUCCESS)
				{
					ubi_twi0.init_flag = FALSE;
				}
				else
					ubi_twi0.init_flag = TRUE;
			}
			else
				result = ALREADY_INIT;
			break;

		case TWI_PORT_1:

			if(ubi_twi1.init_flag != TRUE)
			{
				sysclk_enable_peripheral_clock(ID_TWI1);

				ioport_set_pin_peripheral_mode(TWI1_DATA_GPIO, TWI1_DATA_FLAGS);
				ioport_set_pin_peripheral_mode(TWI1_CLK_GPIO, TWI1_CLK_FLAGS);

				result = __ubi_twim_basic_init(ubi_twi1.pTwi, speed);
				if (result != UBI_I2C_SUCCESS)
				{
					ubi_twi1.init_flag = FALSE;
				}
				else
					ubi_twi1.init_flag = TRUE;
			}
			else
				result = ALREADY_INIT;
			break;

		default:
			break;
	}
	return result;
}

/*
 * uint8_t i2c_port
 * uint8_t device_address
 * uint8_t register_address
 * uint8_t address_data_size
 * uint8_t *write_buffer
 * uint32_t plength
 * */
int ubi_twim_write_bytes(uint8_t i2c_port, uint8_t device_address, uint8_t register_address, uint8_t address_data_size, uint8_t *write_buffer, uint32_t plength)
{
	int result = UBI_I2C_SUCCESS;

	switch (i2c_port) {
		case TWI_PORT_0:
			if(ubi_twi0.init_flag != FALSE)
			{
				result = __ubi_twim_basic_write_bytes(ubi_twi0.pTwi, device_address, register_address, address_data_size, write_buffer, plength);
			}
			else
				result = NOT_INIT;
			break;
		case TWI_PORT_1:
			if(ubi_twi1.init_flag != FALSE)
			{
				result = __ubi_twim_basic_write_bytes(ubi_twi1.pTwi, device_address, register_address, address_data_size, write_buffer, plength);
			}
			else
				result = NOT_INIT;
			break;
		default:
			break;
	}
	return result;
}

int ubi_twim_read_bytes(uint8_t i2c_port, uint8_t device_address, uint8_t register_address, uint8_t address_data_size, uint8_t *read_buffer, uint32_t plength)
{
	int result = UBI_I2C_SUCCESS;

	switch (i2c_port) {
		case TWI_PORT_0:
			if(ubi_twi0.init_flag != FALSE)
			{
				result = __ubi_twim_basic_read_bytes(ubi_twi0.pTwi, device_address, register_address, address_data_size, read_buffer, plength);
			}
			else
				result = NOT_INIT;
			break;
		case TWI_PORT_1:
			if(ubi_twi1.init_flag != FALSE)
			{
				result = __ubi_twim_basic_read_bytes(ubi_twi1.pTwi, device_address, register_address, address_data_size, read_buffer, plength);
			}
			else
				result = NOT_INIT;
			break;
		default:
			break;
	}
	return result;
}



/*************************************************************************************************/
static twi_options_t _ubi_option;
/*
 * i2c init function
 * */
static int __ubi_twim_basic_init(Twi *pTwi, uint32_t pSpeed)
{


	memset((void *)&_ubi_option, 0, sizeof(twi_options_t));

	_ubi_option.master_clk = sysclk_get_cpu_hz();
	_ubi_option.master_clk = 48000000;
	_ubi_option.smbus      = 0;
	_ubi_option.speed = pSpeed;		// 100KHz for I2C speed

	return twi_master_init(pTwi, &_ubi_option);
}

static int __ubi_twim_basic_write_bytes(Twi *pTwi, uint8_t device_address, uint8_t register_address, uint8_t address_data_size, uint8_t *write_buffer, uint32_t plength)
{
//	bsp_busywaitms(1);	// need delay about 50us
	bsp_busywaitus(50);

	twi_packet_t packet_wr = {
			.addr[0] 		= register_address,
			.addr_length 	= sizeof(uint8_t) * address_data_size,
			.chip			= device_address,
			.buffer			= (void *)write_buffer,
			.length			= plength
	};

	return twi_master_write(pTwi, &packet_wr);
}

static int __ubi_twim_basic_read_bytes(Twi *pTwi, uint8_t device_address, uint8_t register_address, uint8_t address_data_size, uint8_t *write_buffer, uint32_t plength)
{
//	bsp_busywaitms(1);	// need delay about 50us
	bsp_busywaitus(50);

	twi_packet_t packet_wr = {
			.addr[0] 		= register_address,
			.addr_length 	= sizeof(uint8_t) * address_data_size,
			.chip			= device_address,
			.buffer			= (void *)write_buffer,
			.length			= plength
	};

	return twi_master_read(pTwi, &packet_wr);
}

//twi_package_t packet_wr = {
//	.addr[0]      = reg_addr,       /* TWI slave memory address */
//	.addr_length  = sizeof(uint8_t),/* TWI slave memory address data size */
//	.chip         = BOARD_QT_DEVICE_ADDRESS, /* TWI slave bus address */
//	.buffer       = (void *)write_buffer,/* Transfer data buffer */
//	.length       = length          /* Transfer data size (bytes) */
//};
