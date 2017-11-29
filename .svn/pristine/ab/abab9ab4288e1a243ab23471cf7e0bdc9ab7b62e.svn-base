/*
 * lib_lowpower.c
 *
 *  Created on: 2015. 5. 8.
 *      Author: Moonsik Choi
 */

#include "../itf/lib_lowpower.h"

//#include <stdio.h>
#include <sam4e.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"
#include "itf_ubinos/itf/ubinos.h"

#include "_bsp_board.h"

#include "../../lib_ubi_pio/itf/lib_ubi_pio.h"

#include "gcc/asf.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "conf_uart_serial.h"
#include "low_power_board.h"

#if !defined(PMC_PCK_PRES_CLK_1)
#define PMC_PCK_PRES_CLK_1   PMC_PCK_PRES(0)
#define PMC_PCK_PRES_CLK_2   PMC_PCK_PRES(1)
#define PMC_PCK_PRES_CLK_4   PMC_PCK_PRES(2)
#define PMC_PCK_PRES_CLK_8   PMC_PCK_PRES(3)
#define PMC_PCK_PRES_CLK_16  PMC_PCK_PRES(4)
#define PMC_PCK_PRES_CLK_32  PMC_PCK_PRES(5)
#define PMC_PCK_PRES_CLK_64  PMC_PCK_PRES(6)
#endif

//#define DEBUG

#define CLOCK_TIMEOUT	5000

#define STRING_EOL    "\r"
#define STRING_HEADER "-- Low Power Example --\r\n" \
	"-- "BOARD_NAME " --\r\n" \
	"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

#ifndef PLL_DEFAULT_MUL
#define PLL_DEFAULT_MUL  7
#endif

#ifndef PLL_DEFAULT_DIV
#define PLL_DEFAULT_DIV  1
#endif

#ifndef MCK_DEFAULT_DIV
#define MCK_DEFAULT_DIV  PMC_MCKR_PRES_CLK_4
#endif

#ifndef example_switch_clock
#define example_switch_clock(a, b, c, d) \
	do {                                 \
		pmc_enable_pllack(a, b, c);      \
		pmc_switch_mck_to_pllack(d);     \
	} while (0)
#endif

#ifndef example_disable_pll
#define example_disable_pll()  pmc_disable_pllack()
#endif

/** Current MCK in Hz */
uint32_t g_ul_current_mck;

/** Button pressed flag */
volatile uint32_t g_ul_button_pressed = 0;

/**
 * \brief Set default clock (MCK = 24MHz).
 */
static void set_default_working_clock(void) {
#if (SAMG)
	/* Switch MCK to slow clock  */
	pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);

	/*
	 * Configure PLL and switch clock.
	 * MCK = XTAL * (PLL_DEFAULT_MUL+1) / PLL_DEFAULT_DIV / MCK_DEFAULT_DIV
	 *     = 24 MHz
	 */
	example_switch_clock(PLL_DEFAULT_MUL, PLL_COUNT, PLL_DEFAULT_DIV,
			MCK_DEFAULT_DIV);
#else
//	/* Switch MCK to slow clock  */
//	pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);
//
//	/* Switch mainck to external xtal */
//	pmc_switch_mainck_to_xtal(0, BOARD_OSC_STARTUP_US);
//
//	/*
//	 * Configure PLL and switch clock.
//	 * MCK = XTAL * (PLL_DEFAULT_MUL+1) / PLL_DEFAULT_DIV / MCK_DEFAULT_DIV
//	 *     = 24 MHz
//	 */
//	example_switch_clock(PLL_DEFAULT_MUL, PLL_COUNT, PLL_DEFAULT_DIV,
//			MCK_DEFAULT_DIV);

