/**
 * @file usr_mlme_disassociate_ind.c
 *
 * @brief This file contains user call back function for
 * MLME-DISASSOCIATE.indication
 *
 * $Id: usr_mlme_disassociate_ind.c,v 1.2 2010-07-30 16:38:38 slb Exp $
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

#if (MAC_DISASSOCIATION_BASIC_SUPPORT == 1)

/* === Macros ============================================================== */


/* === Globals ============================================================= */


/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */

void usr_mlme_disassociate_ind(uint64_t DeviceAddress,
                               uint8_t DisassociateReason)
{
    /* Keep compiler happy. */
    DeviceAddress = DeviceAddress;
    DisassociateReason = DisassociateReason;
}

#endif  /* (MAC_DISASSOCIATION_BASIC_SUPPORT == 1) */

/* EOF */
