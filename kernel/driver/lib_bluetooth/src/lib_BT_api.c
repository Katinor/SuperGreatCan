/*
 * BT_api_appl.c
 *
 *  Created on: 2017. 4. 6.
 *      Author: JKwang
 */
#include "../ubiconfig.h"

// standard c library include
#include <stdio.h>
#include <stdlib.h>
#include <sam4e.h>

// ubinos library include
#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"
#include "itf_ubinos/itf/bsp_fpu.h"

// chipset driver include
#include "lib_ubi_pio/itf/lib_ubi_pio.h"
#include "ioport.h"
#include "pio/pio.h"

// new estk driver include
#include "lib_new_estk_api/itf/new_estk_led.h"
#include "lib_new_estk_api/itf/new_estk_glcd.h"

#include "common/services/gpio/sam_gpio/sam_gpio.h"
#include "lib_new_estk_api/itf/new_estk_ioport_set.h"

#include "lib_bluetooth/itf/lib_BT.h"
#include "lib_bluetooth/itf/BT_Module_Interface.h"

//check BT_Module state
uint8_t check_ack_init_state = 0;

uint8_t check_ack_adv_state = 0;
uint8_t check_ack_adv_stop_state = 0;

uint8_t check_scan_done_state = 0;
uint8_t check_ack_scan_stop_state = 0;

uint8_t check_ack_connected_state = 0;
uint8_t check_ack_disconnected_state = 0;

uint8_t check_ack_data_send_state = 0;

uint8_t current_ack_wait_cmd = 0xFF;

static uint8_t temp_BTM_tx_buffer[BTM_INTERFACE_BUF_SIZE] = {0xFF, };
static module_id_st target_id ;
msgq_pt Main_Msgq;
msgq_pt temp_user_event_queue =NULL;

module_id_st temp_scan_result[MAX_SCAN_NUM];

extern uint8_t* R_packet,W_packet;
extern _mutex_tip_t mutex;


//--------------------------------------------------------------------------BLE USER API--------------

int8_t BT_INIT(uint8_t role, module_id_st setup_module_id, msgq_pt* user_event_queue)
{
	int r;
	int8_t result;
	check_ack_init_state = 0;
	uint8_t* temp_buffer = malloc(BTM_INTERFACE_BUF_SIZE);

	temp_user_event_queue = user_event_queue;

	task_sleep(1000);
	led_init();

	// 1st line
	glcd_clear();
	glcdGotoChar(1,1);
	if(role == INIT_ROLE_CENTRAL)
		glcd_printf("BT_ Central");
	else
		glcd_printf("BT_ Peripheral");


	PMC->PMC_PCER0 = 1 << ID_PIOA;

	gpio_configure_pin(CS, CS_FLAGS);
	gpio_configure_pin(RESET, RESET_FLAGS);

	//Blutooth_Module_reset
	gpio_set_pin_low(RESET);
	task_sleep(200);
	gpio_set_pin_high(RESET);
	task_sleep(300);

	//initialize packet
	memset(R_packet, 0x00, BTM_INTERFACE_BUF_SIZE);
	memset(W_packet, 0xFF, BTM_INTERFACE_BUF_SIZE);

	spi_init();

	BT_interrupt_setting(PIN_isr1);

	//message queue create : Main_Msgq
	r = msgq_create(&Main_Msgq, sizeof(BT_Evt_t), MAIN_MSGQ_MAX_COUNT);
	if (0 != r) {
		logme("fail at msgq_create\r\n");
	}

	//create_BT_control_task
	r = task_create(NULL, BT_control_task, NULL, task_getmiddlepriority(), 1024,	"root");
	if (0 != r) {
		logme("fail at BT_CTL_task_create\r\n");
	}

	memset(temp_buffer, 0xFF, BTM_INTERFACE_BUF_SIZE);

	//set packet : Init
	temp_buffer[INDEX_CMD] = CMD_INIT;
	temp_buffer[INDEX_INIT_ROLE] = role;

	temp_buffer[INDEX_INIT_DEVICE_ID0] = setup_module_id.module_id[0];
	temp_buffer[INDEX_INIT_DEVICE_ID1] = setup_module_id.module_id[1];
	temp_buffer[INDEX_INIT_DEVICE_ID2] = setup_module_id.module_id[2];
	temp_buffer[INDEX_INIT_DEVICE_ID3] = setup_module_id.module_id[3];

	BT_Main_event_send(NULL, BTM_EVT_CMD_INIT, temp_buffer);

	//check BTM init
	while(check_ack_init_state == 0)
		task_sleep(100);

	check_ack_init_state = 0;
	result	=0;

	return result;
}