//	lowlevel_init();

	/* Select external slow clock */
	/* 32KHz */
	unsigned int timeout = 0;
	if ((SUPC->SUPC_SR & SUPC_SR_OSCSEL) != SUPC_SR_OSCSEL_CRYST) {
		SUPC->SUPC_CR = ((unsigned int) 0xA5 << 24)
				| SUPC_CR_XTALSEL_CRYSTAL_SEL;
		ARM_DATASYNC()
		;
		while (!(SUPC->SUPC_SR & SUPC_SR_OSCSEL_CRYST)
				&& (timeout++ == CLOCK_TIMEOUT))
			;
	}

	PMC->PMC_WPMR = 0x504D4300;
	/* Initialize main oscillator */
	/* 12MHz */
	/* Start up time : 8 * 1/32K * 48 = 12ms */
	//Edit by YuJin Park
	if (!(PMC->CKGR_MOR & CKGR_MOR_MOSCSEL)) {
		PMC->CKGR_MOR = ((unsigned int) 0x37 << 16)
				| ((unsigned int) 0x08/*0x30*/<< 8) |
				CKGR_MOR_MOSCRCEN |
				CKGR_MOR_MOSCXTEN;
		ARM_DATASYNC()
		;
		while (!(PMC->PMC_SR & PMC_SR_MOSCXTS))
			;
	}

	/* Switch main clock to 3-20MHz Xtal oscillator */
	PMC->CKGR_MOR = ((unsigned int) 0x37 << 16)
			| ((unsigned int) CKGR_MOR_MOSCXTST(0x8) & (0x8 << 8))/*((unsigned int) 0x30 <<  8)*/
			|
			CKGR_MOR_MOSCRCEN |
			CKGR_MOR_MOSCXTEN |
			CKGR_MOR_MOSCSEL;
	ARM_DATASYNC()
	;
	while (!(PMC->PMC_SR & PMC_SR_MOSCSELS))
		;

	/* Switch master clock to main clock */
	ARM_DATASYNC()
	;
	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY))
		;
	/* Initialize PLLA */
	/* 12M * (7+1) / 1 = 96MHz */
	/* Lock time : 8 * 1/32K * 8 = 2ms */
	PMC->CKGR_PLLAR = ((1 << 29) | (0x7 << 16) | (0x01 << 8)/*(0x8 << 8)*/
			| (0x1 << 0));
	ARM_DATASYNC()
	;
	while (!(PMC->PMC_SR & PMC_SR_LOCKA))
		;

	/* Switch master clock to fast clock */
	/* PLLA / 2 = 48Mhz */
//	PMC->PMC_MCKR = 	PMC_MCKR_PRES_CLK_2 |
//						PMC_MCKR_CSS_MAIN_CLK;
//	ARM_DATASYNC();
//	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
//	PMC->PMC_MCKR = 	PMC_MCKR_PRES_CLK_2 |
//						PMC_MCKR_CSS_PLLA_CLK;
	PMC->PMC_MCKR = ((PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK)
			& ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
	ARM_DATASYNC()
	;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY))
		;

	PMC->PMC_MCKR = (PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK);
	ARM_DATASYNC()
	;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY))
		;

	/* Disable unused clock to save power */
	pmc_osc_disable_fastrc();
#endif
//
//	/* Save current clock */
//#if SAMG55
//	g_ul_current_mck = 48000000; /* 48MHz */
//#else
//	g_ul_current_mck = 24000000; /* 24MHz */
//#endif
}

/**
 *  Configure UART console.
 */
