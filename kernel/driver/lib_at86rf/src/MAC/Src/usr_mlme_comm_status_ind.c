/**
 * @file usr_mlme_comm_status_ind.c
 *
 * @brief This file contains user call back function for
 * MLME-COMM-STATUS.indication.
 *
 * $Id: usr_mlme_comm_status_ind.c,v 1.2 2010-07-30 16:38:38 slb Exp $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#include "../../../ubiconfig.h"

/* === Includes ============================================================= */

#include <stdint.h>
#include <stdbool.h>
#include "../Inc/mac_api.h"

/* === Macros ============================================================== */

/* === Globals ============================================================= */

/* === Prototypes ========================================================== */

/* === Implementation ====================================================== */

void usr_mlme_comm_status_ind(wpan_addr_spec_t *SrcAddrSpec,
                              wpan_addr_spec_t *DstAddrSpec,
                              uint8_t status)
{
    /* Keep compiler happy. */
    SrcAddrSpec = SrcAddrSpec;
    DstAddrSpec = DstAddrSpec;
    status = status;
}
/* EOF */
