#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H

#include "../../ubiconfig.h"

#if		(APP_TYPE__NONE 							== APP_TYPE)
#elif	(APP_TYPE__TAL_EXAMPLE_PERFORMANCE_TEST 	== APP_TYPE)
	#include "../Applications/TAL_Examples/Performance_Test/Inc/app_config.h"
#elif	(APP_TYPE__API_main 	== APP_TYPE)
	#include "../Applications/API_main/Inc/app_config.h"
#else
	#error "Unsupported APP_TYPE"
#endif

#endif /* _APP_CONFIG_H */