static void configure_console(void) {
	const usart_serial_options_t uart_serial_options = { .baudrate =
			CONF_UART_BAUDRATE,
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
	pio_configure_pin_group(CONF_UART_PIO, CONF_PINS_UART,
			CONF_PINS_UART_FLAGS);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 *  Reconfigure UART console for changed MCK and baudrate.
 */
#if SAMG55
static void reconfigure_console(uint32_t ul_mck, uint32_t ul_baudrate)
{
	sam_usart_opt_t uart_serial_options;

	uart_serial_options.baudrate = ul_baudrate,
	uart_serial_options.char_length = CONF_UART_CHAR_LENGTH,
	uart_serial_options.parity_type = US_MR_PAR_NO;
	uart_serial_options.stop_bits = CONF_UART_STOP_BITS,
	uart_serial_options.channel_mode= US_MR_CHMODE_NORMAL,
	uart_serial_options.irda_filter = 0,

	/* Configure PMC */
	flexcom_enable(CONF_FLEXCOM);
	flexcom_set_opmode(CONF_FLEXCOM, FLEXCOM_USART);

	/* Configure PIO */
	pio_configure_pin_group(CONF_UART_PIO, CONF_PINS_UART,
			CONF_PINS_UART_FLAGS);

	/* Configure UART */
	usart_init_rs232(CONF_UART, &uart_serial_options, ul_mck);
	/* Enable the receiver and transmitter. */
	usart_enable_tx(CONF_UART);
	usart_enable_rx(CONF_UART);
}
#else
static void reconfigure_console(uint32_t ul_mck, uint32_t ul_baudrate) {
	const sam_uart_opt_t uart_console_settings = { ul_mck, ul_baudrate,
			UART_MR_PAR_NO };

	/* Configure PMC */
	pmc_enable_periph_clk(CONSOLE_UART_ID);

	/* Configure PIO */
	pio_configure_pin_group(CONF_UART_PIO, CONF_PINS_UART,
			CONF_PINS_UART_FLAGS);

	/* Configure UART */
	uart_init(CONF_UART, &uart_console_settings);
}
#endif

/**
 * \brief Initialize the chip for low power test.
 */
static void init_chip(void) {
#if SAMG55
	/* Wait for the transmission done before changing clock */
	while (!usart_is_tx_empty(CONSOLE_UART)) {
	}
#else
	/* Wait for the transmission done before changing clock */
	while (!uart_is_tx_empty(CONSOLE_UART)) {
	}
#endif

	/* Disable all the peripheral clocks */
	pmc_disable_all_periph_clk();

	/* Disable brownout detector */
	supc_disable_brownout_detector(SUPC);

	/* Initialize the specific board */
	init_specific_board();
}

/**
 * \brief Change clock configuration.
 *
 * \param p_uc_str Hint string to be output on console before changing clock.
 */
static void user_change_clock(uint8_t uc_key, uint8_t *p_uc_str) {
	uint32_t ul_id;

#ifdef DEBUG
	/* Print menu */
	puts(CLOCK_LIST_MENU);

	printf("Select option is: %c\n\r\n\r", uc_key);
	if (p_uc_str) {
		puts((char const *)p_uc_str);
	}
#endif
	printf("test\r\n");
#if SAMG55
	/* Wait for the transmission done before changing clock */
	while (!usart_is_tx_empty(CONSOLE_UART)) {
	}
#else
	/* Wait for the transmission done before changing clock */
	while (!uart_is_tx_empty(CONSOLE_UART)) {
	}
#endif

	if ((uc_key >= MIN_CLOCK_FAST_RC_ITEM)
			&& (uc_key <= MAX_CLOCK_FAST_RC_ITEM)) {
		ul_id = uc_key - MIN_CLOCK_FAST_RC_ITEM;

		/* Save current clock */
		g_ul_current_mck = g_fastrc_clock_list[ul_id][0];

		/* Switch MCK to Slow clock  */
		pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);

		/* Switch mainck to fast RC */
		pmc_osc_enable_fastrc(CKGR_MOR_MOSCRCF_8_MHz);
		pmc_switch_mainck_to_fastrc(g_fastrc_clock_list[ul_id][1]);

		/* Switch MCK to mainck */
		pmc_switch_mck_to_mainck(g_fastrc_clock_list[ul_id][2]);

		/* Disable unused clock to save power */
		pmc_osc_disable_xtal(0);
		pmc_disable_pllack();

	} else if ((uc_key >= MIN_CLOCK_PLL_ITEM)
			&& (uc_key <= MAX_CLOCK_PLL_ITEM)) {
		ul_id = uc_key - MIN_CLOCK_PLL_ITEM;

		/* Save current clock */
		g_ul_current_mck = g_pll_clock_list[ul_id][0];

#if (SAMG)
		/* Switch MCK to main clock  */
		pmc_switch_mck_to_mainck(PMC_MCKR_PRES_CLK_1);
#else
		/* Switch MCK to slow clock  */
		pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);

		/* Switch mainck to external xtal */
		pmc_switch_mainck_to_xtal(0, BOARD_OSC_STARTUP_US);
#endif
		/* Configure PLL and switch clock */
		example_switch_clock(g_pll_clock_list[ul_id][1], PLL_COUNT,
				g_pll_clock_list[ul_id][2], g_pll_clock_list[ul_id][3]);

#if (!SAMG)
		/* Disable unused clock to save power */
		pmc_osc_disable_fastrc();
#endif
	} else {
		puts("Clock is not changed.\r");
	}
}

/**
 * \brief Set peripheral clock for low power mode.
 */
void init_low_power(void) {
	/* Initialize the chip for the power consumption test */
	init_chip();

	/* Re-configure UART */
	dtty_init();
}

/**
 * \brief Handler for button interrupt.
 *
 * \note This interrupt is for waking up from sleep mode or exiting from active
 * mode.
 */
static void button_handler(uint32_t ul_id, uint32_t ul_mask) {
	if (PIN_PUSHBUTTON_WAKEUP_ID == ul_id &&
	PIN_PUSHBUTTON_WAKEUP_MASK == ul_mask) {
		g_ul_button_pressed = 1;
	}
}

