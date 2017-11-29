#ifndef PROPERTY_PROLOGUE_H__lib_at86rf
#define PROPERTY_PROLOGUE_H__lib_at86rf

#ifdef	__cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* HIGHEST_STACK_LAYER */
#define PAL                             (1)
#define TINY_TAL                        (2)
#define TAL                             (3)
#define MAC                             (4)
#define NWK                             (5)


/*****************************************************************************/
/* TAL_TYPE */
#define AT86RF230A              		(0x01)
#define AT86RF230B              		(0x02)
#define AT86RF231               		(0x11)
#define AT86RF212               		(0x21)
	/* TAL Type for Mega RF single chips, e.g. ATMEGA128RFA1 */
#define ATMEGARF_TAL_1          		(0x31)
	/*  */
#define AT86RF232               		(0x41)


/*****************************************************************************/
/* SAL_TYPE (security) */
	/** Dummy SAL type */
#define NO_SAL                          (0x00)
	/** SAL with transceiver based AES via SPI */
#define AT86RF2xx                       (0x01)
	/** SAL with single chip transceiver based AES */
#define ATMEGARF_SAL                    (0x02)
	/** AES software implementation */
#define SW_AES_SAL                      (0x03)
	/** SAL with ATxmega family based AES */
#define ATXMEGA_SAL                     (0x04)


/*****************************************************************************/
/* PAL_GENERIC_TYPE */
#define AVR                             (0x01)
#define MEGA_RF                         (0x02)
#define XMEGA                           (0x03)
#define AVR32                           (0x04)
#define ARM7                            (0x05)
#define ARM9                            (0x06)
#define SAM3S                           (0x07)


/*****************************************************************************/
/* PAL_TYPE */
	/* for AVR 8-bit MCUs */
#define AT90USB1287                 	(0x01)
#define ATMEGA1281                  	(0x02)
#define ATMEGA1284P                 	(0x03)
#define ATMEGA2561                  	(0x04)
#define ATMEGA644P                  	(0x05)
	/* for XMEGA MCUs */
#define ATXMEGA128A1                	(0x01)
#define ATXMEGA256A3                	(0x02)
	/* for ARM7 MCUs */
#define AT91SAM7X256                	(0x01)
#define AT91SAM7XC256               	(0x02)
	/* for ARM9 MCUs */
#define AT91SAM9XE512   	            (0x01)
	/* for Cortex-m3 MCUs */
#define AT91SAM3S 	               		(0x01)
	/* for MEGA-RF single chips (MCU plus transceiver) */
#define ATMEGA128RFA1               	(0x01)


/*****************************************************************************/
/* BOARD_TYPE */

/*************************************/
/* ARM7: AT91SAM7X-EK, AT91SAM7XC-EK */

/* Boards for AT86RF230B */
/*
 * AT91SAM7X-EK boards with Radio Extender board REB230B V2.3 on REBtoSAM7EK adapter
 * http://www.dresden-elektronik.de/shop/prod72.html
 * http://www.dresden-elektronik.de/shop/prod90.html
 */
#define REB_2_3_REB_TO_SAM7EK       (0x01)
/* AT91SAM7X-EK, AT91SAM7XC-EK boards with Radio Extender board REB230B V2.3 on REX_ARM adapter Revision 2 */
#define REB_2_3_REX_ARM_REV_2       (0x02)

/* Boards for AT86RF231 */
/*
 * AT91SAM7X-EK boards with Radio Extender board REB231 V4.0.1/V4.0.2 on REBtoSAM7EK adapter
 * http://www.dresden-elektronik.de/shop/prod73.html
 * http://www.dresden-elektronik.de/shop/prod90.html
 */
#define REB_4_0_2_REB_TO_SAM7EK     (0x11)
/* AT91SAM7X-EK boards with Radio Extender board REB231 V4.0.1/4.0.2 on REX_ARM adapter Revision 3 */
#define REB_4_0_2_REX_ARM_REV_3     (0x12)
/* ESPS boards with Radio Extender board REB231 V4.0.1/4.0.2 on REX_ARM adapter Revision 3 */
#define REB_4_0_2_REX_ARM_ESPS_REV_1	(0xA1)
/* SMART boards with Radio Extender board REB231 V4.0.1/4.0.2 on REX_ARM adapter Revision 3 */
#define REB_4_0_2_REX_ARM_SMART_REV_1	(0xB1)
/* Cortex-m3 boards with Radio Extender board REB231 V4.0.1/4.0.2 on REX_ARM adapter Revision 3 */
#define REB_4_0_2_REX_ARM_SAM3S_REV_1	(0xC1)


/* Boards for AT86RF212 */
/*
 * AT91SAM7X-EK boards with Radio Extender board REB212 V5.0.2 on REBtoSAM7EK adapter
 * http://www.dresden-elektronik.de/shop/prod79.html
 * http://www.dresden-elektronik.de/shop/prod90.html
 */
