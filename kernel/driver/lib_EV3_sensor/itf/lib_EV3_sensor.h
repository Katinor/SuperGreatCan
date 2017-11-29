/*
 * lib_EV3_sensor.h
 *
 *  Created on: 2015. 7. 8.
 *      Author: minsu
 */

#ifndef APPLICATION_LIB_EV3_SENSOR_ITF_LIB_EV3_SENSOR_H_
#define APPLICATION_LIB_EV3_SENSOR_ITF_LIB_EV3_SENSOR_H_


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*-----------------------------------------------------------------------------------------------------------------------*/
/**	@define Set, Clear	*/
#define SET								1
#define CLEAR							0

#define CLOCK 47923200
/**	@define Message values	*/
#define   BYTE_ACK                      0x04
#define   BYTE_NACK                     0x02
#define   CMD_SELECT                    0x43
#define   CMD_TYPE                      0x40
#define   CMD_MODES                     0x49
#define   CMD_SPEED                     0x52
#define   CMD_MASK                      0xC0
#define   CMD_INFO                      0x80
#define   CMD_LLL_MASK                  0x38
#define   CMD_LLL_SHIFT                 0x03
#define   CMD_MMM_MASK                  0x07
#define   CMD_DATA                      0xc0
#define   CMD_WRITE                     0x44

/** @define Color sensor type */
#define   TYPE_COLOR                    0x1d

/** @define RESET status */
#define RESET 							0x00
/** @define START status */
#define STARTED 						0x01
/** @define DATA MODE status */
#define DATA_MODE 						0x02

/** @define Maximum number of modes supported */
#define MAX_MODES 						10
/** @define The maximum number of data items in a sample */
#define MAX_DATA_ITEMS 					10
/** @define The time between heartbeats in milliseconds */
#define HEART_BEAT 						100

typedef	struct {
	unsigned char name[12];                     		// The mode name
	unsigned char symbol[4];                    		// The unit symbol
	unsigned char sets;                        			// The number of samples
	unsigned char data_type;                   			// The data type 0= 8bits, 1=16 bits, 2=32 bits, 3=float
	unsigned char figures;                    			// Number of significant digits
	unsigned char decimals;                   			// Number of decimal places
	float raw_low, raw_high;         					// Low and high values for raw data
	float si_low, si_high;            					// Low and high values for SI data
	float pct_low, pct_high;	      					// Low and high values for Percentage values
}EV3UartMode;

typedef struct {
		unsigned long speed;                           	// The required bit rate of the sensor
		int mode;                                     	// The current sensor mode
		unsigned char status;                           // The current status of the connection
		unsigned char modes;                            // The number of modes supported
		unsigned char views;                            // The number of views supported
		unsigned long last_nack;                       	// The time of the last heartbeat NACK
		unsigned char type;                             // The internal type encoding of the sensor
		int data_errors;                               	// Total number of data errors
		int value[MAX_DATA_ITEMS];                   	// The current value
		int num_samples;                               	// The current number of samples
		EV3UartMode* mode_array[MAX_MODES] ;           	// An array of EV3UARTMode objects
		unsigned char consecutive_errors;               // Number of sonsective errors
		int recent_messages;                           	// Number of recent messages
		int seq_num;
}EV3UARTSensor;

/** @define color type for ev3 color sensor COL_COLOR mode */
typedef enum EV3_COLOR{
	NONE,
	BLACK,
	BLUE,
	GREEN,
	YELLOW,
	RED,
	WHITE,
	BROWN
} ev3_color_t;

/** @define mode for ev3 color sensor */
typedef enum COLOR_MODE {
	COL_REFLECT,
	COL_AMBIENT,
	COL_COLOR
} color_mode_t;

/* Global variables */
EV3UARTSensor sensor;
uint8_t cm;

/* @function for Ev3 color sensor configure*/
/**----------------------------------------------------------------------------------------------------------------------*/
void initEV3UARTSensor(EV3UARTSensor* ev3uartsensor); 						// Create the sensor and specify the pins for SoftwareSerial
void begin(EV3UARTSensor* ev3uartsensor);                                   // Start communicating with the sensor
void end();                                    								// End communication with
int get_number_of_modes(EV3UARTSensor* ev3uartsensor);                      // Number of modes supported
void set_mode(EV3UARTSensor* ev3uartsensor,int mode);                       // Set the sensor to the specific mode
int get_current_mode(EV3UARTSensor* ev3uartsensor);                         // The current sensor mode
int sample_size(EV3UARTSensor* ev3uartsensor);                              // The number of items in a sample for the current mode
void fetch_sample(EV3UARTSensor* ev3uartsensor,float* sample, int offset);  // Fetch a sample in the current mode
int get_status(EV3UARTSensor* ev3uartsensor);                               // Get the status of the connection
EV3UartMode* get_mode(EV3UARTSensor* ev3uartsensor,int mode);               // Get the EV3UARTMode object for a specific mode
void reset(EV3UARTSensor* ev3uartsensor);                                   // Make the sensor reset
void send_write(unsigned char* bb, int len);            					// Send a WRITE command to the sensor
int get_type(EV3UARTSensor* ev3uartsensor);                                 // Get the LEGO type code for the sensor

void check_for_initiation(EV3UARTSensor* ev3uartsensor, int mod, uint8_t* ack_flag);
unsigned char read_byte();                              					// Read a byte from the sensor (synchronous)
unsigned long get_long(unsigned char* bb, int offset);  					// Helper method to get a long value
void get_string(unsigned char* string,unsigned char* bb, int len);          // Helper method to get a String value
float get_float(unsigned char* bb, int len);           						// Helper method to get a float value
int exp2(int val);                             								// Helper method for powers of 2
void send_select(uint8_t mode);
int get_int(unsigned char* bb, int offset);             					// Helper method to get an int
void timer_start();
/*-----------------------------------------------------------------------------------------------------------------------*/
void initEV3UARTMode();
/*-----------------------------------------------------------------------------------------------------------------------*/
void port_select(int port);
void UartRead_ISR();
void uart_buf_read(uint8_t* buf);

/** @function for user to use sensors*/
/*-----------------------------------------------------------------------------------------------------------------------*/
void ev3_sensor_init(int port, int mod);

int ev3_sensor_get(int port);
/*-----------------------------------------------------------------------------------------------------------------------*/

#endif /* APPLICATION_LIB_EV3_SENSOR_ITF_LIB_EV3_SENSOR_H_ */
