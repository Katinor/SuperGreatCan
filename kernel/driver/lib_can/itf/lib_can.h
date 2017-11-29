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

/** @define Tx ���� �ڽ��� �������� �ʾ��� �� CAN_PORT�� tx_box_num ��*/
#define NO_TX_BOX			-1

/** @define ���� �ڽ� ��� ���� ������ */
#define USE					1
#define NO_USE				0

/** @define �ִ� Can port�� ��*/
#define MAX_CAN_MB_SIZE		8

/* control constant */
/** @define ���۽� 8����Ʈ ������ ����  */
#define CAN_INTER_MESSAGE_DELAY (3500)
/** @define ���� ���� ����  */
#define	CAN_POLL_DELAY 			(10)

/** @define ���� ��� ����  */
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
 * @brief	CAN��Ʈ ������ �����մϴ�.
 */
typedef struct _CAN_PORT
{
	msgq_pt 		msgq_rxbuf;		// ISR���� ���ŵ� �޽����� �ӽ÷� ����Ǵ� ���� ť�� ����Ű�� �ڵ鷯
	can_mb_conf_t 	mbox[MAX_CAN_MB_SIZE];			// ��Ʈ���� ����� ���� ���Ϲڽ���
	int8_t  		mbox_state[MAX_CAN_MB_SIZE];
	int8_t 			cbox_num;								// ��Ʈ���� ����� �۽� ���Ϲڽ�
	int8_t 			protocol;      		  					// ���� ������� CAN ��������
	int8_t 			tx_box_num;      		  					// ���� ������� CAN ��������
}CAN_PORT, *PCAN_PORT;

/**
 * @struct	CAN_DATA
 * @brief	CAN ��Ŷ ������ �����մϴ�.
 */