/**
 *  \brief Configure source for waking up sleep mode.
 */
static void sleep_wakeup_source(void) {
	ubi_pio_interrupt_config(PORTA, 15, PIN_PUSHBUTTON_WAKEUP_ATTR,
			button_handler);
}

#if (!(SAMG51 || SAMG53 || SAMG54))
/**
 * \brief sleep Mode.
 */
/**
 *  CK125KHz_RC	- 125KHz from Fast RC
 *  CK250KHz_RC	- 250KHz from Fast RC
 *  CK500KHz_RC	- 500KHz from Fast RC
 *  CK1MHz_RC	- 1MHz from Fast RC
 *  CK2MHz_RC	- 2MHz from Fast RC
 *  CK4MHz_RC	- 4MHz from Fast RC
 *  CK8MHz_RC	- 8MHz from Fast RC
 *  CK12MHz_RC	- 12MHz from Fast RC
 *  CK24MHz_PLL	- 24MHz from PLL clock
 *  CK32MHz_PLL	- 32MHz from PLL clock
 *  CK48MHz_PLL	- 48MHz from PLL clock
 *  CK64MHz_PLL	- 64MHz from PLL clock
 *  CK84MHz_PLL	- 84MHz from PLL clock
 *  CK100MHz_PLL- 100MHz from PLL clock
 *  CK120MHz_PLL- 120MHz from PLL clock
 */

void ubi_sleep_mode(uint8_t select_clock) {

	/* Configure source for waking up sleep mode */
	sleep_wakeup_source();

	/* Select clock for sleep mode */
	user_change_clock(select_clock, STRING_SLEEP);

	/* Disable UART */
	pmc_disable_periph_clk(CONSOLE_UART_ID);

	SysTick->CTRL = 0x04; //Disable Systick interrupt

	/* Enter into sleep Mode */
	pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);

	SysTick->CTRL = 0x07; //Enable Systick interrupt

	/* Set default clock and re-configure UART */
	set_default_working_clock();

	//reconfigure_console(g_ul_current_mck, CONF_UART_BAUDRATE);
	dtty_init();
#ifdef DEBUG
	puts("Exit from sleep Mode.\r");
#endif
}
#endif

/**
 * \brief wait mode.
 */
/**
 * ---------------------------------------------------
 *  Wakeup pin list
 * ---------------------------------------------------
 *  WKUP0(PA0) - LED_TIMER(Blue)
 *  WKUP1(PA1) - BUTTON_SCROLL_UP
 *  WKUP2(PA2) - BUTTON_SCROLL_DOWN
 *  WKUP3(PA4) - QTouch_SCL
 *  WKUP4(PA5) - ZIGB_SEL#, SPI FLASH_CS#
 *  WKUP5(PA8) - XOUT32
 *  WKUP6(PA9) - DBGU_ROUT
 *  WKUP7(PA11) - TOUCH_CS#
 *  WKUP8(PA14) - TOUCH_DCLK, ZIGB_SCLK, SPI FLASH_SCK
 *  WKUP9(PA19) - BUTTON_WAKU
 *  WKUP10(PA20) - BUTTON_TAMP
 *  WKUP11(PA30) - SD_DAT0
 *  WKUP12(PB2) - CAN0_D, AFE1_BNC
 *  WKUP13(PB5) - JTAG_TDO
 *  WKUP14(PA15) - ZIGB_SLP_TR
 *  WKUP15(PA16) - TOUCH_PENIRQ#, ZIGB_MISC
 *  ---------------------------------------------------
 */
