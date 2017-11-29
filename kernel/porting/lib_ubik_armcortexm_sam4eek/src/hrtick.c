/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the lib_ubik_armclassic_sam7xek component of the Ubinos.

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

#include "../ubiconfig.h"

#include "_ubik_board.h"

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"
#include "itf_ubinos/itf/bsp_intr.h"
#include "itf_ubinos/itf/ubiclib.h"
#include "itf_ubinos/itf/ubiclib_logm.h"
#include "itf_ubinos/itf/ubiclib_heap.h"
#include "itf_ubinos/itf/ubik.h"
#include "itf_ubinos/itf/ubik_task.h"
#include "itf_ubinos/itf/ubik_sem.h"
#include "itf_ubinos/itf/ubik_hrtick.h"

#if (1 != EXCLUDE_HRTICK)

#if   (CPU_MODEL == CPU_MODEL__SAM7X256) || (CPU_MODEL == CPU_MODEL__SAM7X512) || (CPU_MODEL == CPU_MODEL__SAM7L128)
	// MCK = 48000000, TIMER_CLOCK3: Freq = MCK/32  = 1500000, Interval = 1/Freq =  666.666667 ns, 16 bit (65536): Max = Interval * 65536 = 43,690,666 ns
	#define HRTICK_DIV_TYPE		AT91C_TC_CLKS_TIMER_DIV3_CLOCK
	#define HRTICK_DIV			32
#elif (CPU_MODEL == CPU_MODEL__SAM9XE512)
	// MCK = 99328000, TIMER_CLOCK4: Freq = MCK/128 =  776000, Interval = 1/Freq = 1288.659794 ns, 16 bit (65536): Max = Interval * 65536 = 84,453,608 ns
	#define HRTICK_DIV_TYPE		AT91C_TC_CLKS_TIMER_DIV4_CLOCK
	#define HRTICK_DIV			128
#else
	#error "Unsupported CPU_MODEL"
#endif

#if defined(__SAM4E16E__)
Tc *		_ubik_hrtick_htimer_p 				= NULL;
int 			_ubik_hrtick_htimer_id 				= -1;
unsigned int	_ubik_hrtick_hrtickpertick			= UINT_MAX;
#else
AT91S_TC *		_ubik_hrtick_htimer_p 				= NULL;
int 			_ubik_hrtick_htimer_id 				= -1;
unsigned int	_ubik_hrtick_hrtickpertick			= UINT_MAX;
#endif

