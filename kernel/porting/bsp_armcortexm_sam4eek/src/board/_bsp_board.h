/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the bsp_armcortexm_sam3s4ek component of the Ubinos.

  GNU General Public License Usage
  This file may be used under the terms of the GNU
  General Public License version 3.0 as published by the Free Software
  Foundation and appearing in the file license_gpl3.txt included in the
  packaging of this file. Please review the following information to
  ensure the GNU General Public License version 3.0 requirements will be
  met: http://www.gnu.org/copyleft/gpl.html.

  GNU Lesser General Public License Usage
  Alternatively, this file may be used under the terms of the GNU Lesser
  General Public License version 2.1 as published by the Free Software
  Foundation and appearing in the file license_lgpl.txt included in the
  packaging of this file. Please review the following information to
  ensure the GNU Lesser General Public License version 2.1 requirements
  will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.

  Commercial Usage
  Alternatively, licensees holding valid commercial licenses may
  use this file in accordance with the commercial license agreement
  provided with the software or, alternatively, in accordance with the
  terms contained in a written agreement between you and rightful owner.
*/

#ifndef _BSP_BOARD_H_
#define _BSP_BOARD_H_

#include "../../ubiconfig.h"

#include "../arch/_bsp_arch.h"

#if   (CPU_MODEL == CPU_MODEL__SAM4SD32C)
			#include "../../itf/sam4sd32c.h"
#elif   	(CPU_MODEL == CPU_MODEL__SAM4E16E)
			#include "../../itf/sam4e.h"
#else
			#error "Unsupported CPU_MODEL"
#endif

#ifndef __ASSEMBLY__
	void lowlevel_init(void);
#endif

/* Platform specific definition */

#define AT91_CHIP_ID__SAM4E16E_REV_A	0xA3CC0CE0
#define AT91_CHIP_ID__SAM4E8E_REV_A		0xA3CC0CE0
#define AT91_CHIP_ID__SAM4E16C_REV_A	0xA3CC0CE0
#define AT91_CHIP_ID__SAM4E8C_REV_A		0xA3CC0CE0

#ifndef __ASSEMBLY__

#define AT91_NVM_CMD__SECURITY							0
#define AT91_NVM_CMD__BOOT_MODE_SELECT					1

#include "../../itf/core_cm4.h"

#include "itf_ubinos/itf/bsp_intr.h"

extern isr_ft _irq_handler_table[EXCEPTION_TYPE_IRQ_END - EXCEPTION_TYPE_IRQ_START + 1];

int at91_getnvm(int cmd);
int at91_setnvm(int cmd, int value);

#endif

#endif /* _BSP_BOARD_H_ */