int8_t BT_DATA_SEND(uint8_t role, uint8_t* data_msg )
{
	int8_t result;
	check_ack_data_send_state = 0;
	uint8_t* temp_buffer = malloc(BTM_INTERFACE_BUF_SIZE);

	//set packet : Init
	if(role==INIT_ROLE_PERIPHERAL)
		temp_buffer[INDEX_CMD] = CMD_PE_DATA_SEND;
	else
		temp_buffer[INDEX_CMD] = CMD_CE_DATA_SEND;
	//back up msg
	for(int i= 1;i<BTM_INTERFACE_BUF_SIZE;i++)
		temp_buffer[i]=data_msg[i-1];

	if(role==INIT_ROLE_PERIPHERAL)
		BT_Main_event_send(NULL, BTM_EVT_PE_CMD_DATA_SEND, temp_buffer);
	else
		BT_Main_event_send(NULL, BTM_EVT_CE_CMD_DATA_SEND, temp_buffer);

	//check BTM DATA_SEND
	while (check_ack_data_send_state == 0)
		task_sleep(100);

	check_ack_data_send_state = 0;
	result = 0;
	free(temp_buffer);

	return result;

}



//--------------------------------------------------------------------------BLE peripheral API
int8_t BT_ADV_START()
{
	int8_t result;
	check_ack_connected_state =0;
	uint8_t* temp_buffer = malloc(BTM_INTERFACE_BUF_SIZE);

	memset(temp_buffer, 0xFF, BTM_INTERFACE_BUF_SIZE);

	//set packet : Init
	temp_buffer[INDEX_CMD] = CMD_PE_ADV_START;

	BT_Main_event_send(NULL, BTM_EVT_PE_CMD_ADV_START, temp_buffer);

	//check BTM ADV
	while(	check_ack_connected_state == 0)//or check_ack_adv_state
	{
		task_sleep(100);
		led_toggle(LED1);
	}

	check_ack_connected_state = 0;
	result = 0;

	return result;
}
//--------------------------------------------------------------------------BLE central API

int8_t BT_SCAN_START()
{
		int8_t result =0;
		check_scan_done_state =0;
		uint8_t* temp_buffer = malloc(BTM_INTERFACE_BUF_SIZE);
		memset(temp_buffer, 0xFF, BTM_INTERFACE_BUF_SIZE);

		//set packet : Init
		temp_buffer[INDEX_CMD] = CMD_CE_SCAN_START;

		 BT_Main_event_send(NULL, BTM_EVT_CE_CMD_SCAN_START, temp_buffer);

		//check BTM SCAN_Done
		while(	check_scan_done_state == 0)
		{
			task_sleep(100);
			led_toggle(LED1);
		}

		if (check_scan_done_state ==2)
		{
			check_scan_done_state = 0;
			BT_SCAN_START();
		}

		return result;
}

int8_t BT_CONNECT(module_id_st target_module_id)
{
	int8_t result;
	check_ack_connected_state = 0;
	uint8_t* temp_buffer = malloc(BTM_INTERFACE_BUF_SIZE);

	memset(temp_buffer, 0xFF, BTM_INTERFACE_BUF_SIZE);

	//set packet : Connect

	temp_buffer[INDEX_CMD] = CMD_CE_CONNECT;
	temp_buffer[BT_MODULE_INDEX_CE_CON_DEVICE_ID0] = target_module_id.module_id[0];
	temp_buffer[BT_MODULE_INDEX_CE_CON_DEVICE_ID1] = target_module_id.module_id[1];
	temp_buffer[BT_MODULE_INDEX_CE_CON_DEVICE_ID2] = target_module_id.module_id[2];
	temp_buffer[BT_MODULE_INDEX_CE_CON_DEVICE_ID3] = target_module_id.module_id[3];

	BT_Main_event_send(NULL, BTM_EVT_CE_CMD_CONNECT, temp_buffer);
	//check BTM CONNECTED
	while(	check_ack_connected_state == 0)
		task_sleep(100);

	check_ack_connected_state = 0;
	result = 0;


	return result;
}

