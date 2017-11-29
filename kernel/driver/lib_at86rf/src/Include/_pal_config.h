#ifndef _PAL_CONFIG_H
#define _PAL_CONFIG_H

#include "../../ubiconfig.h"


/* PAL types: */
#if		!defined(PAL_GENERIC_TYPE)
	#error "Unsupported PAL_GENERIC_TYPE"
#elif	(PAL_GENERIC_TYPE == ARM7)

	#if		!defined(PAL_TYPE)
		#error "Unsupported PAL_TYPE"
	#elif	(PAL_TYPE == AT91SAM7X256)

		#if		!defined(BOARD_TYPE)
			#error "Unsupported BOARD_TYPE"
		#elif	(BOARD_TYPE == REB_2_3_REX_ARM_REV_2)
			#include "../PAL/ARM7/AT91SAM7X256/Boards/REB_2_3_REX_ARM_REV_2/pal_config.h"
		#elif	(BOARD_TYPE == REB_4_0_2_REB_TO_SAM7EK)
			#error "Unsupported BOARD_TYPE"
		#elif	(BOARD_TYPE == REB_4_0_2_REX_ARM_REV_3)
			#include "../PAL/ARM7/AT91SAM7X256/Boards/REB_4_0_2_REX_ARM_REV_3/pal_config.h"
		#elif	(BOARD_TYPE ==  REB_4_0_2_REX_ARM_ESPS_REV_1)
			#include "../PAL/ARM7/AT91SAM7X256/Boards/REB_4_0_2_REX_ARM_ESPS_REV_1/pal_config.h"
		#elif	(BOARD_TYPE == REB_5_0_REB_TO_SAM7EK)
			#error "Unsupported BOARD_TYPE"
		#elif	(BOARD_TYPE == REB_5_0_REX_ARM_REV_3)
			#include "../PAL/ARM7/AT91SAM7X256/Boards/REB_5_0_REX_ARM_REV_3/pal_config.h"
		#else
			#error "Unsupported BOARD_TYPE"
		#endif

	#elif	(PAL_TYPE == AT91SAM7XC256)

		#if		!defined(BOARD_TYPE)
			#error "Unsupported BOARD_TYPE"
		#elif	(BOARD_TYPE == REB_2_3_REX_ARM_REV_2)
			#include "../PAL/ARM7/AT91SAM7XC256/Boards/REB_2_3_REX_ARM_REV_2/pal_config.h"
		#else
			#error "Unsupported BOARD_TYPE"
		#endif

	#else
		#error "Unsupported PAL_TYPE"
	#endif

#elif	(PAL_GENERIC_TYPE == ARM9)

	#if		!defined(PAL_TYPE)
		#error "Unsupported PAL_TYPE"
	#elif	(PAL_TYPE == AT91SAM9XE512)
		#if		!defined(BOARD_TYPE)
			#error "Unsupported BOARD_TYPE"
		#elif	(BOARD_TYPE == REB_4_0_2_REX_ARM_SMART_REV_1)
			#include "../PAL/ARM9/AT91SAM9XE512/Boards/REB_4_0_2_REX_ARM_SMART_REV_1/pal_config.h"
		#else
			#error "Unsupported BOARD_TYPE"
		#endif
	#else
		#error "Unsupported PAL_TYPE"
	#endif

#elif	(PAL_GENERIC_TYPE == SAM3S)

	#if		!defined(PAL_TYPE)
		#error "Unsupported PAL_TYPE"
	#elif	(PAL_TYPE == AT91SAM3S)
		#if		!defined(BOARD_TYPE)
			#error "Unsupported BOARD_TYPE"
		#elif	(BOARD_TYPE ==  REB_4_0_2_REX_ARM_SAM3S_REV_1)
			#include "../PAL/SAM3S/AT91SAM3S/Boards/REB_4_0_2_REX_ARM_SAM3S_REV_1/pal_config.h"
		#else
			#error "Unsupported BOARD_TYPE"
		#endif
	#else
		#error "Unsupported PAL_TYPE"
	#endif

