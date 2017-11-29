/*
 * lib_default.h
 *
 *  Created on: 2015. 9. 4.
 *      Author: Taemin
 */

#ifndef APPLICATION_LIB_SWITCH_ITF_LIB_SWITCH_H_
#define APPLICATION_LIB_SWITCH_ITF_LIB_SWITCH_H_


#include <stdio.h>
#include <stdint.h>

/** @define Switch0 configuration */
#define ESTK_SW1_PORT	0
/** @define Switch1 configuration */
#define ESTK_SW2_PORT	1

#define SW1 	0
#define SW2		1

int switch_get(int switch_num);
void switch_init(void *switch_isr1, void *switch_isr2);

#endif /* APPLICATION_LIB_SWITCH_ITF_LIB_SWITCH_H_ */
