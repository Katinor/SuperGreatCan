/*
  Copyright (C) 2011 RTLab, Yu Jin Park, Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the lib_ubik_armcortexm_sam3sek component of the Ubinos.

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

unsigned int ubik_gettickpersec(void) {
	return TICK_PER_SEC;
}

unsigned int ubik_timemstotick(unsigned int timems) {
	if (0 == timems) {
		return 0;
	}

#if		(0 == TICK_PER_SEC%1000)
	timems = timems * (TICK_PER_SEC / 1000);
#elif	(0 == TICK_PER_SEC%100)
	timems = timems * (TICK_PER_SEC  / 100) /   10;
#elif	(0 == TICK_PER_SEC%10)
	timems = timems * (TICK_PER_SEC   / 10) /  100;
#else
	timems = timems * (TICK_PER_SEC       ) / 1000;
#endif

	if (0 == timems) {
		return 1;
	}

	return timems;
}

unsigned int ubik_ticktotimems(unsigned int tick) {
	if (0 == tick) {
		return 0;
	}

#if		(0 == TICK_PER_SEC%1000)
	tick = tick        / (TICK_PER_SEC / 1000);
#elif	(0 == TICK_PER_SEC%100)
	tick = tick * 10   / (TICK_PER_SEC  / 100);
#elif	(0 == TICK_PER_SEC%10)
	tick = tick * 100  / (TICK_PER_SEC   / 10);
#else
	tick = tick * 1000 / (TICK_PER_SEC       );
#endif

	if (0 == tick) {
		return 1;
	}

	return tick;
}

void ubik_tickisr(void) {
	unsigned int 	status = 0;
	edlist_pt		tempedlist;
	_task_pt		task;
	#if (1 != EXCLUDE_STIMER)
	_stimer_pt		stimer;
	#endif /* (1 != EXCLUDE_STIMER) */
	_wtask_pt		wtask;
	_sigobj_pt 		sigobj;
	int				i;
#if       (1 != EXCLUDE_HRTICK_TICKISR_DELAY_CHECK)
	unsigned int hrtick;
#endif /* (1 != EXCLUDE_HRTICK_TICKISR_DELAY_CHECK) */

	status = SysTick->CTRL;
	if (0 != (0x00010000 & status)) {


		////////////////

		_ubik_tickcount++;

#if (1 != EXCLUDE_HRTICK)
		if (0 != _ubik_hrtick_active) {
#if       (1 != EXCLUDE_HRTICK_TICKISR_DELAY_CHECK)
			hrtick	= _ubik_hrtick_htimer_p->TC_CHANNEL[_ubik_hrtick_htimer_ch]->TC_CV;
			if (hrtick > (_ubik_hrtick_hrtickpertick * 2)) {
				dtty_puts("ubik_tickisr: interrupt was delayed\r\n", 80);
				bsp_abortsystem();
			}
#endif /* (1 != EXCLUDE_HRTICK_TICKISR_DELAY_CHECK) */
			// Reset and enable TC
	#if defined(__SAM4E16E__)

	#else
			_ubik_hrtick_htimer_p->TC_CHANNEL[_ubik_hrtick_htimer_ch].TC_CCR = (TC_CCR0_CLKEN | TC_CCR0_SWTRG);
	#endif
		}
#endif /* (1 != EXCLUDE_HRTICK) */

		////////////////

		if (0 == _ubik_tickcount) {
			_ubik_tickcounth++;

			tempedlist						= _task_list_blocked_timed_cur;
			_task_list_blocked_timed_cur	= _task_list_blocked_timed_next;
			_task_list_blocked_timed_next	= tempedlist;

			#if (1 != EXCLUDE_STIMER)
			tempedlist						= _stimer_list_cur;
			_stimer_list_cur				= _stimer_list_next;
			_stimer_list_next				= tempedlist;
			#endif
		}

		for (;;) {
			task = _tasklist_head(_task_list_blocked_timed_cur);
			if (NULL != task && _ubik_tickcount >= task->wakeuptick) {
				for (i=0; i<task->wtask_count; i++) {
					wtask = &task->wtask_p[i];
					sigobj = _sigobj_wtasklist_owner(wtask);
					if (NULL != sigobj) {
						wtask->sigtype	= SIGOBJ_SIGTYPE__TIMEOUT;
						task->wtask_recvcount++;
						_sigobj_wtasklist_remove(wtask);
						_sigobj_wtasklist_notifychange(sigobj);
					}
				}
				task->state				= TASK_STATE__READY;
				task->wakeuptick		= 0;
				_task_changelist(task);
			}
			else {
				break;
			}
		}

#if       (1 != EXCLUDE_STIMER)
		for (;;) {
			stimer = _stimerlist_head(_stimer_list_cur);
			if (NULL != stimer && _ubik_tickcount >= stimer->wakeuptick) {
				switch (stimer->sigobj->type) {
				case OBJTYPE__UBIK_SEM:
					sem_give((sem_pt) stimer->sigobj);
					if (0 == stimer->oneshot) {
						_stimerlist_add(stimer);
					}
					else {
						_stimerlist_remove(stimer);
						stimer->running = 0;
					}
					break;
				case OBJTYPE__UBIK_SIGNAL:
					if (0 == stimer->broadcast) {
						signal_send((signal_pt) stimer->sigobj, stimer->sigtype);
					} else {
						signal_broadcast((signal_pt) stimer->sigobj, stimer->sigtype);
					}
					if (0 == stimer->oneshot) {
						_stimerlist_add(stimer);
					}
					else {
						_stimerlist_remove(stimer);
						stimer->running = 0;
					}
					break;
				default:
					_stimerlist_remove(stimer);
					stimer->running = 0;
					break;
				}
			}
			else {
				break;
			}
		}
#endif /* (1 != EXCLUDE_STIMER) */

		////////////////

#if       (1 != EXCLUDE_TICK_HOOKFUNC)
		if (NULL != _ubik_tickhookfunc_func) {
			_ubik_tickhookfunc_func();
		}
#endif /* (1 != EXCLUDE_TICK_HOOKFUNC) */

		////////////////

		if (0 == _ubik_tasklockcount && 0 != _ubik_active) {
			_task_prev 			= _task_cur;
			_task_cur->state	= TASK_STATE__READY;
			_task_cur			= _tasklist_getcurnext(_task_list_ready_cur);
			_task_cur->state	= TASK_STATE__RUNNING;
		}

		//////////////////

	}
}