#elif	(PAL_GENERIC_TYPE == MEGA_RF)
	#if		!defined(PAL_TYPE)
		#error "Unsupported PAL_TYPE"
	#elif	(PAL_TYPE == ATMEGA128RFA1)

		#if		!defined(BOARD_TYPE)
			#error "Unsupported BOARD_TYPE"
		#elif	(BOARD_TYPE == EK1)
			#include "../PAL/MEGA_RF/ATMEGA128RFA1/Boards/EK1/pal_config.h"
		#elif	(BOARD_TYPE == RCB_6_3_BREAKOUT_BOARD)
			#include "../PAL/MEGA_RF/ATMEGA128RFA1/Boards/RCB_6_3_BREAKOUT_BOARD/pal_config.h"
		#elif	(BOARD_TYPE == RCB_6_3_PLAIN)
			#include "../PAL/MEGA_RF/ATMEGA128RFA1/Boards/RCB_6_3_PLAIN/pal_config.h"
		#elif	(BOARD_TYPE == RCB_6_3_SENS_TERM_BOARD)
			#include "../PAL/MEGA_RF/ATMEGA128RFA1/Boards/RCB_6_3_SENS_TERM_BOARD/pal_config.h"
		#else
			#error "Unsupported BOARD_TYPE"
		#endif

	#else
		#error "Unsupported PAL_TYPE"
	#endif

#elif	(PAL_GENERIC_TYPE == XMEGA)

	#if		!defined(PAL_TYPE)
		#error "Unsupported PAL_TYPE"
	#elif	(PAL_TYPE == ATXMEGA128A1)

		#if		!defined(BOARD_TYPE)
			#error "Unsupported BOARD_TYPE"
		#elif	(BOARD_TYPE == REB_2_3_STK600)
			#include "../PAL/XMEGA/ATXMEGA128A1/Boards/REB_2_3_STK600/pal_config.h"
		#elif	(BOARD_TYPE == REB_4_0_STK600)
			#include "../PAL/XMEGA/ATXMEGA128A1/Boards/REB_4_0_STK600/pal_config.h"
		#elif	(BOARD_TYPE == REB_4_1_STK600)
			#include "../PAL/XMEGA/ATXMEGA128A1/Boards/REB_4_1_STK600/pal_config.h"
		#elif	(BOARD_TYPE == REB_5_0_STK600)
			#include "../PAL/XMEGA/ATXMEGA128A1/Boards/REB_5_0_STK600/pal_config.h"
		#else
			#error "Unsupported BOARD_TYPE"
		#endif

	#elif	(PAL_TYPE == ATXMEGA256A3)

		#if		!defined(BOARD_TYPE)
			#error "Unsupported BOARD_TYPE"
		#elif	(BOARD_TYPE == REB_4_1_STK600)
			#include "../PAL/XMEGA/ATXMEGA256A3/Boards/REB_4_1_STK600/pal_config.h"
		#else
			#error "Unsupported BOARD_TYPE"
		#endif

	#else
		#error "Unsupported PAL_TYPE"
	#endif

