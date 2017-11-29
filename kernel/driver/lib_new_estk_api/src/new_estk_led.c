/*
 * new_estk_led.c
 *
 *  Created on: 2015. 7. 8.
 *      Author: jaegeun
 */

#include <stdio.h>
#include <sam4e.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"

#include "../itf/new_estk_led.h"

#include "sam_gpio/sam_gpio.h"

void led_init(void){
	gpio_configure_pin(LED1, LED1_FLAG);
	gpio_configure_pin(LED2, LED2_FLAG);
	gpio_configure_pin(LED3, LED3_FLAG);
}
void led_off(uint8_t led_port)
{
	switch(led_port)
	{
	case LED1 : gpio_set_pin_high(LED1);	break;
	case LED2 : gpio_set_pin_high(LED2);	break;
	case LED3 : gpio_set_pin_high(LED3);	break;
	default : break;
	}
}
void led_on(uint8_t led_port)
{
	switch(led_port)
	{
	case LED1 : gpio_set_pin_low(LED1);	break;
	case LED2 : gpio_set_pin_low(LED2);	break;
	case LED3 : gpio_set_pin_low(LED3);	break;
	default : break;
	}
}
void led_toggle(uint8_t led_port)
{
	switch(led_port)
	{
	case LED1 : gpio_toggle_pin(LED1);	break;
	case LED2 : gpio_toggle_pin(LED2);	break;
	case LED3 : gpio_toggle_pin(LED3);	break;
	default : break;
	}
}

void led_blink(uint8_t led_port)
{
	led_on(led_port);
	task_sleep(LED_BLINK_DELAY_INTERVAL);
	led_off(led_port);
}
