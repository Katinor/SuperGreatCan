/**
 * @file broadcast_structures.h
 *
 * @brief This module contains structures used internally for broadcast
 *        transmission in a beacon-enabled network by the PAN Coordinator
 *        or a Coordinator.
 *
 * $Id: broadcast_structures.h,v 1.2 2010-07-30 18:06:27 slb Exp $
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
#ifndef BROADCAST_STRUCTURES_H
#define BROADCAST_STRUCTURES_H

#include "../../ubiconfig.h"

/* === Includes ============================================================= */


/* === Macros =============================================================== */


/* === Types ================================================================ */

/**
 * @brief This is the Broadcast message structure.
 */
typedef struct broadcast_tag
{
    frame_info_t *data;
    uint8_t msduHandle;
} broadcast_t;

/* === Externals ============================================================ */


/* === Prototypes =========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BROADCAST_STRUCTURES_H */
/* EOF */