#elif	(PAL_GENERIC_TYPE == AVR)

	#if		!defined(PAL_TYPE)
		#error "Unsupported PAL_TYPE"
	#elif	(PAL_TYPE == AT90USB1287)

		#if		!defined(BOARD_TYPE)
			#error "Unsupported BOARD_TYPE"
		#elif	(BOARD_TYPE == USBSTICK_C)
			#include "../PAL/AVR/AT90USB1287/Boards/USBSTICK_C/pal_config.h"
		#else
			#error "Unsupported BOARD_TYPE"
		#endif

	#elif	(PAL_TYPE == ATMEGA1281)

		#if		!defined(BOARD_TYPE)
			#error "Unsupported BOARD_TYPE"
		#elif	(BOARD_TYPE == RCB_3_2_BREAKOUT_BOARD)
			#include "../PAL/AVR/ATMEGA1281/Boards/RCB_3_2_BREAKOUT_BOARD/pal_config.h"
		#elif	(BOARD_TYPE == RCB_3_2_PLAIN)
			#include "../PAL/AVR/ATMEGA1281/Boards/RCB_3_2_PLAIN/pal_config.h"
		#elif	(BOARD_TYPE == RCB_3_2_SENS_TERM_BOARD)
			#include "../PAL/AVR/ATMEGA1281/Boards/RCB_3_2_SENS_TERM_BOARD/pal_config.h"
		#elif	(BOARD_TYPE == RCB_4_0_BREAKOUT_BOARD)
			#include "../PAL/AVR/ATMEGA1281/Boards/RCB_4_0_BREAKOUT_BOARD/pal_config.h"
		#elif	(BOARD_TYPE == RCB_4_0_PLAIN)
			#include "../PAL/AVR/ATMEGA1281/Boards/RCB_4_0_PLAIN/pal_config.h"
		#elif	(BOARD_TYPE == RCB_4_0_SENS_TERM_BOARD)
			#include "../PAL/AVR/ATMEGA1281/Boards/RCB_4_0_SENS_TERM_BOARD/pal_config.h"
		#elif	(BOARD_TYPE == RCB_4_1_BREAKOUT_BOARD)
			#include "../PAL/AVR/ATMEGA1281/Boards/RCB_4_1_BREAKOUT_BOARD/pal_config.h"
		#elif	(BOARD_TYPE == RCB_4_1_PLAIN)
			#include "../PAL/AVR/ATMEGA1281/Boards/RCB_4_1_PLAIN/pal_config.h"
		#elif	(BOARD_TYPE == RCB_4_1_SENS_TERM_BOARD)
			#include "../PAL/AVR/ATMEGA1281/Boards/RCB_4_1_SENS_TERM_BOARD/pal_config.h"
		#elif	(BOARD_TYPE == RCB_5_3_BREAKOUT_BOARD)
			#include "../PAL/AVR/ATMEGA1281/Boards/RCB_5_3_BREAKOUT_BOARD/pal_config.h"
		#elif	(BOARD_TYPE == RCB_5_3_PLAIN)
			#include "../PAL/AVR/ATMEGA1281/Boards/RCB_5_3_PLAIN/pal_config.h"
		#elif	(BOARD_TYPE == RCB_5_3_SENS_TERM_BOARD)
			#include "../PAL/AVR/ATMEGA1281/Boards/RCB_5_3_SENS_TERM_BOARD/pal_config.h"
		#elif	(BOARD_TYPE == REB_2_3_STK500_STK501)
			#include "../PAL/AVR/ATMEGA1281/Boards/REB_2_3_STK500_STK501/pal_config.h"
		#elif	(BOARD_TYPE == REB_4_0_STK500_STK501)
			#include "../PAL/AVR/ATMEGA1281/Boards/REB_4_0_STK500_STK501/pal_config.h"
		#elif	(BOARD_TYPE == REB_4_1_STK500_STK501)
			#include "../PAL/AVR/ATMEGA1281/Boards/REB_4_1_STK500_STK501/pal_config.h"
		#elif	(BOARD_TYPE == REB_5_0_STK500_STK501)
			#include "../PAL/AVR/ATMEGA1281/Boards/REB_5_0_STK500_STK501/pal_config.h"
		#else
			#error "Unsupported BOARD_TYPE"
		#endif

	#elif	(PAL_TYPE == ATMEGA2561)

		#if		!defined(BOARD_TYPE)
			#error "Unsupported BOARD_TYPE"
		#elif	(BOARD_TYPE == REB_2_3_STK500_STK501)
			#include "../PAL/AVR/ATMEGA1284P/Boards/REB_2_3_STK500_STK501/pal_config.h"
		#else
			#error "Unsupported BOARD_TYPE"
		#endif

	#elif	(PAL_TYPE == ATMEGA644P)

		#if		!defined(BOARD_TYPE)
			#error "Unsupported BOARD_TYPE"
		#elif	(BOARD_TYPE == REB_2_3_STK500)
			#include "../PAL/AVR/ATMEGA644P/Boards/REB_2_3_STK500/pal_config.h"
		#elif	(BOARD_TYPE == REB_4_1_STK500)
			#include "../PAL/AVR/ATMEGA644P/Boards/REB_4_1_STK500/pal_config.h"
		#else
			#error "Unsupported BOARD_TYPE"
		#endif

	#elif	(PAL_TYPE == ATMEGA1284P)

		#error "Unsupported PAL_TYPE"

	#else
		#error "Unsupported PAL_TYPE"
	#endif

#elif	(PAL_GENERIC_TYPE == AVR32)

	#error "Unsupported PAL_GENERIC_TYPE"

#else
	#error "Unsupported PAL_GENERIC_TYPE"
#endif

#endif /* _PAL_CONFIG_H */
