/*
 * lib_can.c
 *
 *  Created on: 2015. 10. 3.
 *      Author: taemin
 */

#include <stdio.h>
#include <sam4e.h>
#include <stdint.h>
#include <stdbool.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"

#include "../itf/lib_can.h"

void lib_can_component_init( void )
{
	printf("library can test\n\r");
}

uint8_t can_open ( uint8_t devid, uint16_t baud ){
	uint8_t result = true;
	switch(devid){
		case CAN_DEV0 :
			/* PIO configuration for CAN */
			pio_set_peripheral(PIOB, PIO_PERIPH_A, PIO_PB2);
			pio_set_peripheral(PIOB, PIO_PERIPH_A, PIO_PB3);

			can_disable_interrupt( CAN0 , 0x0 );

			/* Enable CAN Controller Clock (PMC) */
			pmc_enable_periph_clk(ID_CAN0);

			/* Initialize the baudrate for CAN module and enable the CAN controller*/
			if(1 != can_init(CAN0, sysclk_get_cpu_hz(), baud)){
				printf("\r\n\nCAN0 init fail\r\n\n");
				result = false;
			}
			break;
		case CAN_DEV1 :
			/* PIO configuration for CAN */
			pio_set_peripheral(PIOC, PIO_PERIPH_C, PIO_PC12);
			pio_set_peripheral(PIOC, PIO_PERIPH_C, PIO_PC15);

			can_disable_interrupt( CAN1 , 0x0 );

			/* Enable CAN Controller Clock (PMC) */
			pmc_enable_periph_clk(ID_CAN1);

			/* Initialize the baudrate for CAN module and enable the CAN controller*/
			if(1 != can_init(CAN1, sysclk_get_cpu_hz(), baud)){
				printf("\r\n\nCAN1 init fail\r\n\n");
				result = false;
			}
			break;
		default :
			printf("\r\n\nWrong can device id\r\n\n");
			result = false;
			break;
	}

	return result;
}

uint8_t can_close ( uint8_t devid ){
	uint8_t result = true;
	switch(devid){
		case CAN_DEV0:
			/* Disable CAN Controller Clock (PMC) */
			pmc_disable_periph_clk(ID_CAN0);
			break;
		case CAN_DEV1:
			/* Disable CAN Controller Clock (PMC) */
			pmc_disable_periph_clk(ID_CAN1);
			break;
		default :
			printf("\r\n\nWrong can device id\r\n\n");
			result = false;
			break;
	}

	return result;
}

uint8_t can_port_set(uint8_t devid, PCAN_PORT port_pt){
	uint8_t result = true;
	uint8_t i;
	switch(devid){
		case CAN_DEV0 :
			/* assign mailbox */
			for(i = 0; i < port_pt->cbox_num; i++){
				if(port_pt->mbox_state[i] == USE){
					can_mailbox_init(CAN0, &port_pt->mbox[i]);
					if(i != port_pt->tx_box_num){
						/* enable mailbox interrupt */
						intr_connectisr(CAN0_IRQn, CAN0_Handler, intr_gethighestpriority(), 0);
						can_enable_interrupt(CAN0, 1 << i);
						NVIC_EnableIRQ(CAN0_IRQn);
					}
				}
			}
			NVIC_EnableIRQ(CAN0_IRQn);
			break;
		case CAN_DEV1 :
			/* assign mailbox */
			for(i = 0; i < port_pt->cbox_num; i++){
				if(port_pt->mbox_state[i] == USE){
					can_mailbox_init(CAN1, &port_pt->mbox[i]);
					if(i != port_pt->tx_box_num){
						/* enable mailbox interrupt */
						intr_connectisr(CAN1_IRQn, CAN1_Handler, intr_gethighestpriority(), 0);
						can_enable_interrupt(CAN1, 1 << i);
						NVIC_EnableIRQ(CAN1_IRQn);
					}
				}
			}
			NVIC_EnableIRQ(CAN1_IRQn);
			break;
		default :
			printf("\r\n\nWrong can device id\r\n\n");
			result = false;
			break;
	}

	return result;
}