int ubik_hrtick_enable(int htimerno) {
	unsigned int mckfreqk;

#if defined(__SAM4E16E__)
	if (0 != _ubik_hrtick_active) {
		return -1;
	}

	switch (htimerno) {
	case 0:
		_ubik_hrtick_htimer_p 	= TC0;
		_ubik_hrtick_htimer_id 	= TC0;
		break;
	case 1:
		_ubik_hrtick_htimer_p 	= TC1;
		_ubik_hrtick_htimer_id 	= TC1;
		break;
	case 2:
//		_ubik_hrtick_htimer_p 	= TC2;
//		_ubik_hrtick_htimer_id 	= TC2;
		break;
	default:
		return -1;
	}

	bsp_getmckfreqk(&mckfreqk);
#if		(0 == TICK_PER_SEC%1000)
	_ubik_hrtick_hrtickpertick = mckfreqk / HRTICK_DIV *    1 / (TICK_PER_SEC / 1000);
#elif	(0 == TICK_PER_SEC%100)
	_ubik_hrtick_hrtickpertick = mckfreqk / HRTICK_DIV *   10 / (TICK_PER_SEC /  100);
#elif	(0 == TICK_PER_SEC%10)
	_ubik_hrtick_hrtickpertick = mckfreqk / HRTICK_DIV *  100 / (TICK_PER_SEC /   10);
#else
	_ubik_hrtick_hrtickpertick = mckfreqk / HRTICK_DIV * 1000 / (TICK_PER_SEC /    1);
#endif

	// Enable peripheral clock
	PMC->PMC_PCER0 = 1 << _ubik_hrtick_htimer_id;
	// Disable TC
	_ubik_hrtick_htimer_p->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;//AT91C_TC_CLKDIS;
	// Disable interrupts
	_ubik_hrtick_htimer_p->TC_CHANNEL[0].TC_IDR = 0xFFFFFFFF;
	// Clear status register
	_ubik_hrtick_htimer_p->TC_CHANNEL[0].TC_SR;
	// Set mode
	_ubik_hrtick_htimer_p->TC_CHANNEL[0].TC_CMR = (TC_CMR_TCCLKS_TIMER_CLOCK1 | TC_CMR_WAVE);
	// Reset and enable TC
	_ubik_hrtick_htimer_p->TC_CHANNEL[0].TC_CCR = (TC_CCR_CLKEN | TC_CCR_SWTRG);
	// Disable TC
	_ubik_hrtick_htimer_p->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;

	_ubik_hrtick_active 		= 1;

#else
	if (0 != _ubik_hrtick_active) {
		return -1;
	}

	switch (htimerno) {
	case 0:
		_ubik_hrtick_htimer_p 	= AT91C_BASE_TC0;
		_ubik_hrtick_htimer_id 	= AT91C_ID_TC0;
		break;
	case 1:
		_ubik_hrtick_htimer_p 	= AT91C_BASE_TC1;
		_ubik_hrtick_htimer_id 	= AT91C_ID_TC1;
		break;
	case 2:
		_ubik_hrtick_htimer_p 	= AT91C_BASE_TC2;
		_ubik_hrtick_htimer_id 	= AT91C_ID_TC2;
		break;
	default:
		return -1;
	}

	bsp_getmckfreqk(&mckfreqk);
#if		(0 == TICK_PER_SEC%1000)
	_ubik_hrtick_hrtickpertick = mckfreqk / HRTICK_DIV *    1 / (TICK_PER_SEC / 1000);
#elif	(0 == TICK_PER_SEC%100)
	_ubik_hrtick_hrtickpertick = mckfreqk / HRTICK_DIV *   10 / (TICK_PER_SEC /  100);
#elif	(0 == TICK_PER_SEC%10)
	_ubik_hrtick_hrtickpertick = mckfreqk / HRTICK_DIV *  100 / (TICK_PER_SEC /   10);
#else
	_ubik_hrtick_hrtickpertick = mckfreqk / HRTICK_DIV * 1000 / (TICK_PER_SEC /    1);
#endif

	// Enable peripheral clock
	AT91C_BASE_PMC->PMC_PCER = 1 << _ubik_hrtick_htimer_id;
	// Disable TC
	_ubik_hrtick_htimer_p->TC_CCR = AT91C_TC_CLKDIS;
	// Disable interrupts
	_ubik_hrtick_htimer_p->TC_IDR = 0xFFFFFFFF;
	// Clear status register
	_ubik_hrtick_htimer_p->TC_SR;
	// Set mode
	_ubik_hrtick_htimer_p->TC_CMR = (HRTICK_DIV_TYPE | AT91C_TC_WAVESEL_UP | AT91C_TC_WAVE);
	// Reset and enable TC
	_ubik_hrtick_htimer_p->TC_CCR = (AT91C_TC_CLKEN | AT91C_TC_SWTRG);
	// Disable TC
	//_ubik_hrtick_htimer_p->TC_CCR = AT91C_TC_CLKDIS;

	_ubik_hrtick_active 		= 1;
#endif
	return 0;
}

int ubik_hrtick_disable(void) {
#if defined(__SAM4E16E__)
	if (1 != _ubik_hrtick_active) {
		return -1;
	}

	// Disable TC
	_ubik_hrtick_htimer_p->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;
//	TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;

	_ubik_hrtick_active = 0;
#else
	if (1 != _ubik_hrtick_active) {
		return -1;
	}

	// Disable TC
	_ubik_hrtick_htimer_p->TC_CCR = AT91C_TC_CLKDIS;

	_ubik_hrtick_active = 0;
#endif
	return 0;
}

