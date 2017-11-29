/**
 * @file main.c
 *
 * @brief  Main of TAL Example - Performance_Test
 *
 * $Id: at86rf_API_main.c,v 1.1 2012-03-21 11:22:24 essusige Exp $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#include "../../../../ubiconfig.h"

#if	(APP_TYPE__API_main 	== APP_TYPE)
//#include "at86rf_API_main.h"
/* === INCLUDES ============================================================ */
#include "../../../../itf/lib_at86rf.h"
#include "itf_ubinos/itf/ubinos.h"
#include "../../../PAL/Inc/pal.h"
#include "../../../TAL/Inc/tal.h"
#include "../../../TFA/Inc/tfa.h"
#include "../../../Include/_app_config.h"
#include "../../../Include/ieee_const.h"
#include "../../../Resources/Buffer_Management/Inc/bmm.h"
#include "../../../Include/indirect_data_structures.h"
#include "../../Helper_Files/SIO_Support/Inc/sio_handler.h"

/* === GLOBALS ============================================================= */
bool app_tx_done;
uint8_t storage_buffer[LARGE_BUFFER_SIZE];
frame_info_t *tx_frame_info;
uint8_t frame_length = 20;
uint32_t frames_to_transmit;
bool ack_request = true;
bool csma_enabled = true;
bool retry_enabled = true;
uint8_t API_payload_length;
char API_payload_buffer[aMaxPHYPacketSize];

/* === PROTOTYPES ========================================================== */

void app_tal_rx_frame_cb(frame_info_t *mac_frame_info, uint8_t lqi);
void app_tal_tx_frame_done_cb(retval_t status, frame_info_t *frame);

/* === C_buffer definition ====================================================== */
typedef struct {
	unsigned int read_pointer;
	unsigned int write_pointer;
	unsigned int data_count;
	unsigned int buffer_size;
	unsigned char* data_buffer;
} c_queue;

unsigned int Circular_queue_create(c_queue* queue, unsigned int size);
unsigned int Circular_queue_write(c_queue* queue, unsigned char data);
unsigned char Circular_queue_read(c_queue* queue);
unsigned int Circular_queue_write_buffer(c_queue* queue, unsigned char* buffer, unsigned int length);
unsigned int Circular_queue_read_buffer(c_queue* queue, unsigned char* buffer, unsigned int length);
unsigned int Circular_queue_delete(c_queue* queue);

/* === IMPLEMENTATION ====================================================== */
API_packet_info rx_packet_temp, tx_packet_temp;

typedef void (* api_rx_cb_ft)(API_packet_info* rx_packet);
typedef void (* api_tx_cb_ft)(API_packet_info* tx_packet);

api_rx_cb_ft _rx_cb_ft;
api_tx_cb_ft _tx_cb_ft;
unsigned char temp_rx_packet[127], temp_tx_packet[127];

c_queue tx_buffer;
unsigned int tx_packet_count = 0;

sem_pt rf_sem_guard;

void at86rf_api_init(int task_priority, unsigned int task_stack_depth, void *rx_cb_ft, void *tx_cb_ft)
{
	unsigned int r;
	_rx_cb_ft = rx_cb_ft;
	_tx_cb_ft = tx_cb_ft;

	sem_create_ext(&rf_sem_guard, 1, 20, 0);
	Circular_queue_create(&tx_buffer, 500);
	r = task_create(NULL, (taskfunc_ft) at86rf_app_main, NULL, task_priority, task_stack_depth, "at86rf231_api");
	if (0 != r) {
		logme("fail at task_create\r\n");
	}
}
/**
 * @brief Main function of the Performance_Test application
 */
int at86rf_app_main(void)
{
//	task_sleep(1000);

	tal_rx_frame_cb_set( app_tal_rx_frame_cb );
	tal_tx_frame_done_cb_set( app_tal_tx_frame_done_cb );
    /* Initialize the TAL layer */
    if (tal_init() != MAC_SUCCESS)
    {
        // something went wrong during initialization
        pal_alert();
    }

    /* set source address */
//    set_addr(SRC,  src_srt_addr,  src_pan_addr);
    /* Configure the frame sending; e.g. set short address */
    configure_frame_sending();

    at86rf_ack_request_enable();
    at86rf_csma_enable();
    at86rf_retry_enable();
    at86rf_set_page(17);

	tal_rx_enable(PHY_RX_ON);
	set_trx_state(CMD_RX_AACK_ON);
	tx_packet_count = 0;

    /* Endless while loop */
    while (1)
    {
    	sem_take(rf_sem_guard);
        pal_task(); /* Handle platform specific tasks, like serial interface */
        tal_task(); /* Handle transceiver specific tasks */
        at86rf_app_task(); /* Application task */
    }

    return 0;
}


