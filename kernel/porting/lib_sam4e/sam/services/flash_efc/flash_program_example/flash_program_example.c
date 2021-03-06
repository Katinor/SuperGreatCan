/**
 * \file
 *
 * \brief Flash program example for SAM.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage Flash Program Example
 *
 * \section Purpose
 *
 * This basic example shows how to use the Flash service available on the Atmel
 * SAM microcontrollers. It details steps required to program the internal
 * flash, and manage secure and lock bits.
 *
 * \section Requirements
 *
 * This package can be used with SAM evaluation kits.
 *
 * \section Description
 *
 * The program performs the following set of commands:
 * - Unlock the test page.
 * - Program the test page of the embedded flash with walking bit pattern (0x1,
 * 0x2, 0x4, ...).
 * - Check if the flash is correctly programmed by reading all the values
 * programmed.
 * - Lock the test page and check if it has been locked correctly.
 * - Set the security bit.
 *
 * The SAM MCU features a security bit, based on a specific General Purpose
 * NVM bit 0. When the security bit is enabled, any access to the Flash, SRAM,
 * Core Registers and Internal Peripherals through the ICE interface is
 * forbidden.
 * This example will reproduce this scene.
 *
 * The SAM MCU ROM code embeds small In-Application Programming (IAP) Procedure.
 * Since this function is executed from ROM, this allows Flash programming
 * (such as sector write) to be done when code is running out of Flash.
 * We will use IAP function by default in flash driver.
 *
 * \section Usage
 *
 * -# Build the program and download it into the evaluation board.
 * -# On the computer, open and configure a terminal application
 *    (e.g., HyperTerminal on Microsoft Windows) with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 *    \code
	-- Flash Program Example --
	-- xxxxxx-xx
	-- Compiled: xxx xx xxxx xx:xx:xx --
	-I- Unlocking test page
	-I- Writing test page with walking bit pattern
	-I- Checking page contents  ......................................... ok
	-I- Locking test page
	-I- Try to program the locked page...
	-I- Please open Segger's JMem program
	-I- Read memory at address 0xxxxxxxxx to check contents
	-I- Press any key to continue...
	-I- Good job!
	-I- Now set the security bit
	-I- Press any key to continue to see what happened...
	-I- Setting security bit
	-I- All tests done
\endcode
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "conf_example.h"

#define STRING_EOL    "\r"
#define STRING_HEADER "-- Flash Program Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/**
 *  Configure console using UART.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

typedef unsigned long UL;

/**
 * Perform initialization and tests on flash.
 */
int main(void)
{
	uint32_t ul_test_page_addr = TEST_PAGE_ADDRESS;
	uint32_t *pul_test_page = (uint32_t *) ul_test_page_addr;
	uint32_t ul_rc;
	uint32_t ul_idx;
	uint8_t uc_key;
	uint32_t ul_page_buffer[IFLASH_PAGE_SIZE / sizeof(uint32_t)];

	/* Initialize the SAM system */
	sysclk_init();
	board_init();

	/* Initialize the console uart */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	/* Initialize flash: 6 wait states for flash writing. */
	ul_rc = flash_init(FLASH_ACCESS_MODE_128, 6);
	if (ul_rc != FLASH_RC_OK) {
		printf("-F- Initialization error %lu\n\r", (UL)ul_rc);
		return 0;
	}

	/* Unlock page */
	printf("-I- Unlocking test page: 0x%08x\r\n", ul_test_page_addr);
	ul_rc = flash_unlock(ul_test_page_addr,
			ul_test_page_addr + IFLASH_PAGE_SIZE - 1, 0, 0);
	if (ul_rc != FLASH_RC_OK) {
		printf("-F- Unlock error %lu\n\r", (UL)ul_rc);
		return 0;
	}

	/* Write page */
	printf("-I- Writing test page with walking bit pattern\n\r");
	for (ul_idx = 0; ul_idx < (IFLASH_PAGE_SIZE / 4); ul_idx++) {
		ul_page_buffer[ul_idx] = 1 << (ul_idx % 32);
	}

#if (SAM4S || SAM4E || SAM4N || SAM4C || SAM4CP || SAMG || SAM4CM)
	/* The EWP command is not supported for non-8KByte sectors in all devices
	 *  SAM4 series, so an erase command is requried before the write operation.
	 */
	ul_rc = flash_erase_sector(ul_test_page_addr);
	if (ul_rc != FLASH_RC_OK) {
		printf("-F- Flash programming error %lu\n\r", (UL)ul_rc);
		return 0;
	}

	ul_rc = flash_write(ul_test_page_addr, ul_page_buffer,
			IFLASH_PAGE_SIZE, 0);
#else
	ul_rc = flash_write(ul_test_page_addr, ul_page_buffer,
			IFLASH_PAGE_SIZE, 1);
#endif
	if (ul_rc != FLASH_RC_OK) {
		printf("-F- Flash programming error %lu\n\r", (UL)ul_rc);
		return 0;
	}

	/* Validate page */
	printf("-I- Checking page contents ");
	for (ul_idx = 0; ul_idx < (IFLASH_PAGE_SIZE / 4); ul_idx++) {
		printf(".");
		if (pul_test_page[ul_idx] != ul_page_buffer[ul_idx]) {
			printf("\n\r-F- data error\n\r");
			return 0;
		}
	}
	printf("OK\n\r");

#if (SAM4S || SAM4E || SAM4N || SAM4C || SAM4CP || SAMG || SAM4CM)
	/* The EWP command is not supported for non-8KByte sectors in some SAM4
	 * series, so an erase command is requried before the write operation.
	 */
	ul_rc = flash_erase_sector(ul_test_page_addr);
	if (ul_rc != FLASH_RC_OK) {
		printf("-F- Flash programming error %lu\n\r", (UL)ul_rc);
		return 0;
	}
#endif

	/* Lock page */
	printf("-I- Locking test page\n\r");
	ul_rc = flash_lock(ul_test_page_addr,
			ul_test_page_addr + IFLASH_PAGE_SIZE - 1, 0, 0);
	if (ul_rc != FLASH_RC_OK) {
		printf("-F- Flash locking error %lu\n\r", (UL)ul_rc);
		return 0;
	}

	/* Check if the associated region is locked. */
	printf("-I- Try to program the locked page ...\n\r");
	ul_rc = flash_write(ul_test_page_addr, ul_page_buffer,
			IFLASH_PAGE_SIZE,
#if (SAM4S || SAM4E || SAM4N || SAM4C || SAMG || SAM4CP || SAM4CM)
			0);
#else
			1);
#endif
	if (ul_rc != FLASH_RC_OK) {
		printf("-I- The page to be programmed belongs to locked region. Error %lu\n\r",
				(UL)ul_rc);
	}

	printf("-I- Please open Segger's JMem program \n\r");
	printf("-I- Read memory at address 0x%08lx to check contents\n\r",
			(UL)ul_test_page_addr);
	printf("-I- Press any key to continue...\n\r");
	scanf("%c", (char *)&uc_key);

	printf("-I- Good job!\n\r"
			"-I- Now set the security bit \n\r"
			"-I- Press any key to continue to see what happened...\n\r");
	scanf("%c", (char *)&uc_key);

	/* Set security bit */
	printf("-I- Setting security bit \n\r");
	ul_rc = flash_enable_security_bit();
	if (ul_rc != FLASH_RC_OK) {
		printf("-F- Set security bit error %lu\n\r", (UL)ul_rc);
	}

	printf("-I- All tests done\n\r");

	while (1) {
		/* Do nothing */
	}
}
