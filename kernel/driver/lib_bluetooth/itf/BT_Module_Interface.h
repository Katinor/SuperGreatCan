/*
 * PB_BT_Module.h
 *
 *  Created on: 2017. 2. 14.
 *      Author: Administrator
 */

#ifndef APPLICATION_PAARBAND_EXE_PAARBAND_SRC_PB_BT_MODULE_INTERFACE_H_
#define APPLICATION_PAARBAND_EXE_PAARBAND_SRC_PB_BT_MODULE_INTERFACE_H_

//BT Module Buffer size
#define BTM_INTERFACE_BUF_SIZE			20
#define DATA_SEND_BUFFER_SIZE			19
#define MAX_SCAN_NUM					10


//BT Module Command : IDLE State
#define CMD_INIT							0x01
#define CMD_INIT_ACK						CMD_INIT+ACK_SUM


//BT Module Command : Peripheral Role State
#define CMD_PE_ADV_START					0x11
#define CMD_PE_ADV_STOP					0x12
#define CMD_PE_CONNECTED					0x13
#define CMD_PE_DISCONNECTED				0x14
#define CMD_PE_DATA_REV					0x15
#define CMD_PE_DATA_SEND					0x16

//BT Module Command : Central Role State
#define CMD_CE_SCAN_START					0x21
#define CMD_CE_SCAN_STOP					0x22
#define CMD_CE_SCAN_RESULT				0x23
#define CMD_CE_SCAN_RESULT_DETAIL			0x24
#define CMD_CE_CONNECT					0x25
#define CMD_CE_DISCONNECT					0x26
#define CMD_CE_CONNECTED					0x27
#define CMD_CE_DISCONNECTED				0x28
#define CMD_CE_DATA_REV					0x29
#define CMD_CE_DATA_SEND					0x30

//BT Module Command Ack : Peripheral Role State
#define CMD_PE_ADV_START_ACK					CMD_PE_ADV_START+ACK_SUM
#define CMD_PE_ADV_STOP_ACK					CMD_PE_ADV_STOP+ACK_SUM
#define CMD_PE_CONNECTED_ACK					CMD_PE_CONNECTED+ACK_SUM
#define CMD_PE_DISCONNECTED_ACK				CMD_PE_DISCONNECTED+ACK_SUM
#define CMD_PE_DATA_REV_ACK					CMD_PE_DATA_REV+ACK_SUM
#define CMD_PE_DATA_SEND_ACK					CMD_PE_DATA_SEND+ACK_SUM


//BT Module Command Ack : Central Role State
#define CMD_CE_SCAN_START_ACK					CMD_CE_SCAN_START+ACK_SUM
#define CMD_CE_SCAN_STOP_ACK					CMD_CE_SCAN_STOP+ACK_SUM
#define CMD_CE_SCAN_RESULT_ACK				CMD_CE_SCAN_RESULT+ACK_SUM
#define CMD_CE_SCAN_RESULT_DETAIL_ACK			CMD_CE_SCAN_RESULT_DETAIL+ACK_SUM
#define CMD_CE_CONNECT_ACK					CMD_CE_CONNECT+ACK_SUM
#define CMD_CE_DISCONNECT_ACK					CMD_CE_DISCONNECT+ACK_SUM
#define CMD_CE_CONNECTED_ACK					CMD_CE_CONNECTED+ACK_SUM
#define CMD_CE_DISCONNECTED_ACK				CMD_CE_DISCONNECTED+ACK_SUM
#define CMD_CE_DATA_REV_ACK					CMD_CE_DATA_REV+ACK_SUM
#define CMD_CE_DATA_SEND_ACK					CMD_CE_DATA_SEND+ACK_SUM
//BT Module Command : ERROR
#define CMD_ERROR							0xF0

//BT Module Command Common Index : CMD
#define INDEX_CMD							0

//BT Module Command Parameter Index : IDLE State
#define INDEX_INIT_ROLE					1
#define INDEX_INIT_DEVICE_ID0				2
#define INDEX_INIT_DEVICE_ID1				3
#define INDEX_INIT_DEVICE_ID2				4
#define INDEX_INIT_DEVICE_ID3				5
#define DEVICE_ID_SIZE					4

//BT Module Command Parameter Index : Peripheral Role State
#define BT_MODULE_INDEX_PE_PACKET_DATA				1

//BT Module Command Parameter Index : Central Role State
#define BT_MODULE_INDEX_CE_SCAN_NUM_DEVICE			1
#define BT_MODULE_INDEX_CE_SCAN_DEVICE_ID0			1
#define BT_MODULE_INDEX_CE_SCAN_DEVICE_ID1			2
#define BT_MODULE_INDEX_CE_SCAN_DEVICE_ID2			3
#define BT_MODULE_INDEX_CE_SCAN_DEVICE_ID3			4
#define BT_MODULE_INDEX_CE_SCAN_RSSI				5
#define BT_MODULE_INDEX_CE_CON_DEVICE_ID0			1
#define BT_MODULE_INDEX_CE_CON_DEVICE_ID1			2
#define BT_MODULE_INDEX_CE_CON_DEVICE_ID2			3
#define BT_MODULE_INDEX_CE_CON_DEVICE_ID3			4
#define BT_MODULE_INDEX_CE_PACKET_DATA				1


//BT Module Command PArameter Index : Error
#define INDEX_ERROR_DETAIL				1

//BT Module ACK Sum
#define ACK_SUM							0x80

//BT Module Init Role
#define INIT_ROLE_PERIPHERAL				0x01
#define INIT_ROLE_CENTRAL					0x02

//BT Module ERROR Detail
#define ERROR_DETAIL_WRONG_CMD			0x01

struct{
	uint8_t module_id[4];
}typedef module_id_st;

#endif /* APPLICATION_PAARBAND_EXE_PAARBAND_SRC_PB_BT_MODULE_INTERFACE_H_ */