typedef struct _CAN_DATA{
	uint32_t 	data_high;	// ����Ʈ 4, 5, 6, 7
	uint32_t 	data_low;	// ����Ʈ 0, 1, 2, 3
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
 * @param 	devid	�ʱ�ȭ�ϰ��� �ϴ� CAN ��ġ
 *
 * @param 	baud	CAN baud rate
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t can_open ( uint8_t devid, uint16_t baud );

/**
 * CAN ��ġ�� �ݽ��ϴ�. ��� ��Ʈ�� ����� �� �����ϴ�.
 *
 * @param 	devid	�ݰ��� �ϴ� CAN ��ġ
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t can_close ( uint8_t devid );

/**
 * CAN ����� ���� ��Ʈ�� �����մϴ�.
 * ��Ʈ�� ���� ���� �ݵ��  can_open �� ���� ��ġ�� �ʱ�ȭ�Ǿ� �־�� �մϴ�.
 *
 * @param 	devid	����ϰ��� �ϴ� CAN ��ġ
 *
 * @param 	port_pt	�����Ǵ� ��Ʈ�� ���� �ڵ鷯
 *
 * @param 	num_recvmbox	�Ҵ��� ���� ���Ϲڽ� ����
 *
 * @param 	use_sendmbox	�۽� ���Ϲڽ��� ��������� ����
 *
 * @param 	size_recv	���� ���� ũ��
 *
 * @return	result	1: success
 * 					0: fail
 */

uint8_t can_port_set(uint8_t devid, PCAN_PORT port_pt);

uint8_t	can_port_open (uint8_t devid, PCAN_PORT port_pt, uint8_t num_recvmbox, int8_t use_sendmbox, uint32_t size_recv);

/**
 * ���̻� ������� �ʴ� CAN ��Ʈ�� �ݽ��ϴ�. �Ҵ�� ���Ϲڽ����� �����մϴ�.
 *
 * @param 	devid	����ϰ��� �ϴ� CAN ��ġ
 *
 * @param 	port_pt	�ݰ��� �ϴ� ��Ʈ�� �ڵ鷯
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t	can_port_close (uint8_t devid, PCAN_PORT port_pt);

/**
 * �۽� �޽����� ID�� �����մϴ�.
 *
 * @param 	devid	����ϰ��� �ϴ� CAN ��ġ
 *
 * @param 	port_pt	������ ��Ʈ�� �ڵ鷯
 *
 * @param 	id	ID��
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t	can_port_set_sendid (uint8_t devid, PCAN_PORT port_pt, uint32_t id);

/**
 * �۽� �޽����� �켱������ �����մϴ�.
 * ���⼭ �켱������ ���� �޽������� �켱������ �ƴ϶� �� ��ġ ���� �ټ��� �۽� ��Ʈ�� ���� ��� �̵鰣�� �켱������ ���մϴ�.
 *
 * @param 	devid	����ϰ��� �ϴ� CAN ��ġ
 *
 * @param 	port_pt	������ ��Ʈ�� �ڵ鷯
 *
 * @param 	priority	�켱���� ��
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t	can_port_set_sendpriority (uint8_t devid, PCAN_PORT port_pt, uint32_t priority);

/**
 * ���� ���Ϲڽ��� ID�� �����մϴ�. �� ����  can_port_set_recvidmask ���� ������ ����ũ ���� ������ �޾Ƶ��� �޽����� �����մϴ�.
 *
 * @param 	devid	����ϰ��� �ϴ� CAN ��ġ
 *
 * @param 	port_pt	������ ��Ʈ�� �ڵ鷯
 *
 * @param 	id	���� ���Ϲڽ� ID
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t	can_port_set_recvid (uint8_t devid, PCAN_PORT port_pt, uint32_t id);

/**
 * ���� ���Ϲڽ��� ����ũ�� �����մϴ�. �� ����  can_port_set_recvid ����  ������ ID ���� ������ �޾Ƶ��� �޽����� �����մϴ�.
 *
 * @param 	devid	����ϰ��� �ϴ� CAN ��ġ
 *
 * @param 	port_pt	������ ��Ʈ�� �ڵ鷯
 *
 * @param 	mask	���� ���Ϲڽ� ����ũ
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t	can_port_set_recvidmask (uint8_t devid, PCAN_PORT port_pt, uint32_t mask);

/**
 * ��ſ� ����� CAN���������� �����մϴ�. 2.0A�� 2.0B�� �ϳ��� ����մϴ�.
 *
 * @param 	devid	����ϰ��� �ϴ� CAN ��ġ
 *
 * @param 	port_pt	������ ��Ʈ�� �ڵ鷯
 *
 * @param 	protocol	����� ��������
 *
 * @return	result	1: success
 * 					0: fail
 */
uint8_t	can_port_set_protocol (uint8_t devid, PCAN_PORT port_pt, int8_t protocol);

/**
 * Size ������ �޽����� pPort��Ʈ�� ���� �۽��մϴ�.
 *
 * @param 	devid	����ϰ��� �ϴ� CAN ��ġ
 *
 * @param 	port_pt	����ϰ����ϴ� CAN ��Ʈ
 *
 * @param 	buf_send_pt	�۽��ϰ��� �ϴ� �޽���
 *
 * @param 	size	�޽����� ���� (byte)
 *
 * @param 	timeout	Ÿ�Ӿƿ� �ð�(���� ms)
 *
 * @return	result	1: success
 * 					0: fail
 */
int16_t can_port_send (uint8_t devid, PCAN_PORT port_pt, uint8_t * buf_send_pt, int8_t size, int16_t timeout);

/**
 * pPort ���� ���� ���ۿ� ����� �޽��� �� Size ������ �޽����� �����ɴϴ�.
 *
 * @param 	devid	����ϰ��� �ϴ� CAN ��ġ
 *
 * @param 	port_pt	����ϰ����ϴ� CAN ��Ʈ
 *
 * @param 	buf_recv_pt	������ �޽����� ����� ����
 *
 * @param 	size	������ �޽����� ���� (byte)
 *
 * @param 	timeout	Ÿ�Ӿƿ� �ð�(���� ms)
 *
 * @return	result	1: success
 * 					0: fail
 */
int16_t can_port_recv (uint8_t devid, PCAN_PORT port_pt, uint8_t * buf_recv_pt, uint8_t size, int16_t timeout);

#endif /* APPLICATION_LIB_CAN_ITF_LIB_CAN_H_ */