int ubik_hrtick_reset(void) {
#if defined(__SAM4E16E__)
	// Disable TC
	_ubik_hrtick_htimer_p->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;
	// Reset and enable TC
	_ubik_hrtick_htimer_p->TC_CHANNEL[0].TC_CCR = (TC_CCR_CLKEN | TC_CCR_SWTRG);
#else
	// Disable TC
	_ubik_hrtick_htimer_p->TC_CCR = AT91C_TC_CLKDIS;
	// Reset and enable TC
	_ubik_hrtick_htimer_p->TC_CCR = (AT91C_TC_CLKEN | AT91C_TC_SWTRG);
#endif
	return 0;
}

unsigned int ubik_hrtick_gettickpersec(void) {
	return TICK_PER_SEC;
}

unsigned int ubik_hrtick_gethrtickpermsec(void) {
	unsigned int value;
	if (1 != _ubik_hrtick_active) {
		return -1;
	}

#if		(0 == TICK_PER_SEC%1000)
	value = _ubik_hrtick_hrtickpertick * (TICK_PER_SEC / 1000);
#elif	(0 == TICK_PER_SEC%100)
	value = _ubik_hrtick_hrtickpertick * (TICK_PER_SEC  / 100) /   10;
#elif	(0 == TICK_PER_SEC%10)
	value = _ubik_hrtick_hrtickpertick * (TICK_PER_SEC   / 10) /  100;
#else
	value = _ubik_hrtick_hrtickpertick * (TICK_PER_SEC       ) / 1000;
#endif

	return value;
}

int ubik_hrtick_gettick(hrtick_t * tick_p) {
#if defined(__SAM4E16E__)
	if (NULL == tick_p) {
		return -2;
	}
	if (1 != _ubik_hrtick_active) {
		return -1;
	}

	ubik_entercrit();

	tick_p->high	= _ubik_tickcounth;
	tick_p->low		= _ubik_tickcount;
	tick_p->hrtick	= _ubik_hrtick_htimer_p->TC_CHANNEL[0].TC_CV;

	ubik_exitcrit();
#else
	if (NULL == tick_p) {
		return -2;
	}
	if (1 != _ubik_hrtick_active) {
		return -1;
	}

	ubik_entercrit();

	tick_p->high	= _ubik_tickcounth;
	tick_p->low		= _ubik_tickcount;
	tick_p->hrtick	= _ubik_hrtick_htimer_p->TC_CV;

	ubik_exitcrit();
#endif
	return 0;
}

int ubik_hrtick_gettickdiff(hrtick_t * tick1_p, hrtick_t * tick2_p, hrtick_t * tickdiff_p) {
	if (NULL == tick1_p) {
		return -2;
	}
	if (NULL == tick2_p) {
		return -3;
	}
	if (NULL == tickdiff_p) {
		return -4;
	}
	if (1 != _ubik_hrtick_active) {
		return -1;
	}

	if (tick1_p->high > tick2_p->high) {
		tickdiff_p->high = UINT_MAX - tick1_p->high + 1 + tick2_p->high;
	}
	else {
		tickdiff_p->high = tick2_p->high - tick1_p->high;
	}

	if (tick1_p->low > tick2_p->low) {
		tickdiff_p->high--;
		tickdiff_p->low = UINT_MAX - tick1_p->low + 1 + tick2_p->low;
	}
	else {
		tickdiff_p->low = tick2_p->low - tick1_p->low;
	}

	if (tick1_p->hrtick > tick2_p->hrtick) {
		if (0 >= tickdiff_p->low) {
			tickdiff_p->high--;
			tickdiff_p->low = UINT_MAX;
		}
		else {
			tickdiff_p->low--;
		}
		tickdiff_p->hrtick = _ubik_hrtick_hrtickpertick - tick1_p->hrtick + 1 + tick2_p->hrtick;
	}
	else {
		tickdiff_p->hrtick = tick2_p->hrtick - tick1_p->hrtick;
	}

	return 0;
}