uint8_t	can_port_open (uint8_t devid, PCAN_PORT port_pt, uint8_t num_recvmbox, int8_t use_sendmbox, uint32_t size_recv){
	uint8_t result = true;
	uint8_t i;

	for( i = 0; i < MAX_CAN_MB_SIZE; i++ )
		port_pt->mbox_state[i] = NO_USE;
	port_pt->protocol = CAN_PROTOCOL_2_0_A;

	/* Check if we can create new port */
	if(port_pt->cbox_num == MAX_CAN_MB_SIZE){
		printf("\r\n\nNo enough free mailbox\r\n\n");
		return false; /* not enough free mailbox. error */
	}

	/* assign a new portId */
	for(i = 0; i < num_recvmbox; i++){
		port_pt->mbox_state[i] = USE;
		port_pt->mbox[i].uc_obj_type = CAN_MB_RX_MODE;
		port_pt->cbox_num += 1;
	}

	if( num_recvmbox == 1 )
		port_pt->mbox[0].uc_obj_type = CAN_MB_RX_OVER_WR_MODE;

	if( use_sendmbox == true ){
		port_pt->mbox_state[i] = USE;
		port_pt->mbox[i].uc_obj_type = CAN_MB_TX_MODE;
		port_pt->cbox_num += 1;
		port_pt->tx_box_num = i;
	}

	switch(devid){
		case CAN_DEV0 :
			can_reset_all_mailbox(CAN0);
			/* assign mailbox */
			for(i = 0; i < port_pt->cbox_num; i++){
				if(port_pt->mbox_state[i] == USE){
					port_pt->mbox[i].ul_mb_idx = i;
				}
			}
			break;
		case CAN_DEV1 :
			can_reset_all_mailbox(CAN1);
			/* assign mailbox */
			for(i = 0; i < port_pt->cbox_num; i++){
				if(port_pt->mbox_state[i] == USE){
					port_pt->mbox[i].ul_mb_idx = i;
				}
			}
			break;
		default :
			printf("\r\n\nWrong can device id\r\n\n");
			result = false;
			break;
	}
	/* create receive buffer(queue) */
	can_buffer_create(devid, port_pt, size_recv);

	return result;
}

uint8_t	can_port_close (uint8_t devid, PCAN_PORT port_pt){
	uint8_t result = true;
	uint8_t i;

	/* the given port is correct? */
	if(port_pt->cbox_num == 0)
		return false; /* incorrect port name. error */

	/* free the receive buffer(queue) */
	can_buffer_delete(devid, port_pt);

	switch(devid)
	{
		/* Device ID : 1 - Atmel CAN Controller */
		case CAN_DEV0 :
			{
				/* Free assigned mailbox */
				for(i = 0; i < port_pt->cbox_num; i++){
					/* disable mailbox interrupt */
					can_disable_interrupt(CAN0, 1 << i);
					port_pt->mbox_state[i] = NO_USE;
					port_pt->mbox[i].ul_mb_idx = i;
					port_pt->mbox[i].uc_obj_type = CAN_MB_DISABLE_MODE;
					can_mailbox_init(CAN0, &port_pt->mbox[i]);
				}

				port_pt->cbox_num = 0;

				if(port_pt->tx_box_num != NO_TX_BOX)
					port_pt->tx_box_num = NO_TX_BOX;
			}
			break;
		case CAN_DEV1 :
		{
			/* Free assigned mailbox */
			for(i = 0; i < port_pt->cbox_num; i++){
				/* disable mailbox interrupt */
				can_disable_interrupt(CAN1, 1 << i);
				port_pt->mbox_state[i] = NO_USE;
				port_pt->mbox[i].ul_mb_idx = i;
				port_pt->mbox[i].uc_obj_type = CAN_MB_DISABLE_MODE;
				can_mailbox_init(CAN1, &port_pt->mbox[i]);
			}

			port_pt->cbox_num = 0;

			if(port_pt->tx_box_num != NO_TX_BOX)
				port_pt->tx_box_num = NO_TX_BOX;
		}
			break;
		default:
			printf("\r\n\nWrong can device id\r\n\n");
			result = false;
			break;
	}

	return result;

}