void ubi_wait_mode(uint32_t pin_wkup, int startup, uint8_t RTTAL_wkup) {

	#ifdef DEBUG
	puts(STRING_WAIT);
#endif

#if SAMG55
	/* Wait for the transmission done before changing clock */
	while (!usart_is_tx_empty(CONSOLE_UART)) {
	}
#else
	/* Wait for the transmission done before changing clock */
	while (!uart_is_tx_empty(CONSOLE_UART)) {
	}
#endif

	/* Set wakeup input for fast startup */
	switch (RTTAL_wkup) {
	case UBIK_SAM_RTT_WAKEUP:
		pmc_set_fast_startup_input((1u << 16));
		break;
	case UBIK_SAM_PIN_WAKEUP:
		if (0 != pin_wkup)
			pmc_set_fast_startup_input(pin_wkup);
		else
			logme("no pin_wakeup\r\n");
		break;

	case (UBIK_SAM_RTT_WAKEUP | UBIK_SAM_PIN_WAKEUP):
		if (0 != pin_wkup)
			pmc_set_fast_startup_input(pin_wkup | (1u << 16));
		else
			logme("no pin_wakeup\r\n");
		break;
	default:
		break;
	}
//	if(RTTAL_wkup)
//		pmc_set_fast_startup_input(pin_wkup|(1u<<16));
//	else
//		pmc_set_fast_startup_input(pin_wkup);

	switch (startup) {
	case WAIT_NORMAL:
		/* Enter into deep power-down wait Mode */
		pmc_sleep(SAM_PM_SMODE_WAIT);
		break;

	case WAIT_STARTUP_FAST:
		/* Enter into startup fast wait Mode */
		pmc_sleep(SAM_PM_SMODE_WAIT_FAST);
		break;

	default:
#ifdef DEBUG
		puts("error startup mode.\r");
#endif
		return;
		break;
	}
#ifdef DEBUG
	puts("Exit from wait Mode.\r");
#endif

	dtty_init();
}

#if (!(SAMG51 || SAMG53 || SAMG54))
/**
 * \brief Test backup mode.
 *
 * \note To test backup mode, the program must run out of flash.
 * ---------------------------------------------------
 *  Wakeup pin list
 * ---------------------------------------------------
 *  PIN_WKUP_0(PA0) - LED_TIMER(Blue)
 *  PIN_WKUP_1(PA1) - BUTTON_SCROLL_UP
 *  PIN_WKUP_2(PA2) - BUTTON_SCROLL_DOWN
 *  PIN_WKUP_3(PA4) - QTouch_SCL
 *  PIN_WKUP_4(PA5) - ZIGB_SEL#, SPI FLASH_CS#
 *  PIN_WKUP_5(PA8) - XOUT32
 *  PIN_WKUP_6(PA9) - DBGU_ROUT
 *  PIN_WKUP_7(PA11) - TOUCH_CS#
 *  PIN_WKUP_8(PA14) - TOUCH_DCLK, ZIGB_SCLK, SPI FLASH_SCK
 *  PIN_WKUP_9(PA19) - BUTTON_WAKU
 *  PIN_WKUP_10(PA20) - BUTTON_TAMP
 *  PIN_WKUP_11(PA30) - SD_DAT0
 *  PIN_WKUP_12(PB2) - CAN0_D, AFE1_BNC
 *  PIN_WKUP_13(PB5) - JTAG_TDO
 *  PIN_WKUP_14(PA15) - ZIGB_SLP_TR
 *  PIN_WKUP_15(PA16) - TOUCH_PENIRQ#, ZIGB_MISC
 * ---------------------------------------------------
 * \param pin_transition Bitmask of level transition of the wake-up inputs.
 * HIGH_TO_LOW means a high-to-low level transition forces the wake up of core power supply.
 * LOW_TO_HIGH means a low-to-high level transition forces the wake up of core power supply.
 */
void ubi_backup_mode(uint32_t pin_wkup, int pin_transition) {
#ifdef DEBUG
	printf("Backup Mode(Entered %d times).\n\r", (int)gpbr_read(GPBR0));
	puts(STRING_BACKUP);
#endif

#if SAMG55
	/* Wait for the transmission done before changing clock */
	while (!usart_is_tx_empty(CONSOLE_UART)) {
	}
#else
	/* Wait for the transmission done before changing clock */
	while (!uart_is_tx_empty(CONSOLE_UART)) {
	}
#endif

#ifdef DEBUG
	/* GPBR0 is for recording times of entering into backup mode */
	gpbr_write(GPBR0, gpbr_read(GPBR0) + 1);
#endif
	/* Enable the PIO for wake-up */
	switch (pin_transition) {
	case HIGH_TO_LOW:
		supc_set_wakeup_inputs(SUPC, pin_wkup, pin_wkup);
		break;

	case LOW_TO_HIGH:
		supc_set_wakeup_inputs(SUPC, pin_wkup, (~(pin_wkup) & pin_wkup));
		break;

	default:
#ifdef DEBUG
		puts("error pin_transition mode.\r");
#endif
		return;
		break;
	}

	/* Switch MCK to slow clock  */
	pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);

	/* Disable unused clock to save power */
	pmc_osc_disable_xtal(0);
	pmc_disable_pllack();

	/* Enter into backup mode */
	pmc_sleep(SAM_PM_SMODE_BACKUP);

	/* Note: The core will reset when exiting from backup mode. */
}
#endif

