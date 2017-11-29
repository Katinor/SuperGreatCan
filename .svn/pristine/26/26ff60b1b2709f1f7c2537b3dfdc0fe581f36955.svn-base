/*
 * lib_lowpower.h
 *
 *  Created on: 2015. 5. 8.
 *      Author: Moonsik Choi
 */

#ifndef APPLICATION_LIB_LOWPOWER_H_
#define APPLICATION_LIB_LOWPOWER_H_


//#include <stdio.h>
#include <stdint.h>
#include "compiler.h"


/* Energy Mode */
#define	UBIK_SAM_BACKUPMODE		0
#define UBIK_SAM_WAITMODE		1
#define	UBIK_SAM_SLEEPMODE		2
#define UBIK_SAM_ACTIVEMODE		3

/**
 *  Clock setting for sleep mode.
 */
#define CK125KHz_RC		'1' //125KHz from Fast RC
#define CK250KHz_RC		'2' //250KHz from Fast RC
#define CK500KHz_RC		'3' //500KHz from Fast RC
#define CK1MHz_RC		'4' //1MHz from Fast RC
#define CK2MHz_RC		'5' //2MHz from Fast RC
#define CK4MHz_RC		'6' //4MHz from Fast RC
#define CK8MHz_RC		'7' //8MHz from Fast RC
#define CK12MHz_RC		'8' //12MHz from Fast RC
#define CK24MHz_PLL		'a' //24MHz from PLL clock
#define CK32MHz_PLL		'b' //32MHz from PLL clock
#define CK48MHz_PLL		'c' //48MHz from PLL clock
#define CK64MHz_PLL		'd' //64MHz from PLL clock
#define CK84MHz_PLL		'e' //84MHz from PLL clock
#define CK100MHz_PLL	'f' //100MHz from PLL clock
#define CK120MHz_PLL	'g' //120MHz from PLL clock

/**
 *  Startup setting for wait mode.
 */
#define WAIT_NORMAL			0 /* Deep_powerdown */
#define WAIT_STARTUP_FAST	1 /* Standby_powerdown */

/**
 *  Wakeup pin ID.
 */
#define PIN_WKUP_0	(1u << 0)
#define PIN_WKUP_1	(1u << 1)
#define PIN_WKUP_2	(1u << 2)
#define PIN_WKUP_3	(1u << 3)
#define PIN_WKUP_4	(1u << 4)
#define PIN_WKUP_5	(1u << 5)
#define PIN_WKUP_6	(1u << 6)
#define PIN_WKUP_7	(1u << 7)
#define PIN_WKUP_8	(1u << 8)
#define PIN_WKUP_9	(1u << 9)
#define PIN_WKUP_10	(1u << 10)
#define PIN_WKUP_11	(1u << 11)
#define PIN_WKUP_12	(1u << 12)
#define PIN_WKUP_13	(1u << 13)
#define PIN_WKUP_14	(1u << 14)
#define PIN_WKUP_15	(1u << 15)

/**
 *  Pin level transition option (Backup mode)
 */
#define HIGH_TO_LOW		0 //high-to-low level transition forces the wake up of core power supply.
#define LOW_TO_HIGH		1 //low-to-high level transition forces the wake up of core power supply.



void Power_Manager_Init(void);

/**
 * \brief Set peripheral clock for low power mode.
 */
void init_low_power(void);

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
void ubi_sleep_mode(uint8_t select_clock);

/**
 * \brief wait mode.
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
 *  WAIT_NORMAL - Deep power down
 *  WAIT_STARTUP_FAST - Standby power down
 */
#define UBIK_SAM_RTT_WAKEUP	(1<<1)
#define	UBIK_SAM_PIN_WAKEUP (1<<2)
void ubi_wait_mode(uint32_t pin_wkup, int startup, uint8_t RTTAL_wkup);

/**
 * \brief backup mode.
 *
 * \note To backup mode, the program must run out of flash.
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
void ubi_backup_mode(uint32_t pin_wkup, int pin_transition);


#endif /* APPLICATION_LIB_LOWPOWER_H_ */