int ubik_inittick(void) {
	int r;
	unsigned int clockfreqk = 0;
	unsigned int counter = 0;
	unsigned int tickpersec = 0;

	_ubik_tickcount 	= 0;
	_ubik_tickcounth	= 0;

	r = bsp_getmckfreqk(&clockfreqk);
	if (0 != r) {
		return -1;
	}
	tickpersec = ubik_gettickpersec();
	counter = (clockfreqk * 1000 / tickpersec) - 1;
	
	if (SysTick_LOAD_RELOAD_Msk < counter) {
		return -1;
	}

	SysTick->LOAD = counter;
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
					SysTick_CTRL_TICKINT_Msk   |
					SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */

	return 0;
}

//
///**
// * @brief  Initialize and start the SysTick counter and its interrupt.
// *
// * @param  uint32_t ticks is the number of ticks between two interrupts
// * @return  none
// *
// * Initialise the system tick timer and its interrupt and start the
// * system tick timer / counter in free running mode to generate
// * periodical interrupts.
// */
//static __INLINE uint32_t SysTick_Config(uint32_t ticks)
//{
//  if (ticks > SYSTICK_MAXCOUNT)  return (1);                                                /* Reload value impossible */
//
//  SysTick->LOAD  =  (ticks & SYSTICK_MAXCOUNT) - 1;                                         /* set reload register */
//  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);                               /* set Priority for Cortex-M0 System Interrupts */
//  SysTick->VAL   =  (0x00);                                                                 /* Load the SysTick Counter Value */
//  SysTick->CTRL = (1 << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE) | (1<<SYSTICK_TICKINT);    /* Enable SysTick IRQ and SysTick Timer */
//  return (0);                                                                               /* Function successful */
//}
//
///** \brief  System Tick Configuration
//
//    This function initialises the system tick timer and its interrupt and start the system tick timer.
//    Counter is in free running mode to generate periodical interrupts.
//
//    \param [in]  ticks  Number of ticks between two interrupts
//    \return          0  Function succeeded
//    \return          1  Function failed
// */
//static __INLINE uint32_t SysTick_Config(uint32_t ticks)
//{
//  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
//
//  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
//  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
//  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
//  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
//                   SysTick_CTRL_TICKINT_Msk   |
//                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
//  return (0);                                                  /* Function successful */
//}