int8_t BT_DISCONNECT()
{
	int8_t result;
	check_ack_disconnected_state =0;
	uint8_t* temp_buffer = malloc(BTM_INTERFACE_BUF_SIZE);

	memset(temp_buffer, 0xFF, BTM_INTERFACE_BUF_SIZE);

	//set packet : Init
	temp_buffer[INDEX_CMD] = CMD_CE_DISCONNECT;

	BT_Main_event_send(NULL, BTM_EVT_CE_CMD_DISCONNECT, temp_buffer);

	//check BTM DISCONNECTED
	while(	check_ack_disconnected_state == 0)
		task_sleep(100);

	check_ack_disconnected_state = 0;
	result = 0;

	return result;
}

//--------------------------------------------------------------------------BLE CONTROL task
void BT_control_task(void * arg) {
	int r = 0;
	int i =0;
	uint8_t temp_rx_buffer[BTM_INTERFACE_BUF_SIZE]={0,};
	uint8_t temp_rx_msg_buffer[DATA_SEND_BUFFER_SIZE]={0,};
	uint8_t scan_num;

	BT_Evt_t BT_control_msgRxBuffer ={0,};

	for (;;) {
		r = msgq_receive(Main_Msgq, (unsigned char*) &BT_control_msgRxBuffer);
		if (0 != r) {
			logme("fail at msgq_receive\r\n");
		} else {
			switch (BT_control_msgRxBuffer.status) {
			case BTM_EVT_READ_INT :

				//read_packet
				BT_READ_CMD(temp_rx_buffer);

				//check Asynchronous msgMain_Msgq
				switch(temp_rx_buffer[INDEX_CMD])
				{
				case CMD_INIT_ACK :
					printf("COMPLETE INIT\r\n");
					check_ack_init_state = 1;
					break;
				case CMD_PE_DATA_REV:
					//send ACK
					BT_ACK_SET(CMD_PE_DATA_REV);
					printf("DATA_revd_PE\r\n");
					//remove CMD
					//memcmp(temp_rx_msg_buffer,temp_rx_buffer[1],DATA_SEND_BUFFER_SIZE);
					for(int i =0 ; i<20 ;i++)
						temp_rx_msg_buffer[i]= temp_rx_buffer[i+1];
					BT_USER_event_send(BT_control_msgRxBuffer.event, BT_EVT_PE_DATA_READ, temp_rx_msg_buffer);
					break;
				case CMD_CE_DATA_REV:
					//send ACK
					BT_ACK_SET(CMD_CE_DATA_REV);
					printf("DATA_revd_CE\r\n");
					//remove CMD
					//memcmp(temp_rx_msg_buffer,temp_rx_buffer[1],DATA_SEND_BUFFER_SIZE);
					for(int i =0 ; i<20 ;i++)
						temp_rx_msg_buffer[i]= temp_rx_buffer[i+1];
					BT_USER_event_send(BT_control_msgRxBuffer.event, BT_EVT_CE_DATA_READ, temp_rx_msg_buffer);
					break;
//-------------------------------------------------Peripheral ack
				case CMD_PE_ADV_START_ACK:
					printf("COMPLETE PE_ADV_START\r\n");
					break;
				case CMD_PE_ADV_STOP_ACK:
					printf("COMPLETE PE_ADV_STOP\r\n");
					break;

				case CMD_PE_DATA_REV_ACK:
					printf("COMPLETE PE_DATA_REV\r\n");
					break;
				case CMD_PE_DATA_SEND_ACK:
					printf("COMPLETE PE_DATA_SEND\r\n");
					check_ack_data_send_state=1;
					break;
//----------------------------------------------------Central ack
				case CMD_CE_SCAN_START_ACK:
					printf("COMPLETE CE_SCAN_STAR\r\n");
					break;
				case CMD_CE_SCAN_STOP_ACK:
					printf("COMPLETE CE_SCAN_STOP\r\n");
					break;

				case CMD_CE_CONNECT_ACK:
					printf("COMPLETE CE_CONNECT\r\n");

					break;
				case CMD_CE_DISCONNECT_ACK:
					printf("COMPLETE CE_DISCONNECT\r\n");
					break;

				case CMD_CE_DATA_REV_ACK:
					printf("COMPLETE CE_DATA_REV\r\n");
					break;

				case CMD_CE_DATA_SEND_ACK:
					printf("COMPLETE CE_DATA_SEND\r\n");
					check_ack_data_send_state=1;
					break;
//--------------------------------------------------------error
				case CMD_ERROR:
					printf("CMD error\r\n");
					break;
//------------------------------------------------------Asynchronous
				case CMD_CE_SCAN_RESULT:
					printf("COMPLETE CE_SCAN_RESULT\r\n");
					scan_num=temp_rx_buffer[1];
					BT_ACK_SET(temp_rx_buffer[INDEX_CMD]);

					break;
				case CMD_CE_SCAN_RESULT_DETAIL:
					printf("COMPLETE CE_SCAN_RESULT_DETAIL\r\n");
					if(i<MAX_SCAN_NUM || i<scan_num)
					{
						memcpy(temp_scan_result[i].module_id,&(temp_rx_buffer[BT_MODULE_INDEX_CE_CON_DEVICE_ID0]),DEVICE_ID_SIZE);
						i++;
						if(i==scan_num)
						{
							for(i =0; i<scan_num;i++)
							{
								if( target_id.module_id[0] == temp_scan_result[i].module_id[0] &&
									target_id.module_id[1] == temp_scan_result[i].module_id[1] &&
									target_id.module_id[2] == temp_scan_result[i].module_id[2] &&
									target_id.module_id[3] == temp_scan_result[i].module_id[3])
								{
									break;
								}

							}
							if( i < scan_num)
								check_scan_done_state=1;

							else
							{
								check_scan_done_state=2;
								i=0;
							}

						}
						BT_ACK_SET(temp_rx_buffer[INDEX_CMD]);
					}
					break;
				case CMD_PE_CONNECTED :
					//send ACK
					BT_ACK_SET(CMD_PE_CONNECTED);


					glcdGotoChar(1, 3);
					glcd_printf("CONNECTED with CENTRAL");


					//send msg to BT event receive queue
					BT_USER_event_send(BT_control_msgRxBuffer.event,BT_EVT_CONNECTED, temp_rx_buffer);

					check_ack_connected_state = 1;
					break;
				case CMD_CE_CONNECTED :
					//send ACK
					BT_ACK_SET(CMD_CE_CONNECTED);


					glcdGotoChar(1, 3);
					glcd_printf("CONNECETED with ");
					glcdGotoChar(1, 4);
					glcd_printf("%x %x %x %x",
							temp_rx_buffer[BT_MODULE_INDEX_CE_SCAN_DEVICE_ID0],
							temp_rx_buffer[BT_MODULE_INDEX_CE_SCAN_DEVICE_ID1],
							temp_rx_buffer[BT_MODULE_INDEX_CE_SCAN_DEVICE_ID2],
							temp_rx_buffer[BT_MODULE_INDEX_CE_SCAN_DEVICE_ID3]);


					//send msg to BT event receive queue
					BT_USER_event_send(BT_control_msgRxBuffer.event, BT_EVT_CONNECTED, temp_rx_buffer);

					check_ack_connected_state =1;
					break;
				case CMD_PE_DISCONNECTED :
					//send ACK
					BT_ACK_SET(CMD_PE_DISCONNECTED);


					printf("PE_DISCONNECTED\r\n");
					glcdGotoChar(1,5);
					glcd_printf("DISCONNECTED");


					BT_USER_event_send(BT_control_msgRxBuffer.event, BT_EVT_DISCONNECTED, temp_rx_buffer);
					check_ack_disconnected_state =1;
					break;
				case CMD_CE_DISCONNECTED :
					//send ACK
					BT_ACK_SET(CMD_CE_DISCONNECTED);


					printf("CE_DISCONNECTED\r\n");
					glcdGotoChar(1, 5);
					glcd_printf("DISCONNECTED");

					BT_USER_event_send(BT_control_msgRxBuffer.event, BT_EVT_DISCONNECTED, temp_rx_buffer);
					check_ack_disconnected_state =1;
					break;

				default :
					break;
				}

				break;
			case BTM_EVT_CMD_INIT:
				if (BT_control_msgRxBuffer.msg[INDEX_INIT_ROLE]	== INIT_ROLE_PERIPHERAL) {
					printf("INIT_PERIPHERAL\r\n");
				} else if (BT_control_msgRxBuffer.msg[INDEX_INIT_ROLE]	== INIT_ROLE_CENTRAL) {
					printf("INIT_CENTRAL\r\n");
				}
				memcpy(temp_BTM_tx_buffer, BT_control_msgRxBuffer.msg, BTM_INTERFACE_BUF_SIZE);
				memcpy(target_id.module_id,&BT_control_msgRxBuffer.msg[INDEX_INIT_DEVICE_ID0],DEVICE_ID_SIZE);
				if (gpio_pin_is_high(BT_PIN_INT)) {
					//BTM Interface INT pin is high

					uint8_t *temp_msg = NULL;
					//backup control msg
					memcpy(temp_msg, BT_control_msgRxBuffer.msg, BTM_INTERFACE_BUF_SIZE);

					BT_Main_event_send(BT_control_msgRxBuffer.event, BT_control_msgRxBuffer.status, temp_msg);
				} else {
					//BTM Interface INT pin is low
					// Send BTM Interface MSG
					BT_WRITE_CMD(temp_BTM_tx_buffer, temp_BTM_tx_buffer[INDEX_CMD]);
					current_ack_wait_cmd = temp_BTM_tx_buffer[INDEX_CMD];
				}

				break;
			case BTM_EVT_PE_CMD_ADV_START :
				printf("ADV_start\r\n");
				memcpy(temp_BTM_tx_buffer, BT_control_msgRxBuffer.msg,BTM_INTERFACE_BUF_SIZE);

				//Check BTM Interface Read Event
				if (gpio_pin_is_high(BT_PIN_INT)) {
					//BTM Interface INT pin is high

					uint8_t *temp_msg = NULL;
					//backup control msg
					memcpy(temp_msg, BT_control_msgRxBuffer.msg,BTM_INTERFACE_BUF_SIZE);

					BT_Main_event_send(BT_control_msgRxBuffer.event,BT_control_msgRxBuffer.status, temp_msg);
				} else {
					//BTM Interface INT pin is low
					// Send BTM Interface MSG
					BT_WRITE_CMD(temp_BTM_tx_buffer,temp_BTM_tx_buffer[INDEX_CMD]);
					current_ack_wait_cmd = temp_BTM_tx_buffer[INDEX_CMD];
				}
				break;
			case BTM_EVT_PE_CMD_ADV_STOP :
				break;
			case BTM_EVT_PE_CMD_DATA_SEND :

				printf("SEND_MSG_PE\r\n");
				memcpy(temp_BTM_tx_buffer, BT_control_msgRxBuffer.msg,BTM_INTERFACE_BUF_SIZE);

				//Check BTM Interface Read Event
				if (gpio_pin_is_high(BT_PIN_INT)) {
					//BTM Interface INT pin is high

					uint8_t *temp_msg = NULL;
					//backup control msg
					memcpy(temp_msg, BT_control_msgRxBuffer.msg,BTM_INTERFACE_BUF_SIZE);

					BT_Main_event_send(BT_control_msgRxBuffer.event,BT_control_msgRxBuffer.status, temp_msg);
				} else {
					//BTM Interface INT pin is low
					// Send BTM Interface MSG
					BT_WRITE_CMD(temp_BTM_tx_buffer,temp_BTM_tx_buffer[INDEX_CMD]);
					current_ack_wait_cmd = temp_BTM_tx_buffer[INDEX_CMD];
				}

				break;
			case BTM_EVT_CE_CMD_SCAN_START :
				printf("SCAN_START\r\n");
				memcpy(temp_BTM_tx_buffer, BT_control_msgRxBuffer.msg,BTM_INTERFACE_BUF_SIZE);

				//Check BTM Interface Read Event
				if (gpio_pin_is_high(BT_PIN_INT)) {
					//BTM Interface INT pin is high

					uint8_t *temp_msg = NULL;
					//backup control msg
					memcpy(temp_msg, BT_control_msgRxBuffer.msg,BTM_INTERFACE_BUF_SIZE);

					BT_Main_event_send(BT_control_msgRxBuffer.event,BT_control_msgRxBuffer.status, temp_msg);
				} else {
					//BTM Interface INT pin is low
					// Send BTM Interface MSG
					BT_WRITE_CMD(temp_BTM_tx_buffer,temp_BTM_tx_buffer[INDEX_CMD]);
					current_ack_wait_cmd = temp_BTM_tx_buffer[INDEX_CMD];
				}
				break;
			case BTM_EVT_CE_CMD_SCAN_STOP :
				break;
			case BTM_EVT_CE_CMD_CONNECT :

				printf("CONNECT\r\n");
				memcpy(temp_BTM_tx_buffer, BT_control_msgRxBuffer.msg,BTM_INTERFACE_BUF_SIZE);

				//Check BTM Interface Read Event
				if (gpio_pin_is_high(BT_PIN_INT))
				{
					//BTM Interface INT pin is high

					uint8_t *temp_msg = NULL;
					//backup control msg
					memcpy(temp_msg, BT_control_msgRxBuffer.msg,BTM_INTERFACE_BUF_SIZE);

					BT_Main_event_send(BT_control_msgRxBuffer.event,BT_control_msgRxBuffer.status, temp_msg);
				}
				else
				{
					for(i =0; i<=MAX_SCAN_NUM;i++)
					{
						if( temp_BTM_tx_buffer[BT_MODULE_INDEX_CE_CON_DEVICE_ID0] == temp_scan_result[i].module_id[0] &&
							temp_BTM_tx_buffer[BT_MODULE_INDEX_CE_CON_DEVICE_ID1] == temp_scan_result[i].module_id[1] &&
							temp_BTM_tx_buffer[BT_MODULE_INDEX_CE_CON_DEVICE_ID2] == temp_scan_result[i].module_id[2] &&
							temp_BTM_tx_buffer[BT_MODULE_INDEX_CE_CON_DEVICE_ID3] == temp_scan_result[i].module_id[3])
						{
							break;
						}

					}
					if( i <= MAX_SCAN_NUM)
					{
						BT_WRITE_CMD(temp_BTM_tx_buffer,temp_BTM_tx_buffer[INDEX_CMD]);
						current_ack_wait_cmd = temp_BTM_tx_buffer[INDEX_CMD];
					}
					else
					{
						printf("i can't find target_BT_ID");

					}

				}


			break;
			case BTM_EVT_CE_CMD_DISCONNECT:

				printf("DISCONNECT\r\n");
				memcpy(temp_BTM_tx_buffer, BT_control_msgRxBuffer.msg,BTM_INTERFACE_BUF_SIZE);

				//Check BTM Interface Read Event
				if (gpio_pin_is_high(BT_PIN_INT)) {
					//BTM Interface INT pin is high

					uint8_t *temp_msg = NULL;
					//backup control msg
					memcpy(temp_msg, BT_control_msgRxBuffer.msg,BTM_INTERFACE_BUF_SIZE);

					BT_Main_event_send(BT_control_msgRxBuffer.event,BT_control_msgRxBuffer.status, temp_msg);
				} else {
					//BTM Interface INT pin is low
					// Send BTM Interface MSG
					BT_WRITE_CMD(temp_BTM_tx_buffer,temp_BTM_tx_buffer[INDEX_CMD]);
					current_ack_wait_cmd = temp_BTM_tx_buffer[INDEX_CMD];
				}

				break;
			case BTM_EVT_CE_CMD_DATA_SEND :
				printf("SEND_MSG_CE\r\n");
				memcpy(temp_BTM_tx_buffer, BT_control_msgRxBuffer.msg,BTM_INTERFACE_BUF_SIZE);

				//Check BTM Interface Read Event
				if (gpio_pin_is_high(BT_PIN_INT)) {
					//BTM Interface INT pin is high

					uint8_t *temp_msg = NULL;
					//backup control msg
					memcpy(temp_msg, BT_control_msgRxBuffer.msg,BTM_INTERFACE_BUF_SIZE);

					BT_Main_event_send(BT_control_msgRxBuffer.event,BT_control_msgRxBuffer.status, temp_msg);
				} else {
					//BTM Interface INT pin is low
					// Send BTM Interface MSG
					BT_WRITE_CMD(temp_BTM_tx_buffer,temp_BTM_tx_buffer[INDEX_CMD]);
					current_ack_wait_cmd = temp_BTM_tx_buffer[INDEX_CMD];
				}

				break;

			default:
				break;
			}

			if(BT_control_msgRxBuffer.msg != NULL)
			{
				free(BT_control_msgRxBuffer.msg);
			}

		}
	}
}
