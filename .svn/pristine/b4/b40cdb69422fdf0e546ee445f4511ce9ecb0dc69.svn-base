/*
 * lib_ubi_power_manager.c
 *
 *  Created on: 2015. 6. 20.
 *      Author: geoyun
 */

#include "../itf/lib_lowpower.h"

//#include <stdio.h>
#include <sam4e.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/bsp_dtty.h"
#include "itf_ubinos/itf/ubinos.h"

#include "_bsp_board.h"

#include "../../lib_ubi_pio/itf/lib_ubi_pio.h"

#include "gcc/asf.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "conf_uart_serial.h"
#include "low_power_board.h"

#define		RTC_CLOCK_PER_TICK		33

#define		SYSTEM_TICK_OVERFLOW	-1

//static uint32_t sleepMode = EM2;
static uint32_t sleepMode = UBIK_SAM_WAITMODE;
static uint32_t updateTick = 0;
static uint32_t previousCount = 0;

void RTT_Handler(void)
{
	uint32_t ul_status;

	/*Get RTT status*/
	ul_status = rtt_get_status(RTT);

	/*Alarm*/
	if((ul_status & RTT_SR_ALMS) == RTT_SR_ALMS)
	{
	}
}

static uint32_t Power_Get_Wakeup_Tick(_task_pt taskPtr, _stimer_pt stimerPtr)
{
	uint32_t taskWakeupTick = 0;
	uint32_t stimerWakeupTick = 0;

	uint32_t wakeupTick = 0;

	taskWakeupTick = taskPtr->wakeuptick;
	stimerWakeupTick = stimerPtr->wakeuptick;

	if((taskPtr == NULL) && (stimerPtr != NULL))
	{
		wakeupTick = stimerWakeupTick;
	}
	else if((taskPtr != NULL) && (stimerPtr == NULL))
	{
		wakeupTick = taskWakeupTick;
	}
	else if((taskPtr != NULL) && (stimerPtr != NULL))
	{
		wakeupTick = (taskWakeupTick > stimerWakeupTick) ? stimerWakeupTick : taskWakeupTick;
	}

	wakeupTick -= _ubik_tickcount;

	return wakeupTick;
}

static int Power_Check_Tick_Overflow(void)
{
	_task_pt nextTaskPtr = NULL;
	_stimer_pt nextStimerPtr = NULL;

	nextTaskPtr = _tasklist_head(_task_list_blocked_timed_next);
	nextStimerPtr = _tasklist_head(_stimer_list_next);

	if((nextTaskPtr != 0) || (nextStimerPtr != 0))
	{
		updateTick = 1;

		return -1;
	}

	return 0;
}

static void Power_Enter_Periodic_Sleep(uint32_t sleepTick)
{
	uint16_t _tick;

	_tick = 32*sleepTick;
	_ubik_tickcount += (sleepTick - 1);
//	RTC_CompareSet(0, RTC_CLOCK_PER_TICK * sleepTick - 1);
		/* Configure RTT for a 1 second tick interrupt */
		rtt_sel_source(RTT, false);
		rtt_init(RTT, _tick);

		/* Enable RTT interrupt */
		NVIC_DisableIRQ(RTT_IRQn);
		NVIC_ClearPendingIRQ(RTT_IRQn);
		//NVIC_SetPriority(RTT_IRQn, 0);
		intr_connectisr(RTT_IRQn, RTT_Handler, intr_gethighestpriority(), 0);
		NVIC_EnableIRQ(RTT_IRQn);
		rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);
		rtt_write_alarm_time(RTT, 3);

		ubi_wait_mode(0, WAIT_STARTUP_FAST, UBIK_SAM_RTT_WAKEUP);
//		ubi_wait_mode(PIN_WKUP_14, WAIT_STARTUP_FAST, UBIK_SAM_PIN_WAKEUP|UBIK_SAM_RTT_WAKEUP);

//	EMU_EnterEM2(true);
}
static void Power_Enter_Aperiodic_Sleep(void)
{
	ubi_wait_mode(PIN_WKUP_14, WAIT_STARTUP_FAST, UBIK_SAM_PIN_WAKEUP);

//	previousCount = RTC_CounterGet();

//	EMU_EnterEM2(true);

//	RTC->CNT = previousCount;
//	RTC_Enable(true);
}

static void Power_Manager_Task(void)
{
	uint32_t r;
	uint32_t sleepTick;

	_task_pt taskPtr = NULL;
	_stimer_pt stimerPtr = NULL;

	for(;;)
	{
		if((sleepMode == UBIK_SAM_SLEEPMODE) || (updateTick))
		{
//			EMU_EnterEM1();
//			task_sleep(100);
		}
		else if(sleepMode == UBIK_SAM_WAITMODE)
		{
			taskPtr = _tasklist_head(_task_list_blocked_timed_cur);
			stimerPtr = _tasklist_head(_stimer_list_cur);

			if((taskPtr == NULL) && (stimerPtr == NULL))
			{
				r = Power_Check_Tick_Overflow();
				if(r != SYSTEM_TICK_OVERFLOW)
				{
					Power_Enter_Aperiodic_Sleep();
				}
			}
			else
			{
				sleepTick = Power_Get_Wakeup_Tick(taskPtr, stimerPtr);
				if(sleepTick > 0)
				{
					Power_Enter_Periodic_Sleep(sleepTick);
				}
			}
		}
	}
}

static Power_Tick_ISR_Hookfunc(void)
{
	updateTick = 0;
}

void Power_Manager_Init(void)
{
	int r;

	r = ubik_setidletaskhookfunc(Power_Manager_Task, NULL, "Power", IDLEHOOKFUNC_OPT__ONCE);
	if (0 != r) {
		logme("fail at ubik_setidletaskhookfunc\r\n");
	}

	r = ubik_settickhookfunc(Power_Tick_ISR_Hookfunc);
	if (0 != r) {
		logme("fail at ubik_settickhookfunc\r\n");
	}

}

void Power_Update_Tickcount(uint32_t type)
{
	uint32_t tarTick;
	uint32_t curTick;

//	if(!(RTC_IntGet() & RTC_IF_COMP0))
//	{
//		tarTick = ((RTC_CompareGet(0) + 1) / RTC_CLOCK_PER_TICK) - 1;
//		curTick = (RTC_CounterGet() + 1) / RTC_CLOCK_PER_TICK;
//
//		if((tarTick - curTick) != 0 )
//		{
//			updateTick = 1;
//			_ubik_tickcount -= (tarTick - curTick);
//
//			RTC_CompareSet(0, (RTC_CLOCK_PER_TICK * (curTick + 1)) - 1);
//		}
//	}
}

void Power_Change_Energy_Mode(uint32_t mode)
{
	if((mode == UBIK_SAM_SLEEPMODE) || (mode == UBIK_SAM_WAITMODE))
	{
		sleepMode = mode;
	}
}





