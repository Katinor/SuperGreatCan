#ifndef PROPERTY_EPILOGUE_H__lib_at86rf
#define PROPERTY_EPILOGUE_H__lib_at86rf

#ifdef	__cplusplus
extern "C" {
#endif

#if 	(1 == INCLUDE_MAC_USER_BUILD_CONFIG)
	#define MAC_USER_BUILD_CONFIG
#endif

#if 	(DEVICE_TYPE__FFD == DEVICE_TYPE)
	#define FFD
#elif	(DEVICE_TYPE__RFD == DEVICE_TYPE)
#else
	#error "Unsupported DEVICE_TYPE"
#endif

#if		(TAL_MODE__NORMAL 		== TAL_MODE)
#elif 	(TAL_MODE__PROMISCUOUS 	== TAL_MODE)
	#define PROMISCUOUS_MODE
#else
	#error "Unsupported TAL_MODE"
#endif

#if		(STB_TYPE__NONE 		== STB_TYPE)
#elif	(STB_TYPE__ON_SAL 		== STB_TYPE)
	#define STB_ON_SAL
#elif	(STB_TYPE__ARMCRYPTO	== STB_TYPE)
	#define STB_ARMCRYPTO
#else
	#error "Unsupported STB_TYPE"
#endif

#if 	(1 == INCLUDE_SIO_HUB)
	#define SIO_HUB
#endif

#if 	(1 == INCLUDE_UART0)
	#define PAL_UART0
#endif

#if 	(1 == INCLUDE_UART1)
	#define PAL_UART1
#endif

#if 	(1 == INCLUDE_USB0)
	#define USB0
#endif

#if 	(1 == INCLUDE_TFA)
	#define ENABLE_TFA
#endif

#if 	(1 == INCLUDE_BEACON_SUPPORT)
	#define BEACON_SUPPORT
#endif

#if 	(1 == INCLUDE_HIGH_DATA_RATE_SUPPORT)
	#define HIGH_DATA_RATE_SUPPORT
#endif

#if		(LQI_MAPPING_MODE__NORMAL 	== LQI_MAPPING_MODE)
#elif	(LQI_MAPPING_MODE__RSSI 	== LQI_MAPPING_MODE)
	#define RSSI_TO_LQI_MAPPING
#elif	(LQI_MAPPING_MODE__EDLEVEL	== LQI_MAPPING_MODE)
	#define RSSI_TO_LQI_MAPPING
	#define EDLEVEL_TO_LQI_MAPPING
#else
	#error "Unsupported LQI_MAPPING_MODE"
#endif

#if 	(1 == INCLUDE_NON_BLOCKING_SPI)
	#define NON_BLOCKING_SPI
#endif

#if 	(1 == INCLUDE_SNIFFER)
	#define SNIFFER
#endif

#if 	(1 == INCLUDE_HIGH_PRIO_TMR)
	#define ENABLE_HIGH_PRIO_TMR
#endif

#if 	(1 == INCLUDE_DEBUG_PINS)
	#define ENABLE_DEBUG_PINS
#endif

#if 	(1 == INCLUDE_FTN_PLL_CALIBRATION)
	#define ENABLE_FTN_PLL_CALIBRATION
#endif

#if 	(1 == INCLUDE_EXTERN_EEPROM)
	#define EXTERN_EEPROM_AVAILABLE
#endif

#if		(1 == INCLUDE_TRX_SRAM)
	#define ENABLE_TRX_SRAM
#endif

#if 	(1 == INCLUDE_TEST_HARNESS)
	#define TEST_HARNESS
#endif

#if 	(1 == EXCLUDE_IEEE_ADDR_CHECK)
	#define DISABLE_IEEE_ADDR_CHECK
#endif

#ifdef	__cplusplus
}
#endif

#endif /* PROPERTY_EPILOGUE_H__lib_at86rf */
