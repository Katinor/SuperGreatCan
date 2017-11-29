#ifndef _TAL_INTERNAL_H
#define _TAL_INTERNAL_H

#include "../../ubiconfig.h"

/* TAL types: */
#if		!defined(TAL_TYPE)
	#error "Unsupported TAL_TYPE"
#elif	(TAL_TYPE == AT86RF212)
	#include "../TAL/AT86RF212/Inc/tal_internal.h"
#elif	(TAL_TYPE == AT86RF230A)
	#error "Unsupported TAL type"
#elif	(TAL_TYPE == AT86RF230B)
	#include "../TAL/AT86RF230B/Inc/tal_internal.h"
#elif	(TAL_TYPE == AT86RF231)
	#include "../TAL/AT86RF231/Inc/tal_internal.h"
#elif	(TAL_TYPE == ATMEGARF_TAL_1)
	#include "../TAL/ATMEGARF_TAL_1/Inc/tal_internal.h"
#elif	(TAL_TYPE == AT86RF232)
	#error "Unsupported TAL type"
#else
	#error "Unsupported TAL type"
#endif

#endif /* _TAL_INTERNAL_H */
