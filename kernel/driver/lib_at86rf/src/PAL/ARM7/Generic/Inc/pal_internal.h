/**
 * @file pal_internal.h
 *
 * @brief PAL internal functions prototypes for ARM7 MCUs
 *
 * $Id: pal_internal.h,v 1.3 2010-08-05 01:54:27 slb Exp $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */


/* Prevent double inclusion */
#ifndef PAL_INTERNAL_H
#define PAL_INTERNAL_H

#include "../../../../../ubiconfig.h"

/* === Includes ============================================================= */

#include "../../../Inc/pal.h"

/* === Types ================================================================ */


/* === Externals ============================================================ */


/* === Macros ================================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

void gpio_init(void);
void trx_interface_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* PAL_INTERNAL_H */
/* EOF */
