/*
 * lib_default.h
 *
 *  Created on: 2015. 7. 8.
 *      Author: jaegeun
 */

#ifndef APPLICATION_LIB_SENSORCALIB_ITF_LIB_SENSORCALIB_H_
#define APPLICATION_LIB_SENSORCALIB_ITF_LIB_SENSORCALIB_H_

/**
 * @ file sensorcalib.h
 *
 * @ Brief sensorcalib 컴포넌트 자료형 및 함수정의
 */

/*********************************************************
 * sensor level states
 *********************************************************/
#define MAX_TOUCH_LEVEL		2 	// touch sensor : OPENED, PRESSED
#define MAX_LIGHT_LEVEL		3 	// light sensor : DARK, DIM, BRIGHT
#define MAX_SOUND_LEVEL		4	// sound sensor : QUIET, TALKING, SHOUT, LOUD
#define MAX_SONA_LEVEL		3 	// ultra sensor : NEAR, MIDDLE, FAR

/*********************************************************
 * sensor level value
 *********************************************************/
#define TOUCH_OPENED		0	// touch sensor OPENED
#define TOUCH_CLOSED		1   // touch sensor CLOSED

#define LIGHT_DARK			0	// light sensor dark
#define LIGHT_DIM			1	// light sensor dim
#define LIGHT_BRIGHT		2 	// light sensor bright

#define SOUND_QUIET			0	// sound sensor quiet
#define SOUND_TALKING		1	// sound sensor talking
#define SOUND_SHOUT			2	// sound sensor shout
#define SOUND_LOUD			3	// sound sensor loud

#define SONA_NEAR			0	//Test for near
#define SONA_MIDDLE			1	//Test for middle
#define SONA_FAR			2	//Test for far

/*********************************************************
 * sensor value
 *********************************************************/
static int touch_value[MAX_TOUCH_LEVEL] = {961, 181};
static int light_value[MAX_LIGHT_LEVEL] = {0, 0, 0};
static int sound_value[MAX_SOUND_LEVEL] = {932, 780, 395, 63};
static int sona_value[MAX_SONA_LEVEL] = {0, };

void lib_sensorcalib_component_init( void );
void calibSensor(char port, char states, int * value_p);
void calibEV3Sensor(char port, char states, int * value_p);
int get_level(int value, char state, int *value_p);

#endif /* APPLICATION_LIB_SENSORCALIB_ITF_LIB_SENSORCALIB_H_ */
