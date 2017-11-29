/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the lib_ubik_port_arm component of the Ubinos.

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

#ifndef _UBIK_ARMCORTEXM_SAM4EEK_H_
#define _UBIK_ARMCORTEXM_SAM4EEK_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * @file _ubik_armcortexm_sam4sek2.h
 *
 * @brief ubik (Ubinos Kernel) SAM7x-EK porting ������Ʈ ���� �������̽�
 *
 * ubik SAM7x-EK porting ������Ʈ ���� �������̽��� �����մϴ�.
 */

#include "../ubiconfig.h"

#include "../../../common/lib_ubik/src/_ubik.h"

#if   (CPU_MODEL == CPU_MODEL__SAM4E16E)
	#define DONT_USE_CMSIS_INIT
	#include "../../bsp_armcortexm_sam4eek/itf/sam4e.h"
	#include "../../bsp_armcortexm_sam4eek/itf/core_cm4.h"
#else
	#error "Unsupported CPU_MODEL"
#endif

#if (1 != EXCLUDE_HRTICK)
	#if defined(__SAM4E16E__)
	extern Tc * 	_ubik_hrtick_htimer_p;
	extern int 			_ubik_hrtick_htimer_id;
	extern unsigned int	_ubik_hrtick_hrtickpertick;

	#else
	extern AT91S_TC * 	_ubik_hrtick_htimer_p;
	extern int 			_ubik_hrtick_htimer_id;
	extern unsigned int	_ubik_hrtick_hrtickpertick;
	#endif
#endif

#ifdef	__cplusplus
}
#endif

#endif /* _UBIK_ARMCORTEXM_SAM4EEK_H_ */
