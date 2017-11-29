#ifndef UBICONFIG_H__lib_at86rf
#define UBICONFIG_H__lib_at86rf

#define UBI_BUILDMODE__DEBUG					0
#define UBI_BUILDMODE__RELEASE					1
#define UBI_BUILDMODE							UBI_BUILDMODE__DEBUG
#define UBI_CPU_TYPE__ARM7TDMI					0
#define UBI_CPU_TYPE__ARM9						1
#define UBI_CPU_TYPE__ARM926EJ_S				2
#define UBI_CPU_TYPE__CORTEX_M3					3
#define UBI_CPU_TYPE__CORTEX_M4					4
#define UBI_CPU_TYPE							UBI_CPU_TYPE__CORTEX_M4
#define UBI_THUMB_MODE							1
#define UBI_THUMB_INTERWORK						0
#define UBI_LITTLE_ENDIAN						1
#define UBI_SUCHECK								1
#define UBI_SOCHECK								1
#define UBI_SOCHECK_TYPE						0
#define UBI_INCLUDE_STDLIB						0
#define UBI_GNUC								1


#include "cfg/property_prologue.h"
/*
 * PAL_TYPE				AT91SAM3S
 * PAL_GENERIC_TYPE		SAM3S
 * ���� ���� �ʿ�
 */
#define INCLUDE_MAC_USER_BUILD_CONFIG		0
#define DEBUG		0
#define DEVICE_TYPE		DEVICE_TYPE__FFD
#define HIGHEST_STACK_LAYER		TAL
#define TAL_TYPE		AT86RF231
#define TAL_MODE		TAL_MODE__NORMAL
#define PAL_TYPE							AT91SAM3S
#define PAL_GENERIC_TYPE					SAM3S
#define BOARD_TYPE		REB_4_0_2_REX_ARM_SAM3S_REV_1
#define SAL_TYPE		AT86RF2xx
#define STB_TYPE		STB_TYPE__NONE
#define LQI_MAPPING_MODE		LQI_MAPPING_MODE__NORMAL
#define APP_TYPE		APP_TYPE__API_main
#define INCLUDE_SIO_HUB		1
#define INCLUDE_UART0		1
#define INCLUDE_UART1		0
#define INCLUDE_USB0		0
#define INCLUDE_TFA		1
#define INCLUDE_BEACON_SUPPORT		0
#define INCLUDE_HIGH_DATA_RATE_SUPPORT		1
#define INCLUDE_NON_BLOCKING_SPI		0
#define INCLUDE_SNIFFER		0
#define INCLUDE_HIGH_PRIO_TMR		0
#define INCLUDE_DEBUG_PINS		0
#define INCLUDE_FTN_PLL_CALIBRATION		0
#define INCLUDE_EXTERN_EEPROM		0
#define INCLUDE_TRX_SRAM		0
#define INCLUDE_TEST_HARNESS		0
#define EXCLUDE_IEEE_ADDR_CHECK		0


#include "cfg/property_epilogue.h"

#define UBI_COMPONENT__lib_at86rf
#define UBI_COMCONFIG__lib_at86rf

#endif /* UBICONFIG_H__lib_at86rf */
