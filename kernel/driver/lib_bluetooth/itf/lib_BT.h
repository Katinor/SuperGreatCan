/*
 * BT_api.h
 *
 *  Created on: 2017. 4. 6.
 *      Author: JKwang
 */

#ifndef APPLICATION_PROJECT_UBITOOLS_EXE_UBINOS_TEST_ITF_BT_API_H_
#define APPLICATION_PROJECT_UBITOOLS_EXE_UBINOS_TEST_ITF_BT_API_H_





#endif /* APPLICATION_PROJECT_UBITOOLS_EXE_UBINOS_TEST_ITF_BT_API_H_ */
/* -------------------------------------------------------------------------
 Include
 ------------------------------------------------------------------------- */
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
/*
#include "lib_new_estk_api/itf/new_estk_led.h"
#include "lib_new_estk_api/itf/new_estk_glcd.h"

#include "../kernel/porting/lib_sam4e/sam/utils/cmsis/sam4e/include/sam4e16e.h"
#include "../kernel/porting/lib_sam4e/sam/boards/sam4e_ek/sam4e_ek.h"

#include "../../../../kernel/porting/lib_sam4e/common/services/gpio/sam_gpio/sam_gpio.h"
#include "../../../../kernel/porting/lib_sam4e/sam/drivers/spi/spi.h"
#include "../../../../kernel/driver/lib_new_estk_api/itf/new_estk_ioport_set.h"
#include "../../../../kernel/porting/lib_sam4e/common/services/ioport/sam/ioport_pio.h"
#include "../../../../kernel/porting/lib_sam4e/common/services/spi/sam_spi/spi_master.h"
*/

#include "../../../../kernel/driver/lib_bluetooth/itf/BT_Module_Interface.h"
#include "BT_Module_Interface.h"

// user header file include
void BT_interrupt_setting(void *pin_isr1);

void BT_control_task(void * arg);
void BT_EVT_task(void * arg);
static void BT_centraltask(void * arg);
static void BT_peripheraltask(void * arg);
static void lcd_outputtask();

int BT_Main_event_send(uint8_t Main_evt, uint8_t Main_evt_state, uint8_t* msg);
int BT_USER_event_send(uint8_t Main_evt, uint8_t Main_evt_state, uint8_t* msg);

void BT_WRITE_CMD(uint8_t *buf, uint8_t CMD);
void BT_READ_CMD(uint8_t *buf);
void BT_ACK_CHECK(uint8_t* buf);
void BT_ACK_SET(uint8_t cmd);

void SWITCH_isr1(void);
void SWITCH_isr2(void);
void PIN_isr1(void);
void BT_interrupt_setting(void *pin_isr1);

void SPI_READ_DATA(uint8_t *data, uint32_t len);
void SPI_WRITE_DATA(uint8_t *data, uint32_t len);

//--------------------------------------------------------------USER API-------------------
//int8_t BT_INIT(uint8_t role, module_id_st setup_module_id);
int8_t BT_INIT(uint8_t role, module_id_st setup_module_id, msgq_pt* user_event_queue);
int8_t BT_DATA_SEND(uint8_t role, uint8_t *data_msg );
int8_t BT_ADV_START();
int8_t BT_SCAN_START();
int8_t BT_CONNECT( module_id_st setup_module_id);
int8_t BT_DISCONNECT();
/* -------------------------------------------------------------------------
 Global variables
 ------------------------------------------------------------------------- */
#define MAIN_MSGQ_MAX_COUNT	20

#define CS 					  (PIO_PA11_IDX)	//11
#define CS_FLAGS			  (PIO_OUTPUT_1 | PIO_DEFAULT)

#define RESET 					(PIO_PA18_IDX)	//21
#define RESET_FLAGS			  (PIO_OUTPUT_1 | PIO_DEFAULT)

#define SPI_MISO_GPIO         (PIO_PA12_IDX)	//12
#define SPI_MOSI_GPIO         (PIO_PA13_IDX)	//13
#define SPI_SPCK_GPIO         (PIO_PA14_IDX)	//14
#define BT_PIN_INT			  (PIO_PA16_IDX)	//16or17
#define BT_PIN_INT_FLAGS	  (PIO_INPUT | PIO_DEFAULT)

//switch interrupt
#define SW1_INTERRUPT_PIN_PIO PIO_PA19
#define SW2_INTERRUPT_PIN_PIO PIO_PA20

/*---------------------------
 spi configuration
 -------------------------*/
#define SPI_CHIP_SEL 	1
#define SPI_CHIP_PCS	spi_get_pcs(SPI_CHIP_SEL)

#define SPI_CLK_POLARITY	0
#define SPI_CLK_PHASE		1
#define SPI_DLYBS			0x00
#define SPI_DLYBCT			0x00
#define SPI_BUFFER_SIZE 	32



//Main event Structure
typedef struct {
	uint8_t event;
	uint8_t status;
	uint8_t* msg;
} BT_Evt_t;

//BT Module Role State
enum {
	ROLE_STATE_IDLE, ROLE_STATE_PERIPHERAL, ROLE_STATE_CENTRAL,
};

//PB BLE Event State
enum {
	PB_BLE_CONNECTION_EVT_ST = 0,
	PB_BLE_DISCONNECTION_EVT_ST,
	PB_BLE_PACKET_REV_EVT_ST,
	PB_BLE_PACKET_WRITE_REP_EVT_ST,
};

//PB BLE Event State
enum {
	CONTROL_ADV_START_ST = 0,
	CONTROL_ADV_STOP_ST,
	CONTROL_SCAN_START_ST,
	CONTROL_CONNECTION_START_ST,
	CONTROL_DISCONNECTION_ST,
	CONTROL_PACKET_SEND_ST,
};

//event
enum {
	BT_SW1_EVT = 0, BT_SW2_EVT, BTM_READ_INT_EVT
};
//state
enum {
	SWITCH_TEST = 0, BT_INT_PIN,
};

enum {
	//common event
	BTM_EVT_READ_INT = 0,
	BTM_EVT_CMD_INIT,

	//peripheral event
	BTM_EVT_PE_CMD_ADV_START,
	BTM_EVT_PE_CMD_ADV_STOP,
	BTM_EVT_PE_CMD_DATA_SEND,

	//central event
	BTM_EVT_CE_CMD_SCAN_START,
	BTM_EVT_CE_CMD_SCAN_STOP,
	BTM_EVT_CE_CMD_CONNECT,
	BTM_EVT_CE_CMD_DISCONNECT,
	BTM_EVT_CE_CMD_DATA_SEND,
};
/*enum {
	//main_interface_event
	MAIN_EVT_CE_CMD_DATA_READ=0,
	MAIN_EVT_PE_CMD_DATA_READ,
	MAIN_EVT_CE_CMD_DISCONNECTED,
	MAIN_EVT_PE_CMD_DISCONNECTED,
	MAIN_EVT_CE_CMD_CONNECTED,
	MAIN_EVT_PE_CMD_CONNECTED,

};*/
enum {
	BT_EVT_CE_DATA_READ=0,
	BT_EVT_PE_DATA_READ,
	BT_EVT_DISCONNECTED,
	BT_EVT_CONNECTED,
};

extern uint8_t check_ack_init_state ;

extern uint8_t check_ack_adv_state ;
extern uint8_t check_ack_adv_stop_state ;

extern uint8_t check_scan_done_state ;
extern uint8_t check_ack_scan_stop_state;

extern uint8_t check_ack_connected_state ;
extern uint8_t check_ack_disconnected_state ;

extern uint8_t check_ack_data_send_state ;