#define REB_5_0_REB_TO_SAM7EK       (0x21)
/* AT91SAM7X-EK boards with Radio Extender board REB212 V5.0.2 on REX_ARM adapter Revision 3 */
#define REB_5_0_REX_ARM_REV_3       (0x22)


/*************************************/
/* MEGA_RF: ATMEGA128RFA1 */

/* Boards for ATMEGA128RFA1 */
/* Atmel internal */
#define EMURFA1                 (0x01)
/* Atmel internal */
#define EVALRFA1                (0x02)
/*
 * Plain Radio Controller board RCB V6.3 with ATmega128RFA1
 */
#define RCB_6_3_PLAIN           (0x03)
/*
 * STK541 USB Adapter board with Radio Controller board RCB V6.3 with ATmega128RFA1
 */
#define RCB_6_3_STK541          (0x04)
/*
 * Radio Controller board RCB V6.3 connected to Key Remote Controller Board
 */
#define RCB_6_3_KEY_RC          (0x05)
/*
 * RCB Breakout Board with Radio Controller board RCB 6.3
 * http://www.dresden-elektronik.de/shop/prod85.html
 *
 * RCB Breakout Board Light with Radio Controller board RCB 6.3
 * http://www.dresden-elektronik.de/shop/prod84.html
 */
#define RCB_6_3_BREAKOUT_BOARD  (0x06)
/*
 * Sensor Terminal board with Radio Controller board RCB 6.3
 * http://www.dresden-elektronik.de/shop/prod75.html
 */
#define RCB_6_3_SENS_TERM_BOARD (0x07)
/*
 * ATmega128RFA1-EK1 Evaluation Kit
 * http://www.atmel.com/dyn/products/tools_card_mcu.asp?tool_id=4677
 */
#define EK1                     (0x11)


/*************************************/
/* XMEGA: ATMEGA128RFA1, ATXMEGA256A3 */

/* Boards for AT86RF230B */
/* STK600 board with
 * - REB to STK600 Adapter
 * - Radio Extender board REB230B V2.3
 */
#define REB_2_3_STK600              (0x01)

/* Boards for AT86RF231 */
/* STK600 board with
 * - REB to STK600 Adapter
 * - Radio Extender board REB231 V4.0.1
 */
#define REB_4_0_STK600              (0x11)

/* STK600 board with
 * - REB to STK600 Adapter
 * - Radio Extender board REB231 V4.0.1
 * CLKM from transceiver is used as timer source
 */
#define REB_4_0_STK600_USING_CLKM   (0x12)

/* STK600 board with
 * - REB to STK600 Adapter
 * - Radio Extender board REB231ED V4.1.1
 */
#define REB_4_1_STK600              (0x13)

/* Boards for AT86RF212 */
/* STK600 board with
 * - REB to STK600 Adapter
 * - Radio Extender board REB212 V5.0.2
 */
#define REB_5_0_STK600              (0x21)


/*************************************/
/* AVR: AT90USB1287, ATMEGA1281, ATMEGA2561, ATMEGA644P */

/*
 * Boards for AT86RF230A
 */
/* STK541 USB Adapter board with Radio Controller board RCB230 V3.1 */
#define RCB_3_1_230A            (0x01)
/* STK500 & STK501 boards with Radio Extender board REB230A V2.2 */
#define REB_2_2_230A            (0x02)

/*
 * Boards for AT86RF230B
 */
#define USBSTICK_C              (0x01)
/*
 * RCB Breakout Board with Radio Controller board RCB230 3.2
 * http://www.dresden-elektronik.de/shop/prod85.html
 *
 * RCB Breakout Board Light with Radio Controller board RCB230 3.2
 * http://www.dresden-elektronik.de/shop/prod84.html
 */
#define RCB_3_2_BREAKOUT_BOARD  (0x11)
/*
 * Plain Radio Controller board RCB230 V3.2
 */
#define RCB_3_2_PLAIN           (0x12)
/*
 * Sensor Terminal board with Radio Controller board RCB230 3.2
 * http://www.dresden-elektronik.de/shop/prod75.html
 */
#define RCB_3_2_SENS_TERM_BOARD (0x13)
/* STK541 USB Adapter board with Radio Controller board RCB230 V3.2 */
#define RCB_3_2_STK541          (0x14)
/* STK500 & STK501 boards with Radio Extender board REB230B V2.3 */
#define REB_2_3_STK500_STK501   (0x15)
/* STK500 board with Radio Extender board REB230B V2.3 */
#define REB_2_3_STK500          (0x11)


/*
 * Boards for AT86RF231
 */
/*
 * RCB Breakout Board with Radio Controller board RCB231 4.0
 * http://www.dresden-elektronik.de/shop/prod85.html
 *
 * RCB Breakout Board Light with Radio Controller board RCB231 4.0
 * http://www.dresden-elektronik.de/shop/prod84.html
 */
#define RCB_4_0_BREAKOUT_BOARD  (0x21)
/*
 * Remote Control Demonstration platform using push button keys.
 */
#define RCB_4_0_KEY_RC          (0x22)
/*
 * Plain Radio Controller board RCB231 V4.0
 */
