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
#define MOTOR_PORT   0
#define S_SENSOR_PORT	0
#define L_SENSOR_PORT	2
/* -------------------------------------------------------------------------
	Calibration Definition
 ------------------------------------------------------------------------- */
#define MOTOR_GAIN 1000
#define TIME_OUT 1000
#define ROTATE_GAIN1 1
#define ROTATE_GAIN2 200
/* -------------------------------------------------------------------------
	State Definition
 ------------------------------------------------------------------------- */
enum binState
{
	BIN_OPEN,
	BIN_CLOSE
};
/* -------------------------------------------------------------------------
	Task Handler
 ------------------------------------------------------------------------- */
static void lcd_outputtask(void * arg);
static void BT_peripheraltask(void * arg);
void bin_open(void);
void bin_close(void);
void bin_control(void);
void bin_status(void);

/* -------------------------------------------------------------------------
	Global variables
 ------------------------------------------------------------------------- */
int r;
int motorset=0;
int binState=BIN_CLOSE;
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
	mutex_create(&_g_mutex);

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
		//2nd line
		glcdGotoChar(1,2);
		glcd_printf("%d", motorset);

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
				if(motorset == 1) bin_control();
				if(motorset == 1) bin_status();
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

void bin_close()
{
	motor_turn(MOTOR_PORT,90);
	binState = BIN_CLOSE;
}
void bin_status()
{
	print_packet[0] = binState;
	BT_DATA_SEND(INIT_ROLE_PERIPHERAL, print_packet);
}

void motor_turn(int port,int degree)
{
	int timeout = 0;
	int diff_degree=0;
	int speed = 0;

	printf("rotate start : %d \r\n",degree);
	timeout = TIME_OUT;
	encoder_reset(port);
	do{
		diff_degree = (encoder_get(port)) + (degree * ROTATE_GAIN1);
		speed = ROTATE_GAIN2 * diff_degree;
		motor_set(port,speed);
		timeout--;
		bsp_busywaitms(10);
	} while ((diff_degree != 0)&&(timeout != 0));
	printf(" > rotate complete\r\n");
	motor_set(port,0);
}
