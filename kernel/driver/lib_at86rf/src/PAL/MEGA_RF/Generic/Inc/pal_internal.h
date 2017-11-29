/**
 * @file pal_internal.h
 *
 * @brief PAL internal functions prototypes for AVR ATmega MCUs
 *
 * $Id: pal_internal.h,v 1.2 2010-08-01 09:05:46 slb Exp $
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


/* === Types ================================================================ */


/* === Externals ============================================================ */


/* === Macros ================================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#if (EXTERN_EEPROM_AVAILABLE == 1) || defined(DOXYGEN)
retval_t extern_eeprom_get(uint8_t start_offset, uint8_t length, void *value);
void extern_eeprom_init(void);
#endif  /* #if (EXTERN_EEPROM_AVAILABLE == 1) || defined(DOXYGEN) */

void mcu_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* PAL_INTERNAL_H */
/* EOF */
