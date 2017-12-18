/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the exe_helloworld component of the Ubinos.

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

/* -------------------------------------------------------------------------
	Include
 ------------------------------------------------------------------------- */
#include "../ubiconfig.h"

// standard c library include
#include <stdio.h>
#include <sam4e.h>

// ubinos library include
#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"
#include "itf_ubinos/itf/bsp_fpu.h"

// chipset driver include
#include "ioport.h"
#include "pio/pio.h"

// new estk driver include
#include "lib_new_estk_api/itf/new_estk_led.h"
#include "lib_new_estk_api/itf/new_estk_glcd.h"
#include "lib_switch/itf/lib_switch.h"
#include "lib_bluetooth/itf/lib_BT.h"
#include "lib_bluetooth/itf/BT_Module_Interface.h"

#include "lib_EV3_sensor/itf/lib_EV3_sensor.h"
#include "lib_sensor/itf/lib_sensor.h"
#include "lib_motor_driver/itf/lib_motor_driver.h"
#include "../../lib_new/itf/lib_new.h"
#include "../../lib_sensorcalib/itf/lib_sensorcalib.h"
#include "lib_new_estk_api/itf/new_estk_ioport_set.h"
// custom library header file include

// user header file include

/* -------------------------------------------------------------------------
	Constant definition
 ------------------------------------------------------------------------- */
#define BLE_MODULE_ID_0	0X01
#define BLE_MODULE_ID_1	0X02
#define BLE_MODULE_ID_2	0X03
#define BLE_MODULE_ID_3	0X51
uint8_t print_packet[DATA_SEND_BUFFER_SIZE] = {0,};
msgq_pt BT_user_event_queue;
mutex_pt _g_mutex;
#define MOTOR_PORT   3
#define S_SENSOR_PORT	0
#define L_SENSOR_PORT	2
/* -------------------------------------------------------------------------
	Calibration Definition
 ------------------------------------------------------------------------- */
#define MOTOR_GAIN 1000
#define TIME_OUT 30
#define ROTATE_GAIN1 1
#define ROTATE_GAIN2 15
#define CAPACITY_GAIN 2
#define CAPACITY_OFFSET 5
/* -------------------------------------------------------------------------
	State Definition
 ------------------------------------------------------------------------- */
enum binState
{
	BIN_OPEN,
	BIN_CLOSE
};
enum binFull
{
	BIN_TRUE,
	BIN_75,
	BIN_50,
	BIN_25,
	BIN_0
};
/* -------------------------------------------------------------------------
	Task Handler
 ------------------------------------------------------------------------- */
static void lcd_outputtask(void * arg);
static void BT_peripheraltask(void * arg);
static void General_controltask(void * arg);
void bin_open(void);
void bin_close(void);
void bin_control(void);
void bin_status(void);
void bin_sync(void);
void bin_hard_open(void);
void bin_hard_close(void);
void motor_turn(int port, int degree);
void sw0_isr(void);
void sw1_isr(void);

/* -------------------------------------------------------------------------
	Global variables
 ------------------------------------------------------------------------- */
int r;
int motorset=0;
int binState=BIN_CLOSE;
int binCapacity=4;
int binFull=4;
int binFixedControl=0;
/* -------------------------------------------------------------------------
	Function Definitions
 ------------------------------------------------------------------------- */
int usrmain(int argc, char * argv[]) {

	printf("\n\n\n\r");
	printf("================================================================================\n\r");
	printf("exe_ubinos_test (build time: %s %s)\n\r", __TIME__, __DATE__);
	printf("================================================================================\n\r");
	printf("hello world!\n\r");
	printf("===== start to task process=====\r\n");

	glcd_init();
	motor_init();
	encoder_init();
	mutex_create(&_g_mutex);
	switch_init(sw0_isr,sw1_isr);
	sensor_init(NXT_DIGITAL_SENSOR_SONA, 0, 0, 0);
	printf("switch interrupt on");
	r = task_create(NULL, lcd_outputtask, NULL, task_getmiddlepriority(), 256, "lcd");
	if (0 != r) {
		logme("fail at task_create\r\n");
		printf("fail to create lcd_outputtask : %d\r\n", r);
	}
	r = task_create(NULL, BT_peripheraltask, NULL, task_getmiddlepriority(), 256, "pheri");
	if (0 != r) {
		logme("fail at task_create\r\n");
		printf("fail to create BT_petipheraltask : %d\r\n", r);
	}
	r = task_create(NULL, General_controltask, NULL, task_getmiddlepriority(), 256, "pheri");
	if (0 != r) {
		logme("fail at task_create\r\n");
		printf("fail to create General_controltask : %d\r\n", r);
	}
	r = msgq_create(&BT_user_event_queue, sizeof(BT_Evt_t), MAIN_MSGQ_MAX_COUNT);
	if (0 != r) {
		logme("fail at msgq_create\r\n");
		printf("fail to create BT_user_event_queue : %d\r\n", r);
	}
	ubik_comp_start();
	return 0;
}

