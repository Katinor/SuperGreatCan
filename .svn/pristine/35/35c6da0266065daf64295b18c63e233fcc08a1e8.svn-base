/*
 * lib_timer.h
 *
 *  Created on: 2015. 6. 1.
 *      Author: geoyun
 */

#ifndef KERNEL_DRIVER_LIB_TIMER_ITF_LIB_TIMER_H_
#define KERNEL_DRIVER_LIB_TIMER_ITF_LIB_TIMER_H_


#include "tc/tc.h"

#define	TIMER_TC0	1
#define	TIMER_TC1	2
#define	TIMER_TC2	3

#define	TIMER_TCCHANNEL0	0
#define	TIMER_TCCHANNEL1	1
#define	TIMER_TCCHANNEL2	2
#define	TIMER_TCCHANNEL3	3
#define	TIMER_TCCHANNEL4	4
#define	TIMER_TCCHANNEL5	5
#define	TIMER_TCCHANNEL6	6
#define	TIMER_TCCHANNEL7	7
#define	TIMER_TCCHANNEL8	8

struct {
	uint32_t high;
	uint32_t low;
}typedef timer_st;

void timer_init(uint8_t pTc_index, uint8_t pTc_channel_index, uint32_t timer_tc_rc);
int timer_start(void);
int timer_end(timer_st *pTimer_pt);

#endif /* KERNEL_DRIVER_LIB_TIMER_ITF_LIB_TIMER_H_ */