#define RCB_4_0_PLAIN           (0x23)
/*
 * Sensor Terminal board with Radio Controller board RCB231 4.0
 * http://www.dresden-elektronik.de/shop/prod75.html
 */
#define RCB_4_0_SENS_TERM_BOARD (0x24)
/* STK541 USB Adapter board with Radio Controller board RCB231 V4.0.2 */
#define RCB_4_0_STK541          (0x25)
/* STK500 & STK501 boards with Radio Extender board REB231 V4.0.1 */
#define REB_4_0_STK500_STK501   (0x26)
/*
 * RCB Breakout Board with Radio Controller board RCB231ED 4.1
 * http://www.dresden-elektronik.de/shop/prod85.html
 *
 * RCB Breakout Board Light with Radio Controller board RCB231ED 4.1
 * http://www.dresden-elektronik.de/shop/prod84.html
 */
#define RCB_4_1_BREAKOUT_BOARD  (0x27)
/*
 * Plain Radio Controller board RCB231ED V4.1.1
 */
#define RCB_4_1_PLAIN           (0x28)
/*
 * Sensor Terminal board with Radio Controller board RCB231 4.1
 * http://www.dresden-elektronik.de/shop/prod75.html
 */
#define RCB_4_1_SENS_TERM_BOARD (0x29)
/*
 * STK541 USB Adapter board with Radio Controller board RCB231ED V4.1.1
 * (using antenna diversity)
 */
#define RCB_4_1_STK541          (0x2A)
/* STK500 & STK501 boards with Radio Extender board REB231ED V4.1.1 */
#define REB_4_1_STK500_STK501   (0x2B)
/* STK500 boards with Radio Extender board REB231ED V4.1.1 */
#define REB_4_1_STK500          (0x21)

/*
 * Boards for AT86RF212
 */
/*
 * RCB Breakout Board with Radio Controller board RCB212SMA V5.3.21
 * http://www.dresden-elektronik.de/shop/prod85.html
 *
 * RCB Breakout Board Light with Radio Controller board RCB212SMA V5.3.2
 * http://www.dresden-elektronik.de/shop/prod84.html
 */
#define RCB_5_3_BREAKOUT_BOARD  (0x31)

/* Plain Radio Controller board RCB212SMA V5.3.2 */
#define RCB_5_3_PLAIN           (0x32)
/*
 * Sensor Terminal board with Radio Controller board RCB 5.3
 * http://www.dresden-elektronik.de
 */
#define RCB_5_3_SENS_TERM_BOARD (0x33)
/* STK541 USB Adapter board with Radio Controller board RCB212SMA V5.3.2 */
#define RCB_5_3_STK541          (0x34)
/* STK500 & STK501 boards with Radio Extender board REB212 V5.0.2 */
#define REB_5_0_STK500_STK501   (0x35)
/* Atmel internal */
#define EMU212                  (0x3F)

/*
 * Boards for AT86RF232
 */
/*
 * RCB Breakout Board with Radio Controller board RCB232SMA V7.3
 * http://www.dresden-elektronik.de/shop/prod85.html
 *
 * RCB Breakout Board Light with Radio Controller board RCB232SMA V7.3
 * http://www.dresden-elektronik.de/shop/prod84.html
 */
#define RCB_7_3_BREAKOUT_BOARD  (0x41)
/* Plain Radio Controller board RCB232SMA V7.3 */
#define RCB_7_3_PLAIN           (0x42)
/*
 * Remote Control Demonstration platform using push button keys.
 */
#define RCB_7_3_KEY_RC          (0x43)
/*
 * Sensor Terminal board with Radio Controller board RCB 7.3
 */
#define RCB_7_3_SENS_TERM_BOARD (0x44)
/* STK541 USB Adapter board with Radio Controller board RCB232SMA V7.3 */
#define RCB_7_3_STK541          (0x45)
/* STK500 & STK501 boards with Radio Extender board REB232 V7.0 */
#define REB_7_0_STK500_STK501   (0x46)
/* Atmel internal */
#define EMU232                  (0x4F)


/*****************************************************************************/

#define DEVIDE_TYPE__FFD		(0x01)
#define DEVIDE_TYPE__RFD		(0x02)

#define TAL_MODE__NORMAL		(0x01)
#define TAL_MODE__PROMISCUOUS	(0x02)

#define STB_TYPE__NONE			(0x01)
#define STB_TYPE__ON_SAL		(0x02)
#define STB_TYPE__ARMCRYPTO		(0x03)

#define APP_TYPE__NONE							(0x01)
#define APP_TYPE__TAL_EXAMPLE_PERFORMANCE_TEST	(0x02)
#define APP_TYPE__API_main						(0x03)

#define LQI_MAPPING_MODE__NORMAL				(0x01)
#define LQI_MAPPING_MODE__RSSI					(0x02)
#define LQI_MAPPING_MODE__EDLEVEL				(0x03)

#ifdef	__cplusplus
}
#endif

#endif /* PROPERTY_PROLOGUE_H__lib_at86rf */
