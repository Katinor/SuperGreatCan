/**
 * @file pal_boardtypes.h
 *
 * @brief PAL board types for AT91SAM7X256
 *
 * This header file contains board types based on AT91SAM7X256.
 *
 * $Id: pal_boardtypes.h,v 1.2 2012-02-14 05:21:09 essusige Exp $
 *
 */
/**
 * @author
 * Atmel Corporation: http://www.atmel.com
 * Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */
#ifndef PAL_BOARDTYPES_H
#define PAL_BOARDTYPES_H

#include "../../../../../ubiconfig.h"
#include "../../../../../../../kernel/porting/lib_sam4e/sam/utils/compiler.h"

/* Boards for AT86RF230B */

/*
 * AT91SAM7X-EK boards with Radio Extender board REB230B V2.3 on REBtoSAM7EK adapter
 * http://www.dresden-elektronik.de/shop/prod72.html
 * http://www.dresden-elektronik.de/shop/prod90.html
 */
#define REB_2_3_REB_TO_SAM3SEK       (0x01)


/* Boards for AT86RF231 */
/*
 * AT91SAM7X-EK boards with Radio Extender board REB231 V4.0.1/V4.0.2 on REBtoSAM7EK adapter
 * http://www.dresden-elektronik.de/shop/prod73.html
 * http://www.dresden-elektronik.de/shop/prod90.html
 */

/* ESPS boards with Radio Extender board REB231 V4.0.1/4.0.2 on REX_ARM adapter Revision 3 */
#define REB_4_0_2_REX_ARM_SAM3S_REV_1	(0xC1)



/* Boards for AT86RF212 */
/*
 * AT91SAM7X-EK boards with Radio Extender board REB212 V5.0.2 on REBtoSAM7EK adapter
 * http://www.dresden-elektronik.de/shop/prod79.html
 * http://www.dresden-elektronik.de/shop/prod90.html
 */
#define REB_5_0_REB_TO_SAM3SEK       (0x21)

#endif  /* PAL_BOARDTYPES_H */
/* EOF */
