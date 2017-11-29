/**
 * @file usr_mlme_set_conf.c
 *
 * @brief This file contains user call back function for MLME-SET.confirm.
 *
 * $Id: usr_mlme_set_conf.c,v 1.2 2010-07-30 16:38:38 slb Exp $
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

void usr_mlme_set_conf(uint8_t status, uint8_t PIBAttribute)
{
    /* Keep compiler happy. */
    status = status;
    PIBAttribute = PIBAttribute;
}
/* EOF */
