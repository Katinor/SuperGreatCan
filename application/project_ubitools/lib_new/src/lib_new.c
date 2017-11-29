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

#include "lib_new_estk_api/itf/new_estk_led.h"
#include "lib_new_estk_api/itf/new_estk_glcd.h"
#include "../itf/lib_new.h"

void lib_default_component_init( void )
{
	printf("library default test\n\r");
}

void print_add_lib_success(void)
{
	printf("Add library success!\r\n");
}

int mylib_comp_init(void)
{
	led_init();
	glcd_init();
	return 0;
}
void led_test(void)
{
	led_on(LED1);
	task_sleep(1000);
	led_on(LED2);
	task_sleep(1000);
	led_on(LED3);
	task_sleep(1000);
}

void lcd_test(void)
{
	glcd_clear();

	glcd_printf("LCD TEST\n");
	glcd_printf("UBINOS\n");
	glcd_printf("HELLO WORLD!!!\n");

	task_sleep(1500);

	glcd_clear();

}
void speedy_test(void)
{
	glcd_clear();
	led_on(LED1);
	led_on(LED2);
	led_on(LED3);
	glcd_printf("LCD TEST\n");
	glcd_printf("UBINOS\n");
	glcd_printf("HELLO WORLD!!!\n");
	task_sleep(500);
	led_off(LED1);
	led_off(LED2);
	led_off(LED3);
	task_sleep(500);
	led_on(LED1);
	led_on(LED2);
	led_on(LED3);
	task_sleep(500);
	led_off(LED1);
	led_off(LED2);
	led_off(LED3);
	task_sleep(500);
	led_on(LED1);
	led_on(LED2);
	led_on(LED3);
	task_sleep(1000);
	led_off(LED1);
	led_off(LED2);
	led_off(LED3);
	glcd_clear();
}