uint8_t	can_port_set_sendid (uint8_t devid, PCAN_PORT port_pt, uint32_t id){
	uint8_t result = true;
	uint8_t offset = 0;
	int8_t protocol = port_pt->protocol;

	/* the given port is correct? */
	if(port_pt->cbox_num == 0)
		return false; /* incorrect port name. error */

	if(port_pt->tx_box_num == NO_TX_BOX)
		return false; /* this port has no Tx mailbox */

	switch(devid)
	{
		/* Device ID : 1 - Atmel CAN Controller */
		case CAN_DEV0 :
		case CAN_DEV1 :
		{
			/* Assign new Send Id to Tx. mailbox  */

			if(protocol == CAN_PROTOCOL_2_0_A)
				offset = 18;
			port_pt->mbox[port_pt->tx_box_num].ul_id = id << offset;
			if(protocol == CAN_PROTOCOL_2_0_B)
				port_pt->mbox[port_pt->tx_box_num].ul_id_msk = CAN_MAM_MIDE;
		}
		break;
		default:
			printf("\r\n\nWrong can device id\r\n\n");
			result = false;
			break;
	}

	return result;
}

uint8_t	can_port_set_sendpriority (uint8_t devid, PCAN_PORT port_pt, uint32_t priority){
	uint8_t result = true;

	/* the given port is correct? */
	if(port_pt->cbox_num == 0)
		return false; /* incorrect port name. error */

	if(port_pt->tx_box_num == NO_TX_BOX)
		return false; /* this port has no Tx mailbox */

	switch(devid)
	{
		/* Device ID : 1 - Atmel CAN Controller */
		case CAN_DEV0 :
		case CAN_DEV1 :
			{
				port_pt->mbox[port_pt->tx_box_num].uc_tx_prio = priority;
			}
			break;
		default:
			result = false;
			break;
	}

	return result;
}

uint8_t	can_port_set_recvid (uint8_t devid, PCAN_PORT port_pt, uint32_t id){
	uint8_t 	result = true;
	uint32_t 	temp;
	uint8_t 	offset = 0;
	uint8_t		i;
	int8_t 		protocol = port_pt->protocol;

	/* the given port is correct? */
	if(port_pt->cbox_num == 0)
		return false; /* incorrect port name. error */

	switch(devid)
	{
	/* Device ID : 0 - Atmel CAN Controller */
		case CAN_DEV0 :
		{
				/* Assign Receive ID to every Rx.Mailbox */
			for( i = 0; i < port_pt->cbox_num; i++ ){
				if( i != port_pt->tx_box_num ){
					temp = CAN0->CAN_MB[i].CAN_MMR;

					if(protocol == CAN_PROTOCOL_2_0_A)
						offset = 18;
					port_pt->mbox[i].ul_id = id << offset;

					if(protocol == CAN_PROTOCOL_2_0_B)
						port_pt->mbox[i].ul_id_msk = CAN_MAM_MIDE;
					CAN0->CAN_MB[i].CAN_MMR = temp;
				}
			}
		}
		break;
		case CAN_DEV1 :
		{
				/* Assign Receive ID to every Rx.Mailbox */
			for( i = 0; i < port_pt->cbox_num; i++ ){
				if( i != port_pt->tx_box_num ){
					temp = CAN1->CAN_MB[i].CAN_MMR;

					if(protocol == CAN_PROTOCOL_2_0_A)
						offset = 18;
					port_pt->mbox[i].ul_id = id << offset;

					if(protocol == CAN_PROTOCOL_2_0_B)
						port_pt->mbox[i].ul_id_msk = CAN_MAM_MIDE;
					CAN1->CAN_MB[i].CAN_MMR = temp;
				}
			}
		}
		break;
	default:
		printf("\r\n\nWrong can device id\r\n\n");
		result = false;
		break;
	}

	return result;
}

uint8_t	can_port_set_recvidmask (uint8_t devid, PCAN_PORT port_pt, uint32_t mask){
	uint8_t 	result = true;
	uint32_t 	temp;
	uint8_t 	offset = 0;
	int8_t		protocol = port_pt->protocol;
	uint8_t		i;

	/* the given port is correct? */
	if(port_pt->cbox_num == 0)
		return false; /* incorrect port name. error */

	switch(devid)
	{
	/* Device ID : 0 - Atmel CAN Controller */
		case CAN_DEV0 :
		{
				/* Assign Receive ID to every Rx.Mailbox */
			for( i = 0; i < port_pt->cbox_num; i++ ){
				if( i != port_pt->tx_box_num ){
					temp = CAN0->CAN_MB[i].CAN_MMR;

					if(protocol == CAN_PROTOCOL_2_0_A)
						offset = 18;
					port_pt->mbox[i].ul_id_msk = mask << offset;

					if(protocol == CAN_PROTOCOL_2_0_B)
						port_pt->mbox[i].ul_id_msk = CAN_MAM_MIDE;
					CAN0->CAN_MB[i].CAN_MMR = temp;
				}
			}
		}
		break;
		case CAN_DEV1 :
		{
				/* Assign Receive ID to every Rx.Mailbox */
			for( i = 0; i < port_pt->cbox_num; i++ ){
				if( i != port_pt->tx_box_num ){
					temp = CAN1->CAN_MB[i].CAN_MMR;

					if(protocol == CAN_PROTOCOL_2_0_A)
						offset = 18;
					port_pt->mbox[i].ul_id_msk = mask << offset;

					if(protocol == CAN_PROTOCOL_2_0_B)
						port_pt->mbox[i].ul_id_msk = CAN_MAM_MIDE;
					CAN1->CAN_MB[i].CAN_MMR = temp;
				}
			}
		}
		break;
	default:
		printf("\r\n\nWrong can device id\r\n\n");
		result = false;
		break;
	}

	return result;
}