/**
 * @brief Application task
 */
unsigned int time_out;
void at86rf_app_task(void)
{
	if(tx_packet_count > 0)
	{
		uint8_t *payload;

		/* To achieve maximum phy frame length, overhead needs to substracted. */
		tx_frame_info->payload_length = Circular_queue_read(&tx_buffer);//API_payload_length;//frame_length - FRAME_OVERHEAD;

		tx_frame_info->frame_ctrl = FCF_FRAMETYPE_DATA |
									FCF_SET_SOURCE_ADDR_MODE(FCF_SHORT_ADDR) |
									FCF_SET_DEST_ADDR_MODE(FCF_SHORT_ADDR);
		if (ack_request)
		{
			tx_frame_info->frame_ctrl |= FCF_ACK_REQUEST;
		}

		/*
		 * Assign dummy payload values.
		 * Payload is stored to the end of the buffer avoiding payload copying by TAL.
		 */
		tx_frame_info->payload = (uint8_t *)tx_frame_info + (LARGE_BUFFER_SIZE - 2 - tx_frame_info->payload_length);  // 2 = FCF_SIZE
		payload = tx_frame_info->payload;
//		for (i = 0; i < API_payload_length; i++)
//		{
//			*payload++ = API_payload_buffer[i]; // dummy value
//		}
		Circular_queue_read_buffer(&tx_buffer,&payload[0],tx_frame_info->payload_length);
		app_tx_done = false;

		tx_frame_info->seq_num++;
		if(tx_frame_info->seq_num > 10000)
			tx_frame_info->seq_num = 0;

		if (csma_enabled)
		{
			tal_tx_frame(tx_frame_info, CSMA_UNSLOTTED, retry_enabled);
		}
		else
		{
			tal_tx_frame(tx_frame_info, NO_CSMA_NO_IFS, retry_enabled);
		}

		time_out = 0;
		while((app_tx_done == false)&&!(time_out == 5000))
		{
			tal_task();
			time_out++;
		}

		tal_rx_enable(PHY_RX_ON);
//		set_trx_state(CMD_RX_AACK_ON);
		tx_packet_count--;
	}
}


/**
 * @brief Callback that is called if data has been received by trx.
 *
 * @param mac_frame_info    Pointer to received data structure
 * @param lqi               LQI value of the received frame
 */

#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
void tal_rx_frame_cb(frame_info_t *mac_frame_info, uint8_t lqi)
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
void app_tal_rx_frame_cb(frame_info_t *mac_frame_info, uint8_t lqi)
#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
{
	if(_rx_cb_ft != NULL)
	{
		unsigned int i;
		rx_packet_temp.dst_pan_id = mac_frame_info->dest_panid;
		rx_packet_temp.dst_short_addr = mac_frame_info->dest_address;
		rx_packet_temp.lqi = lqi;
		rx_packet_temp.payload_length = mac_frame_info->payload_length;
		rx_packet_temp.src_pan_id = mac_frame_info->src_panid;
		rx_packet_temp.src_short_addr = mac_frame_info->src_address;

		for(i = 0; i<mac_frame_info->payload_length; i++)
		{
//			temp_rx_packet[i] = mac_frame_info->payload[i];
			rx_packet_temp.payload[i] = mac_frame_info->payload[i];
		}
//		_rx_cb_ft(mac_frame_info->src_address, mac_frame_info->src_panid,
//				mac_frame_info->dest_address, mac_frame_info->dest_panid, &temp_rx_packet[i], mac_frame_info->payload_length, lqi);
		_rx_cb_ft(&rx_packet_temp);

	}
	/* free buffer that was used for frame reception */
	bmm_buffer_free((buffer_t *)(mac_frame_info->buffer_header));
	sem_give(rf_sem_guard);
}


/**
 * @brief Callback that is called once tx is done.
 *
 * @param status    Status of the transmission procedure
 * @param frame     Pointer to the transmitted frame structure
 */
