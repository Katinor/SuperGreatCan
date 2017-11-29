/*
 * lib_default.c
 *
 *  Created on: 2015. 7. 8.
 *      Author: jaegeun
 */

#include <stdio.h>
#include <sam4e.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"

#include "lib_switch/itf/lib_switch.h"
#include "lib_new_estk_api/itf/new_estk_glcd.h"
#include "lib_sensor/itf/lib_sensor.h"
#include "lib_EV3_sensor/itf/lib_EV3_sensor.h"

#include "../itf/lib_sensorcalib.h"

void lib_sensorcalib_component_init( void )
{
	printf("library sensorcalib test\n\r");
}

void calibSensor(char port, char states, int * value_p)
{
	int i, val;
	switch_init(NULL, NULL);

	glcd_clear();
	glcd_printf("NXT CALIB PORT[%d] START..", port);
	task_sleep(100);

	for(i=0; i<states; i++)
	{
		glcd_clear();\
		task_sleep(200);
		/* message of calibration used SW2 */
		do{ //SW2 opened
			glcdGotoChar(0, 1);
			val = sensor_get(port);
			glcd_printf("PRESS SW2 VAL: %d   ", val);
		}while(switch_get(ESTK_SW2_PORT) != 0);

		/* set the table's value */
		value_p[i] = val;

		/* message of setting value */
		glcdGotoChar(0, i+2);
		glcd_printf("STATE[%d] VAL : %d   ", i, val);
		task_sleep(500);

		/* finish calibration work */
		glcdGotoChar(0, 1);
		glcd_printf("FINISH PORT[%d] CALIB", port);
		glcdGotoChar(0, 7);
		glcd_printf("PRESS SW1 NEXT");
		while(switch_get(ESTK_SW1_PORT) != 0);
	}
}

void calibEV3Sensor(char port, char states, int * value_p)
{
	int i, val;
	switch_init(NULL, NULL);

	glcd_clear();
	glcd_printf("EV3 CALIB PORT[%d] START..", port);
	task_sleep(100);

	for(i=0; i<states; i++)
	{
		task_sleep(200);
		/* message of calibration used SW2 */
		do{ //SW2 opened
			glcdGotoChar(0, 1);
			val = ev3_sensor_get(port);
			glcd_printf("PRESS SW2 VAL: %d", val);
		}while(switch_get(ESTK_SW2_PORT) != 0);

		/* set the table's value */
		value_p[i] = val;

		/* message of setting value */
		glcdGotoChar(0, i+2);
		glcd_printf("STATE[%d] VAL : %d", i, val);
		task_sleep(500);

		/* finish calibration work */
		glcdGotoChar(0, 1);
		glcd_printf("FINISH PORT[%d] CALIB", port);
		glcdGotoChar(0, 7);
		glcd_printf("PRESS SW1 NEXT");
		while(switch_get(ESTK_SW1_PORT) != 0);
	}
}

int get_level(int value, char state, int *value_p)
{
	int i, min, tmp, level;
	min = value;
	for(i =0; i<state; i++)
	{
		tmp = abs(value_p[i] - value);
		if(tmp <= min){
			min = tmp;
			level = i;
		}
	}
	return level;
}