uint8_t	can_port_set_protocol (uint8_t devid, PCAN_PORT port_pt, int8_t protocol){
	uint8_t result = true;

	/* the given port is correct? */
	if(port_pt->cbox_num == 0)
		return false; /* incorrect port name. error */

	switch(devid)
	{
	case CAN_DEV0 :
	case CAN_DEV1 :
		{
			port_pt->protocol = protocol;
		}
		break;

	default:
		printf("\r\n\nWrong can device id\r\n\n");
		result = false;
		break;
	}

	return result;
}

int16_t can_port_send (uint8_t devid, PCAN_PORT port_pt, uint8_t * buf_send_pt, int8_t size, int16_t timeout){
	int 		k;
	uint8_t 	result;
	uint8_t		offset = 0;
	int16_t 	cnt_down = timeout;
	uint8_t 	i;
	uint32_t 	temp;
	uint32_t 	sent_packets = 0;
	int8_t 	tx_length;

	/* the given port is correct? */
	if(port_pt->cbox_num == 0)
		return false; /* incorrect port name. error */

	if(port_pt->tx_box_num == NO_TX_BOX)
		return false; /* this port has no Tx mailbox */

	if(size < 1)
		return false;

	switch( devid )
	{
	/* Device ID : 1 - Atmel CAN Controller */
		case CAN_DEV0 :
		/* start transmit process */

		for(;;)
		{
			while(cnt_down > ERROR_CAN_TX_TIMEOUT_BEFORESEND){
				if((can_mailbox_get_status( CAN0, port_pt->tx_box_num ) & CAN_MSR_MRDY) == CAN_MSR_MRDY )
					break;
				cnt_down--;
				for (k=0; k < CAN_POLL_DELAY; k++)
					__asm__ __volatile__ ( "NOP" );
			}

			/* mailbox not ready yet */
			/* timeout : cnt_down - meaning
			 *   -1    :    -2     - 무한 블로킹 상태
			 *    0    :    -1     - 블로킹하지 않음. 당시 상태만을 1회 체크
			 *   양수      :   이전값-1 - 타임아웃
			 */
			if(cnt_down == ERROR_CAN_TX_TIMEOUT_BEFORESEND)
				return false;

			port_pt->mbox[port_pt->tx_box_num].ul_datal = 0;
			port_pt->mbox[port_pt->tx_box_num].ul_datah = 0;

			port_pt->mbox[port_pt->tx_box_num].uc_length = 8;
			if(size < 8)
				port_pt->mbox[port_pt->tx_box_num].uc_length = size;

			tx_length = port_pt->mbox[port_pt->tx_box_num].uc_length;
			for( i = 0; i < tx_length; i++ )
			{
				temp = *buf_send_pt++;
				offset = 8 * i;

				if (i > 3)
					port_pt->mbox[port_pt->tx_box_num].ul_datah |= temp << ( offset - 32 );
				else
					port_pt->mbox[port_pt->tx_box_num].ul_datal |= temp << offset;
			}
			can_mailbox_write(CAN0, &port_pt->mbox[port_pt->tx_box_num]);
			can_global_send_transfer_cmd(CAN0, 1 << port_pt->tx_box_num);

			sent_packets += tx_length;
			size -= 8;

			if(size < 1)
				break;

			for (k=0; k < CAN_INTER_MESSAGE_DELAY; k++)
			{
				__asm__ __volatile__ ( "NOP" );
			}
		}
		result = sent_packets;
		break;
		case CAN_DEV1 :
		/* start transmit process */

		for(;;)
		{
			while(cnt_down > ERROR_CAN_TX_TIMEOUT_BEFORESEND){
				if( (can_mailbox_get_status( CAN1, port_pt->tx_box_num ) & CAN_MSR_MRDY) == CAN_MSR_MRDY )
					break;
				cnt_down--;
				for (k=0; k < CAN_POLL_DELAY; k++)
					__asm__ __volatile__ ( "NOP" );
			}

			/* mailbox not ready yet */
			/* timeout : cnt_down - meaning
			 *   -1    :    -2     - 무한 블로킹 상태
			 *    0    :    -1     - 블로킹하지 않음. 당시 상태만을 1회 체크
			 *   양수      :   이전값-1 - 타임아웃
			 */
			if(cnt_down == ERROR_CAN_TX_TIMEOUT_BEFORESEND)
				return false;

			port_pt->mbox[port_pt->tx_box_num].ul_datal = 0;
			port_pt->mbox[port_pt->tx_box_num].ul_datah = 0;

			port_pt->mbox[port_pt->tx_box_num].uc_length = 8;
			if(size < 8)
				port_pt->mbox[port_pt->tx_box_num].uc_length = size;

			tx_length = port_pt->mbox[port_pt->tx_box_num].uc_length;
			for( i = 0; i < tx_length; i++ )
			{
				temp = *buf_send_pt++;
				offset = 8 * i;

				if (i > 3)
					port_pt->mbox[port_pt->tx_box_num].ul_datah |= temp << ( offset - 32 );
				else
					port_pt->mbox[port_pt->tx_box_num].ul_datal |= temp << offset;
			}
			can_mailbox_write(CAN1, &port_pt->mbox[port_pt->tx_box_num]);
			can_global_send_transfer_cmd(CAN1, 1 << port_pt->tx_box_num);

			sent_packets += tx_length;
			size -= 8;

			if(size < 1)
				break;

			for (k=0; k < CAN_INTER_MESSAGE_DELAY; k++)
			{
				__asm__ __volatile__ ( "NOP" );
			}
		}
		result = sent_packets;
		break;
	default:
		printf("\r\n\nWrong can device id\r\n\n");
		result = false;
		break;
	}

	return result;
}