#if 	   (1 == EXCLUDE_MODIFICATION_FOR_UBINOS)
void tal_tx_frame_done_cb(retval_t status, frame_info_t *frame)
#else 	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
void app_tal_tx_frame_done_cb(retval_t status, frame_info_t *frame)
#endif	/* (1 == EXCLUDE_MODIFICATION_FOR_UBINOS) */
{
	if(_tx_cb_ft != NULL)
	{
//		unsigned int i;
//		for(i = 0; i<frame->payload_length; i++)
//		{
//			temp_tx_packet[i] = frame->payload[i];
//		}
//		_tx_cb_ft(frame->src_address, frame->src_panid,
//				frame->dest_address, frame->dest_panid, &temp_tx_packet[i], frame->payload_length);
		unsigned int i;
		tx_packet_temp.dst_pan_id = frame->dest_panid;
		tx_packet_temp.dst_short_addr = frame->dest_address;
		tx_packet_temp.lqi = 0x00;
		tx_packet_temp.payload_length = frame->payload_length;
		tx_packet_temp.src_pan_id = frame->src_panid;
		tx_packet_temp.src_short_addr = frame->src_address;

		for(i = 0; i<frame->payload_length; i++)
		{
//			temp_rx_packet[i] = mac_frame_info->payload[i];
			tx_packet_temp.payload[i] = frame->payload[i];
		}
		_tx_cb_ft(&tx_packet_temp);
	}

	app_tx_done = true;
	sem_give(rf_sem_guard);
}

/**
 * @brief Configure the frame sending
 */
void configure_frame_sending(void)
{
    uint8_t temp_value[2];

    /*
     * Set TAL PIBs
     * Use: retval_t tal_pib_set(uint8_t attribute, pib_value_t *value);
     */
    temp_value[0] = (uint8_t)SRC_PAN_ID;
    temp_value[1] =  (uint8_t)(SRC_PAN_ID >> 8);
    tal_pib_set(macPANId, (pib_value_t *)&temp_value);

    temp_value[0] = (uint8_t)OWN_SHORT_ADDR;
    temp_value[1] =  (uint8_t)(OWN_SHORT_ADDR >> 8);
    tal_pib_set(macShortAddress, (pib_value_t *)&temp_value);

    temp_value[0] = (uint8_t)DEFAULT_CHANNEL;
    tal_pib_set(phyCurrentChannel, (pib_value_t *)&temp_value);

    /* Init tx frame info structure value that do not change during program execution */
    tx_frame_info = (frame_info_t *)storage_buffer;
    tx_frame_info->msg_type = MCPS_MESSAGE;  // use data frame type for Performance example
    tx_frame_info->seq_num = (uint8_t)rand();
    tx_frame_info->dest_panid = DST_PAN_ID;
    tx_frame_info->dest_address = DST_SHORT_ADDR;
    tx_frame_info->src_panid = SRC_PAN_ID;
    tx_frame_info->src_address = OWN_SHORT_ADDR;
}


void at86rf_set_page(unsigned char ch_page)
{
	tal_pib_set(phyCurrentPage, (pib_value_t *)&ch_page);
}

void at86rf_ack_request_enable()
{
	ack_request = true;
}

void at86rf_ack_request_disable()
{
	ack_request = false;
}

void at86rf_csma_enable(void)
{
	csma_enabled = true;
}

void at86rf_csma_disable(void)
{
	csma_enabled = false;
}

void at86rf_retry_enable()
{
	retry_enabled = true;
}

void at86rf_retry_disable()
{
	retry_enabled = false;
}

void at86rf_set_frame_length(unsigned char num)
{
	if (num > aMaxPHYPacketSize)
	{
		frame_length = aMaxPHYPacketSize;
		printf("frame length value is wrong!!\r\n");
		printf("frame length value must be between 11 and 127!!\r\n");
		printf("set frame length value = 127\r\n");
	}
	else
	{
		frame_length = (uint8_t)num;
	}
}

