#ifndef UBICONFIG_H__exe_ubinos_test_
#define UBICONFIG_H__exe_ubinos_test_

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



#include "cfg/property_epilogue.h"

#include "itf_ubinos/ubiconfig.h"
#include "../../../kernel/common/lib_ubiclib/ubiconfig.h"
#include "../../../kernel/porting/lib_ubiclib_arm/ubiconfig.h"
#include "../../../kernel/common/lib_ubik/ubiconfig.h"
#include "../../../kernel/porting/lib_ubik_armcortexm/ubiconfig.h"
#include "../../../kernel/porting/lib_ubik_armcortexm_sam4eek/ubiconfig.h"
#include "../../../kernel/porting/bsp_armcortexm_sam4eek/ubiconfig.h"

#define	TURE			1
#define	E__SUCCESS		UBIK_ERR__SUCCESS
#define	E__FAIL			UBIK_ERR__FAIL
#define	E__TIMEOUT		UBIK_ERR__TIMEOUT
#define	E__DEADLOCK		UBIK_ERR__DEADLOCK
#define	E__TERMINATED	UBIK_ERR__TERMINATED
#define	E__OVERFLOWED	UBIK_ERR__OVERFLOWED
#endif /* UBICONFIG_H__exe_ubinos_test_ */