int16_t can_port_recv (uint8_t devid, PCAN_PORT port_pt, uint8_t * buf_recv_pt, uint8_t size, int16_t timeout){
	uint8_t result;
	uint8_t rx_data_qty = 0;
	unsigned int count = 0;
	CAN_DATA recvData;
	/* the given port is correct? */
	if(port_pt->cbox_num == 0)
		return false; /* incorrect port name. error */

	if(size == 0)
		return false;

	switch( devid )
	{
	case CAN_DEV0 :
	case CAN_DEV1 :
		{
			msgq_getcount(port_pt->msgq_rxbuf, &count);

			if(( timeout = 0 ) && ( count < size ))
				return false;

			if(( size % 8 ) == 0 )
			{

				while(size>0)
				{
					if( msgq_receive_timed(port_pt->msgq_rxbuf, (unsigned char * )(&recvData), timeout) == 0 )
					{
						*buf_recv_pt++ = ((recvData.data_high)&0x000000FF);
						*buf_recv_pt++ = ((recvData.data_high)&0x0000FF00) >> 8;
						*buf_recv_pt++ = ((recvData.data_high)&0x00FF0000) >> 16;
						*buf_recv_pt++ = ((recvData.data_high)&0xFF000000) >> 24;

						*buf_recv_pt++ = ((recvData.data_low)&0x000000FF);
						*buf_recv_pt++ = ((recvData.data_low)&0x0000FF00) >> 8;
						*buf_recv_pt++ = ((recvData.data_low)&0x00FF0000) >> 16;
						*buf_recv_pt++ = ((recvData.data_low)&0xFF000000) >> 24;

						rx_data_qty += 8;
						size -= 8;
					}
					else{ /* if there is nothing to read, what shall be done??? */
	//					break;
					}
				}
			}
			else
				return false;

			result = rx_data_qty;
		}
		break;

	/* Other Devices - Not implemented yet */
	default:
		printf("\r\n\nWrong can device id\r\n\n");
		result = false;
		break;
	}

	return result;
}