void at86rf_set_addr(unsigned char separator,  unsigned short sht_addr,  unsigned short pan_id)
{
	if(separator == SRC )
	{
		uint8_t temp_value[2];

	    tx_frame_info->src_panid = pan_id;
	    tx_frame_info->src_address = sht_addr;

	    temp_value[0] = (uint8_t)pan_id;
	    temp_value[1] =  (uint8_t)(pan_id >> 8);
	    tal_pib_set(macPANId, (pib_value_t *)&temp_value);

	    temp_value[0] = (uint8_t)sht_addr;
	    temp_value[1] =  (uint8_t)(sht_addr >> 8);
	    tal_pib_set(macShortAddress, (pib_value_t *)&temp_value);
	}
	else if(separator == DST)
	{
	    tx_frame_info->dest_panid = pan_id;
	    tx_frame_info->dest_address = sht_addr;
	}
	else
	{
		printf("Error : set_addr = Wrong parameter(separator).\r\n");
	}
}

void at86rf_set_tx_power(int tx_power)
{
	unsigned char temp_var;
	temp_var = CONV_DBM_TO_phyTransmitPower(tx_power);
	tal_pib_set(phyTransmitPower, (pib_value_t *)&temp_var);
}

void at86rf_set_channel(unsigned char channel)
{
	tal_pib_set(phyCurrentChannel, (pib_value_t *)&channel);
}

void at86rf_send(unsigned short dst_sht_addr,  unsigned short dst_pan_id, char *packet, char packet_length)
{
	at86rf_set_addr(DST, dst_sht_addr, dst_pan_id);
//	API_payload_length = packet_length;
//	for(i=0; i<packet_length; i++)
//	{
//		API_payload_buffer[i] = packet[i];
//	}
	Circular_queue_write(&tx_buffer,packet_length);
	Circular_queue_write_buffer(&tx_buffer, &packet[0], packet_length);
	tx_packet_count++;
//
	sem_give(rf_sem_guard);
}
#endif /* (APP_TYPE__API_main 	== APP_TYPE) */

//===============C_buffer implementation==================================
unsigned int Circular_queue_create(c_queue* queue, unsigned int size)
{
	queue->read_pointer = 0;
	queue->write_pointer = 0;
	queue->data_count = 0;
	queue->buffer_size = size;
	queue->data_buffer = malloc(sizeof(unsigned char)*size);
	return 0;
}

unsigned int Circular_queue_write(c_queue* queue, unsigned char data)
{
	unsigned int remain_buffer_size;
	remain_buffer_size = queue->buffer_size - queue->data_count;
	if((remain_buffer_size)<=0)
	{
		printf("Circular queue : write error = buffer is full!!\r\n");
		return 1;
	}
	else
	{
		if(queue->write_pointer == queue->buffer_size)
			queue->write_pointer = 0;
		queue->data_buffer[queue->write_pointer++] = data;
		queue->data_count++;
		return 0;
	}
}

unsigned char Circular_queue_read(c_queue* queue)
{
	unsigned char temp = 0;

	if((queue->data_count <= 0))
	{
		printf("Circular queue : read error = buffer is empty!!\r\n");
		return 1;
	}
	else
	{
		 temp = queue->data_buffer[queue->read_pointer++];
		 if(queue->read_pointer == queue->buffer_size)
			 queue->read_pointer = 0;
		 queue->data_count--;
		 return temp;
	}

}

unsigned int Circular_queue_write_buffer(c_queue* queue, unsigned char* buffer, unsigned int length)
{
	unsigned int remain_buffer_size, i;
	remain_buffer_size = queue->buffer_size - queue->data_count;
	if(remain_buffer_size < length)
	{
		printf("Circular queue : write buffer error = remained buffer space is not enough!!\r\n");
		return 1;
	}
	else
	{
		for(i=0; i<length; i++)
		{
			if(queue->write_pointer == queue->buffer_size)
				queue->write_pointer = 0;

			queue->data_buffer[queue->write_pointer++] = buffer[i];
			queue->data_count++;
		}
		return 0;
	}
}
unsigned int Circular_queue_read_buffer(c_queue* queue, unsigned char* buffer, unsigned int length)
{
	unsigned char temp,i;

	if((queue->data_count < length))
	{
		printf("Circular queue : read buffer error = read buffer's length is more than data count!!\r\n");
		return 1;
	}
	else
	{
		for(i=0;i<length;i++)
		{
			temp = queue->data_buffer[queue->read_pointer++];
			if(queue->read_pointer == queue->buffer_size)
				queue->read_pointer = 0;
			queue->data_count--;
			buffer[i] = temp;
		}
		return 0;
	}
}
unsigned int Circular_queue_delete(c_queue* queue)
{
	free(queue->data_buffer);
	return 0;
}
/* EOF */