int ubik_hrtick_hrticktotime(hrtick_t * tick_p, unsigned int * nsec_p, unsigned int * usec_p, unsigned int * msec_p) {
	unsigned int timems;
	unsigned int timeus;
	unsigned int timens;
	unsigned int temp;
	unsigned int tickperxx;

	if (NULL == tick_p) {
		return -2;
	}

#if		(0 == TICK_PER_SEC%1000)
	tickperxx	 = (TICK_PER_SEC /   1000);
	temp		 = (tick_p->low *    1);
#elif	(0 == TICK_PER_SEC%100)
	tickperxx	 = (TICK_PER_SEC /    100);
	temp		 = (tick_p->low *   10);
#elif	(0 == TICK_PER_SEC%10)
	tickperxx	 = (TICK_PER_SEC /     10);
	temp		 = (tick_p->low *  100);
#else
	tickperxx	 = (TICK_PER_SEC /      1);
	temp		 = (tick_p->low * 1000);
#endif
	timems		 = (temp / tickperxx);
	temp		 = (temp % tickperxx) * 1000;
	timeus		 = (temp / tickperxx);
	temp		 = (temp % tickperxx) * 1000;
	timens		 = (temp / tickperxx);

	tickperxx	 = ubik_hrtick_gethrtickpermsec();
	temp		 = (tick_p->hrtick);
	timems		+= (temp / tickperxx);
	temp		 = (temp % tickperxx) * 1000;
	timeus		+= (temp / tickperxx);
	temp		 = (temp % tickperxx) * 1000;
	timens		+= (temp / tickperxx);

	if (NULL != nsec_p) {
		*nsec_p = timens;
	}
	if (NULL != usec_p) {
		*usec_p = timeus;
	}
	if (NULL != msec_p) {
		*msec_p = timems;
	}

	return 0;
}

int ubik_hrtick_hrticktotimens(hrtick_t * tick_p, unsigned int * nsec_p) {
	unsigned int time;
	unsigned int temp;
	unsigned int tickperxx;

	if (NULL == tick_p) {
		return -2;
	}

#if		(0 == TICK_PER_SEC%1000)
	tickperxx	 = (TICK_PER_SEC /   1000);
	temp		 = (tick_p->low *    1);
#elif	(0 == TICK_PER_SEC%100)
	tickperxx	 = (TICK_PER_SEC /    100);
	temp		 = (tick_p->low *   10);
#elif	(0 == TICK_PER_SEC%10)
	tickperxx	 = (TICK_PER_SEC /     10);
	temp		 = (tick_p->low *  100);
#else
	tickperxx	 = (TICK_PER_SEC /      1);
	temp		 = (tick_p->low * 1000);
#endif
	time		 = (temp * 1000 * 10 / tickperxx * 100);

	tickperxx	 = ubik_hrtick_gethrtickpermsec();
	temp		 = (tick_p->hrtick);

	time		+= (temp * 1000 * 10 / tickperxx * 100);

	if (NULL != nsec_p) {
		*nsec_p = time;
	}

	return 0;
}

int ubik_hrtick_hrticktotimeus(hrtick_t * tick_p, unsigned int * usec_p) {
	unsigned int time;
	unsigned int temp;
	unsigned int tickperxx;

	if (NULL == tick_p) {
		return -2;
	}

#if		(0 == TICK_PER_SEC%1000)
	tickperxx	 = (TICK_PER_SEC /   1000);
	temp		 = (tick_p->low *    1);
#elif	(0 == TICK_PER_SEC%100)
	tickperxx	 = (TICK_PER_SEC /    100);
	temp		 = (tick_p->low *   10);
#elif	(0 == TICK_PER_SEC%10)
	tickperxx	 = (TICK_PER_SEC /     10);
	temp		 = (tick_p->low *  100);
#else
	tickperxx	 = (TICK_PER_SEC /      1);
	temp		 = (tick_p->low * 1000);
#endif
	time		 = (temp * 1000 / tickperxx);

	tickperxx	 = ubik_hrtick_gethrtickpermsec();
	temp		 = (tick_p->hrtick);

	time		+= (temp * 1000 / tickperxx);

	if (NULL != usec_p) {
		*usec_p = time;
	}

	return 0;
}

#endif /* (1 != EXCLUDE_HRTICK) */
