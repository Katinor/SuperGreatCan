/*
  Copyright (C) 2009 RTLab.
  Homepage: http://rtlab.knu.ac.kr/

  This file is part of the lib_espsapiv2 component of the Ubinos.

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

/*
 * ESPS API V2 for Ubinos - CAN
 *
 * @file can.c
 */

#include "../itf/lib_can.h"

msgq_pt _g_can0_rx_buff[MAX_CAN_MB_SIZE];
msgq_pt _g_can1_rx_buff[MAX_CAN_MB_SIZE];

void can_buffer_init(uint8_t devid)
{
	uint8_t i;
	switch(devid){
	case CAN_DEV0 :
		for(i=0; i< MAX_CAN_MB_SIZE; i++)
		{
			_g_can0_rx_buff[i] = NULL;
		}
		break;
	case CAN_DEV1 :
		for(i=0; i <MAX_CAN_MB_SIZE; i++)
		{
			_g_can1_rx_buff[i] = NULL;
		}
		break;
	default :
		printf("\r\n\ncan_buffer_init failed - Wrong Can Number\r\n\n");
		break;
	}
}

void can_buffer_create(uint8_t devid, PCAN_PORT port_pt, uint32_t size)
{
	uint8_t i = 0;

	if(port_pt->mbox_state[0] == NO_USE)
		return; /* Rx mailbox not defined. error */
	else if(port_pt->mbox[0].uc_obj_type == CAN_MB_TX_MODE)
		return; /* Rx mailbox not defined. error */

	switch(devid){
	case CAN_DEV0:
		msgq_create(&_g_can0_rx_buff[port_pt->mbox[0].ul_mb_idx], sizeof(CAN_DATA), size);

		while(port_pt->mbox_state[i] != NO_USE){
			if(port_pt->mbox[i].uc_obj_type == CAN_MB_RX_MODE)
				_g_can0_rx_buff[port_pt->mbox[i].ul_mb_idx] = _g_can0_rx_buff[port_pt->mbox[0].ul_mb_idx];
			i++;
		}
		port_pt->msgq_rxbuf = _g_can0_rx_buff[port_pt->mbox[0].ul_mb_idx];
		break;
	case CAN_DEV1:
		msgq_create(&_g_can1_rx_buff[port_pt->mbox[0].ul_mb_idx], sizeof(CAN_DATA), size);

		while(port_pt->mbox_state[i] != NO_USE){
			if(port_pt->mbox[i].uc_obj_type == CAN_MB_RX_MODE)
				_g_can1_rx_buff[port_pt->mbox[i].ul_mb_idx] = _g_can1_rx_buff[port_pt->mbox[0].ul_mb_idx];
			i++;
		}
		port_pt->msgq_rxbuf = _g_can1_rx_buff[port_pt->mbox[0].ul_mb_idx];
		break;
	}

}

void can_buffer_delete(uint8_t devid, PCAN_PORT port_pt)
{
	uint8_t i = 0;

	if(port_pt->mbox_state[0] == NO_USE)
		return; /* Rx mailbox not defined. error */

	switch(devid){
	case CAN_DEV0:
		msgq_delete(&_g_can0_rx_buff[port_pt->mbox[0].ul_mb_idx]);
		_g_can0_rx_buff[port_pt->mbox[0].ul_mb_idx] = NULL;

		while(port_pt->mbox_state[i] != NO_USE){
			if(port_pt->mbox[i].uc_obj_type == CAN_MB_RX_MODE)
				_g_can0_rx_buff[port_pt->mbox[i].ul_mb_idx] = NULL;
			i++;
		}
		break;
	case CAN_DEV1:
		msgq_delete(&_g_can1_rx_buff[port_pt->mbox[0].ul_mb_idx]);
		_g_can1_rx_buff[port_pt->mbox[0].ul_mb_idx] = NULL;

		while(port_pt->mbox_state[i] != NO_USE){
			if(port_pt->mbox[i].uc_obj_type == CAN_MB_RX_MODE)
				_g_can1_rx_buff[port_pt->mbox[i].ul_mb_idx] = NULL;
			i++;
		}
		break;
	default :
		break;
	}
	port_pt->msgq_rxbuf = NULL;
}

void CAN0_Handler()
{
	uint8_t i = 0;
	uint32_t ul_status;
	CAN_DATA recvData;
	can_mb_conf_t temp_mailbox;

	for( i = 0; i< MAX_CAN_MB_SIZE; i++){
		if(((CAN0->CAN_MB[i].CAN_MMR & CAN_MMR_MOT_Msk) >> CAN_MMR_MOT_Pos) == CAN_MB_TX_MODE)
			continue; /* this mailbox is Tx mailbox. do nothing */

		ul_status = can_mailbox_get_status(CAN0, i);
		/* Is there something to read? */
		if((ul_status & CAN_MSR_MRDY) == CAN_MSR_MRDY){
			temp_mailbox.ul_mb_idx = i;
			temp_mailbox.ul_status = ul_status;
			can_mailbox_read(CAN0, &temp_mailbox);

			recvData.data_high = temp_mailbox.ul_datal;                      /* byte 4,5,6,7 */
			recvData.data_low = temp_mailbox.ul_datah;  	                    /* byte 0,1,2,3 */

			msgq_send(_g_can0_rx_buff[i], (unsigned char * )(&recvData)); /* without timeout */
			CAN0->CAN_MB[i].CAN_MCR = CAN_MCR_MTCR;
		}
	}

	/* Tx operation */

	/* Rx operation */
}

void CAN1_Handler()
{
	uint8_t i = 0;
	uint32_t ul_status;
	CAN_DATA recvData;
	can_mb_conf_t temp_mailbox;

	for( i = 0; i< MAX_CAN_MB_SIZE; i++){
		if(((CAN1->CAN_MB[i].CAN_MMR & CAN_MMR_MOT_Msk) >> CAN_MMR_MOT_Pos) == CAN_MB_TX_MODE)
			continue; /* this mailbox is Tx mailbox. do nothing */

		ul_status = can_mailbox_get_status(CAN1, i);
		/* Is there something to read? */
		if((ul_status & CAN_MSR_MRDY) == CAN_MSR_MRDY){
			temp_mailbox.ul_mb_idx = i;
			temp_mailbox.ul_status = ul_status;
			can_mailbox_read(CAN1, &temp_mailbox);

			recvData.data_high = temp_mailbox.ul_datal;                      /* byte 4,5,6,7 */
			recvData.data_low = temp_mailbox.ul_datah;  	                    /* byte 0,1,2,3 */

			msgq_send(_g_can1_rx_buff[i], (unsigned char * )(&recvData)); /* without timeout */
			CAN1->CAN_MB[i].CAN_MCR = CAN_MCR_MTCR;
		}
	}

	/* Tx operation */

	/* Rx operation */
}
