/*
 * lib_at86rf.h
 *
 *  Created on : 2015. 7. 24.
 *  Modified on : 2015. 8. 26.
 *      Author: Taemin Hwang
 */

#ifndef APPLICATION_LIB_I2C_MASTER_ITF_LIB_DEFAULT_H_
#define APPLICATION_LIB_I2C_MASTER_ITF_LIB_DEFAULT_H_

/*
 * modify cfg folder, Applicaion folder in src
 *
 */

#ifdef	__cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "stdint.h"
#include "../ubiconfig.h"

#if		(APP_TYPE__NONE 							== APP_TYPE)
#elif	(APP_TYPE__TAL_EXAMPLE_PERFORMANCE_TEST 	== APP_TYPE)
	int at86rf_app_main(void);
#elif	(APP_TYPE__API_main 	== APP_TYPE)

	void at86rf_api_init(int task_priority, unsigned int task_stack_depth, void *rx_cb_ft, void *tx_cb_ft);
	int at86rf_app_main(void);
	typedef struct API_packet_info
	{
		unsigned short src_short_addr;
		unsigned short src_pan_id;
		unsigned short dst_short_addr;
		unsigned short dst_pan_id;
		char payload[127];
		char payload_length;
		char lqi;
	}API_packet_info;

	/* === MACROS ============================================================== */
	#define DEFAULT_CHANNEL         (20)
	#define DST_PAN_ID              (DEFAULT_PAN_ID)
	#define SRC_PAN_ID              (DEFAULT_PAN_ID)
	#define OWN_SHORT_ADDR          (0x0002)
	#define DST_SHORT_ADDR          (0x0002)
	#define FRAME_OVERHEAD          (13)
	#define DEFAULT_PAN_ID			0xA1A1
	#define DEFAULT_SHORT_ADDR		0xA1A1
	#define DEFAULT_PAGE			0
	#define SRC						0
	#define DST						1

	void at86rf_app_task(void);
	void configure_frame_sending(void);
	void at86rf_set_channel(unsigned char channel);
	void at86rf_set_page(unsigned char ch_page);
	void at86rf_ack_request_enable();
	void at86rf_ack_request_disable();
	void at86rf_csma_enable();
	void at86rf_csma_disable();
	void at86rf_retry_enable();
	void at86rf_retry_disable();
	void at86rf_set_frame_length(unsigned char num);
	void at86rf_send(unsigned short dst_sht_addr,  unsigned short dst_pan_id, char *packet, char packet_length);
	void at86rf_set_addr(unsigned char separator,  unsigned short sht_addr,  unsigned short PAN_addr);

#else
	#error "Unsupported APP_TYPE"
#endif

#ifdef	__cplusplus
}
#endif

#endif /* APPLICATION_LIB_I2C_MASTER_ITF_LIB_DEFAULT_H_ */