static void lcd_outputtask(void * arg){
	task_sleep(300);
	while(1){
		mutex_lock(_g_mutex);
		//glcdGotoChar(1,5);
		//glcd_printf("binFull : %d", binFull);
		//glcdGotoChar(1,6);
		//glcd_printf("binState : %d", binState);
		mutex_unlock(_g_mutex);
		task_sleep(200);
	}
}
static void BT_peripheraltask(void * arg){
	r = 0;
	module_id_st BT_ID;
	BT_Evt_t BT_usr_msgRXBuffer = {0,};

	BT_ID.module_id[0] = BLE_MODULE_ID_0;
	BT_ID.module_id[1] = BLE_MODULE_ID_1;
	BT_ID.module_id[2] = BLE_MODULE_ID_2;
	BT_ID.module_id[3] = BLE_MODULE_ID_3;

	task_sleep(200);

	BT_INIT(INIT_ROLE_PERIPHERAL,BT_ID,BT_user_event_queue);

	BT_ADV_START();

	for(;;)
	{
		r = msgq_receive(BT_user_event_queue, (unsigned char*) &BT_usr_msgRXBuffer);
		if (0!=r)
		{
			logme("fail at msgq_receive\r\n");
		}
		else
		{
			switch (BT_usr_msgRXBuffer.status){
			case BT_EVT_PE_DATA_READ:
			{
				mutex_lock(_g_mutex);
				//memcmp
				motorset = (int)((BT_usr_msgRXBuffer.msg[1] << 8) | (BT_usr_msgRXBuffer.msg[0]));

				for(int i=0; i<20; i++){
					print_packet[i]=BT_usr_msgRXBuffer.msg[i];
				}
				mutex_unlock(_g_mutex);
				switch(motorset)
				{
				case 5:
					bin_hard_close();
					break;
				case 4:
					bin_hard_open();
					break;
				case 3:
					bin_control();
					break;
				case 2:
					bin_status();
					break;
				case 1:
					bin_sync();
					break;
				default:
					break;
				}
				//send same msg
				task_sleep(500);
			}
			break;

			case BT_EVT_DISCONNECTED:

				break;
			case BT_EVT_CONNECTED:

				break;

			}
		}
	}
}

static void General_controltask(void * arg){
	int dist[8];
	int dist_sum, dist_avg;
	for(;;)
	{
		if(binFixedControl==1)
		{
			if(binState == BIN_OPEN) bin_close();
			else bin_open();
			binFixedControl = 0;
		}

		if(binState == BIN_CLOSE){
			for(int j = 0; j<8; j++)
			{
				dist[j] = sensor_get(S_SENSOR_PORT);
				task_sleep(100);
			}
			dist_sum = 0;
			for(int j = 0; j<8; j++)
			{
				dist_sum += dist[j];
			}
			dist_avg = dist_sum / 8;
			if (binFull == BIN_TRUE)
			{
				led_on(LED3);
				if(dist_avg > CAPACITY_OFFSET+CAPACITY_GAIN*2)
				{
					binFull = BIN_50;
				}
			}
			else
			{
				led_off(LED3);
				if(dist_avg < CAPACITY_OFFSET+CAPACITY_GAIN*1) binFull = BIN_TRUE;
				else if(dist_avg < CAPACITY_OFFSET+CAPACITY_GAIN*2) binFull = BIN_75;
				else if(dist_avg < CAPACITY_OFFSET+CAPACITY_GAIN*3) binFull = BIN_50;
				else if(dist_avg < CAPACITY_OFFSET+CAPACITY_GAIN*4) binFull = BIN_25;
				else binFull = BIN_0;
			}
			printf("check capacity : dist %d level %d\r\n",dist_avg,binFull);
			task_sleep(200);
		}
		else task_sleep(1000);
	}
}

void bin_control()
{
	bin_status();
	if(binState == BIN_OPEN) bin_close();
	else bin_open();
	bin_status();
}

void bin_open()
{
	motor_turn(MOTOR_PORT,-90);
	binState = BIN_OPEN;
}

void bin_hard_open()
{
	binState = BIN_OPEN;
	bin_status();
	printf("Bin is opened currently\n\r");
}

void bin_hard_close()
{
	binState = BIN_CLOSE;
	bin_status();
	printf("Bin is closed currently\n\r");
}



void bin_close()
{
	motor_turn(MOTOR_PORT,90);
	binState = BIN_CLOSE;
}
void bin_status()
{
	int packet_buff;
	if(binState == BIN_CLOSE) printf("Bin is Closed currently\n\r");
	else printf("Bin is opened currently\n\r");
	print_packet[1] = binFull;
	print_packet[0] = binState;
	BT_DATA_SEND(INIT_ROLE_PERIPHERAL, print_packet);
}
void bin_sync()
{
	print_packet[1] = binFull;
	print_packet[0] = binState;
	BT_DATA_SEND(INIT_ROLE_PERIPHERAL, print_packet);
}
void motor_turn(int port, int degree)
{
	int timeout = 0;
	int diff_degree=0;
	int speed = 0;
	int buff = 0;
	int swt = 1;
	int deg_buf = degree;
	buff = encoder_get(port);
	printf("rotate %d ready : enc %d \r\n",degree,buff);
	encoder_reset(port);
	buff = encoder_get(port);
	printf("rotate %d start : enc %d\r\n",degree,buff);
	timeout = TIME_OUT;
	if (deg_buf > 0)
	{
		deg_buf *= -1;
		swt = -1;
	}
	do{
		buff = encoder_get(port) * -1 * swt;
		diff_degree = (buff) - (deg_buf * ROTATE_GAIN1);
		speed = ROTATE_GAIN2 * diff_degree * swt;
		printf("rotate %d progress : swt %d enc %d diff %d speed %d time %d \r\n",degree,swt,buff,diff_degree, speed, timeout);
		motor_set(port,speed);
		timeout--;
		bsp_busywaitms(1);
	} while (((diff_degree) > 0)&&(timeout != 0));
	buff = encoder_get(port);
	printf(" > rotate complete : enc %d\r\n",buff);
	motor_set(port,0);
}

void sw0_isr(void){
	binFixedControl=1;
}

void sw1_isr(void){
	led_toggle(LED2);
}
