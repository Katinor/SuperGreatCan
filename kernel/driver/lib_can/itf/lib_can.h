/*
 * lib_can.h
 *
 *  Created on: 2015. 10. 3.
 *      Author: TMHwang
 */

#ifndef APPLICATION_LIB_CAN_ITF_LIB_CAN_H_
#define APPLICATION_LIB_CAN_ITF_LIB_CAN_H_


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// chipset driver include
#include "ioport.h"
#include "pio/pio.h"

#include "lib_ubi_pio/itf/lib_ubi_pio.h"
#include "../../../../interface/itf_ubinos/itf/ubik_msgq.h"
#include "../../../porting/bsp_armcortexm_sam4eek/itf/sam4e16e.h"
#include "../../../porting/lib_sam4e/sam/boards/sam4e_ek/sam4e_ek.h"
#include "../../../porting/lib_sam4e/sam/components/can/sn65hvd234.h"
#include "../../../porting/lib_sam4e/sam/drivers/can/can.h"

/** @define Tx 메일 박스를 설정하지 않았을 시 CAN_PORT의 tx_box_num 값*/
#define NO_TX_BOX			-1

/** @define 메일 박스 사용 여부 결정값 */
#define USE					1
#define NO_USE				0

/** @define 최대 Can port의 수*/
#define MAX_CAN_MB_SIZE		8

/* control constant */
/** @define 전송시 8바이트 사이의 간격  */
#define CAN_INTER_MESSAGE_DELAY (3500)
/** @define 버스 폴링 간격  */
#define	CAN_POLL_DELAY 			(10)

/** @define 에러 상수 설정  */
#define ERROR_CAN_TX_TIMEOUT_BEFORESEND (-1)

enum CAN_NUM{
	CAN_DEV0,
	CAN_DEV1,
};

enum CAN_PROTOCOL
{
	CAN_PROTOCOL_2_0_A, // CAN 2.0A
	CAN_PROTOCOL_2_0_B 	// CAN 2.0B
};

/**
 * @struct	CAN_PORT
 * @brief	CAN포트 정보를 정의합니다.
 */
typedef struct _CAN_PORT
{
	msgq_pt 		msgq_rxbuf;		// ISR에서 수신된 메시지가 임시로 저장되는 수신 큐를 가리키는 핸들러
	can_mb_conf_t 	mbox[MAX_CAN_MB_SIZE];			// 포트에서 사용할 수신 메일박스들
	int8_t  		mbox_state[MAX_CAN_MB_SIZE];
	int8_t 			cbox_num;								// 포트에서 사용할 송신 메일박스
	int8_t 			protocol;      		  					// 현재 사용중인 CAN 프로토콜
	int8_t 			tx_box_num;      		  					// 현재 사용중인 CAN 프로토콜
}CAN_PORT, *PCAN_PORT;

/**
 * @struct	CAN_DATA
 * @brief	CAN 패킷 정보를 정의합니다.
 */
typedef struct _CAN_DATA{
	uint32_t 	data_high;	// 바이트 4, 5, 6, 7
	uint32_t 	data_low;	// 바이트 0, 1, 2, 3
}CAN_DATA, *PCAN_DATA;

void lib_can_component_init( void );

/*****************************************************************************
 * function
 *****************************************************************************/
void can_buffer_init(uint8_t devid);
void can_buffer_create(uint8_t devid, PCAN_PORT port_pt, uint32_t size);
void can_buffer_delete(uint8_t devid, PCAN_PORT port_pt);

/**
 * initialize the CAN device
 *
 * @param 	devid	초기화하고자 하는 CAN 장치
 *
 * @param 	baud	CAN baud rate
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t can_open ( uint8_t devid, uint16_t baud );

/**
 * CAN 장치를 닫습니다. 모든 포트를 사용할 수 없습니다.
 *
 * @param 	devid	닫고자 하는 CAN 장치
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t can_close ( uint8_t devid );

/**
 * CAN 통신을 위한 포트를 생성합니다.
 * 포트를 열기 전에 반드시  can_open 을 통해 장치가 초기화되어 있어야 합니다.
 *
 * @param 	devid	사용하고자 하는 CAN 장치
 *
 * @param 	port_pt	생성되는 포트를 위한 핸들러
 *
 * @param 	num_recvmbox	할당할 수신 메일박스 개수
 *
 * @param 	use_sendmbox	송신 메일박스를 사용할지의 여부
 *
 * @param 	size_recv	수신 버퍼 크기
 *
 * @return	result	1: success
 * 					0: fail
 */

