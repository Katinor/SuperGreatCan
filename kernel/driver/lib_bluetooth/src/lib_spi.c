/*
 * spi.c
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
#include "itf_ubinos/itf/ubik_mutex.h"

// chipset driver include
#include "lib_ubi_pio/itf/lib_ubi_pio.h"
#include "ioport.h"
#include "pio/pio.h"

// new estk driver include
#include "lib_new_estk_api/itf/new_estk_led.h"
#include "lib_new_estk_api/itf/new_estk_glcd.h"

#include "../kernel/porting/lib_sam4e/sam/utils/cmsis/sam4e/include/sam4e16e.h"
#include "../kernel/porting/lib_sam4e/sam/boards/sam4e_ek/sam4e_ek.h"

#include "common/services/gpio/sam_gpio/sam_gpio.h"
#include "spi/spi.h"
#include "lib_new_estk_api/itf/new_estk_ioport_set.h"
#include "common/services/ioport/sam/ioport_pio.h"
#include "common/services/spi/sam_spi/spi_master.h"

//new estk ble drive include
#include "lib_bluetooth/itf/lib_BT.h"
#include "lib_bluetooth/itf/BT_Module_Interface.h"
//#include "../../../../interface/itf_ubinos/itf/ubik_mutex.h"

extern mutex_pt _g_mutex;

void spi_init() {

	ioport_set_pin_peripheral_mode(SPI_MISO_GPIO, SPI_MISO_FLAGS);
	ioport_set_pin_peripheral_mode(SPI_MOSI_GPIO, SPI_MOSI_FLAGS);
	ioport_set_pin_peripheral_mode(SPI_SPCK_GPIO, SPI_SPCK_FLAGS);

	spi_enable_clock(SPI_MASTER_BASE);
	spi_disable(SPI_MASTER_BASE);
	spi_reset(SPI_MASTER_BASE);
	spi_set_master_mode(SPI_MASTER_BASE);
	spi_disable_mode_fault_detect(SPI_MASTER_BASE);
	spi_disable_loopback(SPI_MASTER_BASE);
	spi_set_peripheral_chip_select_value(SPI_MASTER_BASE, SPI_CHIP_SEL);
	spi_set_fixed_peripheral_select(SPI_MASTER_BASE);

	spi_set_transfer_delay(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_DLYBS,
	SPI_DLYBCT);
	spi_set_bits_per_transfer(SPI_MASTER_BASE, SPI_CHIP_SEL,
	SPI_CSR_BITS_8_BIT);
	spi_set_baudrate_div(SPI_MASTER_BASE, SPI_CHIP_SEL, 12);
	spi_set_clock_polarity(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_POLARITY);
	spi_set_clock_phase(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_PHASE);

	spi_enable(SPI_MASTER_BASE);
	printf("init! \r\n");

}

void SPI_WRITE_DATA(uint8_t *data, uint32_t len) {

	gpio_set_pin_low(CS);
	mutex_lock(_g_mutex);
	spi_write_packet(SPI_MASTER_BASE, data, len);
	mutex_unlock(_g_mutex);
	gpio_set_pin_high(CS);

	printf("W : ");

	for (int i = 0; i < 20; i++)
		printf("%x ", data[i]);
	printf("\r\n");

}

void SPI_READ_DATA(uint8_t *data, uint32_t len) {

	gpio_set_pin_low(CS);
	mutex_lock(_g_mutex);
	spi_read_packet(SPI_MASTER_BASE, data, len);
	mutex_unlock(_g_mutex);
	gpio_set_pin_high(CS);

	printf("R : ");
	for (int i = 0; i < 20; i++)
		printf("%x ", data[i]);
	printf("\r\n");

}
