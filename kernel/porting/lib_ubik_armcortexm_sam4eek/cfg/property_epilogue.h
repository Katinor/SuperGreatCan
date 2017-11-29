#ifndef PROPERTY_EPILOGUE_H__lib_ubik_armcortexm_sam4eek_
#define PROPERTY_EPILOGUE_H__lib_ubik_armcortexm_sam4eek_

#ifdef	__cplusplus
extern "C" {
#endif

#include "../../lib_ubik_armcortexm/cfg/property_epilogue.h"

#ifndef	CPU_MODEL
	#define CPU_MODEL								CPU_MODEL__SAM4E16E
#endif

////

#ifndef	TICK_PER_SEC
	#define TICK_PER_SEC                            1000
#endif

////

#if		((60 > TICK_PER_SEC) || (10000 < TICK_PER_SEC))
	#error "Unrecommended TICK_PER_SEC"
#endif

#ifdef	__cplusplus
}
#endif

#endif /* PROPERTY_EPILOGUE_H__lib_ubik_armcortexm_sam4eek_ */