uint8_t can_port_set(uint8_t devid, PCAN_PORT port_pt);

uint8_t	can_port_open (uint8_t devid, PCAN_PORT port_pt, uint8_t num_recvmbox, int8_t use_sendmbox, uint32_t size_recv);

/**
 * 더이상 사용하지 않는 CAN 포트를 닫습니다. 할당된 메일박스들을 해제합니다.
 *
 * @param 	devid	사용하고자 하는 CAN 장치
 *
 * @param 	port_pt	닫고자 하는 포트의 핸들러
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t	can_port_close (uint8_t devid, PCAN_PORT port_pt);

/**
 * 송신 메시지의 ID를 설정합니다.
 *
 * @param 	devid	사용하고자 하는 CAN 장치
 *
 * @param 	port_pt	설정할 포트의 핸들러
 *
 * @param 	id	ID값
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t	can_port_set_sendid (uint8_t devid, PCAN_PORT port_pt, uint32_t id);

/**
 * 송신 메시지의 우선순위를 설정합니다.
 * 여기서 우선순위라 함은 메시지간의 우선순위가 아니라 한 장치 내에 다수의 송신 포트가 있을 경우 이들간의 우선순위를 말합니다.
 *
 * @param 	devid	사용하고자 하는 CAN 장치
 *
 * @param 	port_pt	설정할 포트의 핸들러
 *
 * @param 	priority	우선순위 값
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t	can_port_set_sendpriority (uint8_t devid, PCAN_PORT port_pt, uint32_t priority);

/**
 * 수신 메일박스의 ID를 설정합니다. 이 값과  can_port_set_recvidmask 에서 설정한 마스크 값을 가지고 받아들일 메시지를 결정합니다.
 *
 * @param 	devid	사용하고자 하는 CAN 장치
 *
 * @param 	port_pt	설정할 포트의 핸들러
 *
 * @param 	id	수신 메일박스 ID
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t	can_port_set_recvid (uint8_t devid, PCAN_PORT port_pt, uint32_t id);

/**
 * 수신 메일박스의 마스크를 설정합니다. 이 값과  can_port_set_recvid 에서  설정한 ID 값을 가지고 받아들일 메시지를 결정합니다.
 *
 * @param 	devid	사용하고자 하는 CAN 장치
 *
 * @param 	port_pt	설정할 포트의 핸들러
 *
 * @param 	mask	수신 메일박스 마스크
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t	can_port_set_recvidmask (uint8_t devid, PCAN_PORT port_pt, uint32_t mask);

/**
 * 통신에 사용할 CAN프로토콜을 결정합니다. 2.0A와 2.0B중 하나를 사용합니다.
 *
 * @param 	devid	사용하고자 하는 CAN 장치
 *
 * @param 	port_pt	설정할 포트의 핸들러
 *
 * @param 	protocol	사용할 프로토콜
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t	can_port_set_protocol (uint8_t devid, PCAN_PORT port_pt, int8_t protocol);

/**
 * Size 길이의 메시지를 pPort포트를 통해 송신합니다.
 *
 * @param 	devid	사용하고자 하는 CAN 장치
 *
 * @param 	port_pt	사용하고자하는 CAN 포트
 *
 * @param 	buf_send_pt	송신하고자 하는 메시지
 *
 * @param 	size	메시지의 길이 (byte)
 *
 * @param 	timeout	타임아웃 시간(단위 ms)
 *
 * @return	result	1: success
 * 					0: fail
 */
int16_t can_port_send (uint8_t devid, PCAN_PORT port_pt, uint8_t * buf_send_pt, int8_t size, int16_t timeout);

/**
 * pPort 내의 수신 버퍼에 저장된 메시지 중 Size 길이의 메시지를 꺼내옵니다.
 *
 * @param 	devid	사용하고자 하는 CAN 장치
 *
 * @param 	port_pt	사용하고자하는 CAN 포트
 *
 * @param 	buf_recv_pt	꺼내온 메시지가 저장될 버퍼
 *
 * @param 	size	꺼내올 메시지의 길이 (byte)
 *
 * @param 	timeout	타임아웃 시간(단위 ms)
 *
 * @return	result	1: success
 * 					0: fail
 */
int16_t can_port_recv (uint8_t devid, PCAN_PORT port_pt, uint8_t * buf_recv_pt, uint8_t size, int16_t timeout);

#endif /* APPLICATION_LIB_CAN_ITF_LIB_CAN_H_ */
