#ifndef PROPERTY_EPILOGUE_H__bsp_armcortexm_sam4eek_
#define PROPERTY_EPILOGUE_H__bsp_armcortexm_sam4eek_

#ifdef	__cplusplus
extern "C" {
#endif

#include "../../lib_ubik_armcortexm_sam4eek/cfg/property_epilogue.h"

#ifndef	MAIN_CLOCK_FREQ
	#define MAIN_CLOCK_FREQ								12000
#endif

#ifndef	SLOW_CLOCK_FREQ
	#define SLOW_CLOCK_FREQ								32
#endif

#ifndef	INCLUDE_MMU
	#define INCLUDE_MMU									0
#endif

#ifndef	INCLUDE_CACHE
	#define INCLUDE_CACHE								0
#endif

#ifndef	MEMMAP
	#define MEMMAP										MEMMAP__FLASH
#endif

#ifndef	BUSYWAITCOUNT_PER_MS
	#if (MEMMAP__SRAM == MEMMAP)
		#define BUSYWAITCOUNT_PER_MS						15633
	#elif (MEMMAP__FLASH == MEMMAP)
		#define BUSYWAITCOUNT_PER_MS						15633
	#else
		#error "Unsupported MEMMAP"
	#endif
#endif

/* 2015. 06. 05. RTLAB_GYLee */
/* add us busywaiting*/
#ifndef BUSYWAITCOUNT_PER_US
#if (MEMMAP__SRAM == MEMMAP)
	#define BUSYWAITCOUNT_PER_US						16
#elif (MEMMAP__FLASH == MEMMAP)
	#define BUSYWAITCOUNT_PER_US						16
#else
	#error "Unsupported MEMMAP"
#endif
#endif

#ifndef	EXCLUDE_COMMON_SEGMENT_INIT
	#define EXCLUDE_COMMON_SEGMENT_INIT					0
#endif

#ifndef	EXCLUDE_SEGMENT_END_BOUNDARY_CHECK
	#define EXCLUDE_SEGMENT_END_BOUNDARY_CHECK			0
#endif

#ifndef	EXCLUDE_SPRINTFMEMMAP
	#define EXCLUDE_SPRINTFMEMMAP						0
#endif

#ifndef	EXCLUDE_PRINTFMEMMAP
	#define EXCLUDE_PRINTFMEMMAP						0
#endif

#ifndef	EXCLUDE_MISC
	#define EXCLUDE_MISC								0
#endif

////

#ifndef	ABT_STACK_SIZE
	#define ABT_STACK_SIZE								0x0000
#endif

#ifndef	UND_STACK_SIZE
	#define UND_STACK_SIZE								0x0000
#endif

#ifndef	FIQ_STACK_SIZE
	#define FIQ_STACK_SIZE								0x0000
#endif

#ifndef	IRQ_STACK_SIZE
	#define IRQ_STACK_SIZE								0x0000
#endif

#ifndef	SVC_STACK_SIZE
	#define SVC_STACK_SIZE								0x0200
#endif

#ifndef	SYS_STACK_SIZE
	#define SYS_STACK_SIZE								0x0000
#endif

#ifndef	IDLETASK_STACK_SIZE
	#define IDLETASK_STACK_SIZE							0x0200
#endif

#ifndef	SOCHECK_MARGIN
	#define SOCHECK_MARGIN								0x50
#endif

////

#if 	(0x00 > ABT_STACK_SIZE)
	#error "ABT_STACK_SIZE should be equal or larger than 0x00."
#endif

#if 	(0x00 > UND_STACK_SIZE)
	#error "UND_STACK_SIZE should be equal or larger than 0x00."
#endif

#if 	(0x00 > FIQ_STACK_SIZE)
	#error "FIQ_STACK_SIZE should be equal or larger than 0x00."
#endif

#if 	(0x00 > IRQ_STACK_SIZE)
	#error "IRQ_STACK_SIZE should be equal or larger than 0x00."
#endif

#if 	(0x00 > SYS_STACK_SIZE)
	#error "SYS_STACK_SIZE should be equal or larger than 0x00."
#endif

#if 	(0x50 > SOCHECK_MARGIN)
	#error "SOCHECK_MARGIN should be equal or larger than 0x50."
#endif

#ifdef	__cplusplus
}
#endif

#endif /* PROPERTY_EPILOGUE_H__bsp_armcortexm_sam4eek_ */
