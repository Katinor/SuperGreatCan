/*
 * lib_motor_driver.h
 *
 *  Created on: 2015. 7. 3.
 *      Author: Administrator
 */

#ifndef APPLICATION_LIB_MOTOR_DRIVER_ITF_LIB_MOTOR_DRIVER_H_
#define APPLICATION_LIB_MOTOR_DRIVER_ITF_LIB_MOTOR_DRIVER_H_

#include "sam4e.h"
#include "../../lib_ubi_pio/itf/lib_ubi_pio.h"
#include "../kernel/porting/lib_sam4e/sam/utils/cmsis/sam4e/include/sam4e16e.h"
#include "pio/pio.h"
#include "../kernel/porting/lib_sam4e/sam/drivers/pio/pio.h"
/** PWM frequency in Hz. */
#define PWM_FREQUENCY               83000
/** Maximum duty cycle(pos) value. */
#define MAX_DUTY_CYCLE              1024
/** Maximum duty cycle(neg) value. */
#define MAX_REVERSE_DUTY_CYCLE      -1024
/** Minimum duty cycle value. */
#define MIN_DUTY_CYCLE              0

/** @define PWMC PWM0 pin definition: Output High */
#define PIN_ESTK_PWMC_PWMH0  {1u << 20, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT}
/** @define PWMC PWM0 pin definition: Output Low */
#define PIN_ESTK_PWMC_PWML0  {1u << 24, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT}
/** @define PWMC PWM1 pin definition: Output High */
#define PIN_ESTK_PWMC_PWMH1  {1u << 21, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT}
/** @define PWMC PWM1 pin definition: Output Low */
#define PIN_ESTK_PWMC_PWML1  {1u << 25, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT}
/** @define PWMC PWM2 pin definition: Output High */
#define PIN_ESTK_PWMC_PWMH2  {1u << 22, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT}
/** @define PWMC PWM2 pin definition: Output Low */
#define PIN_ESTK_PWMC_PWML2  {1u << 26, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT}
/** @define PWMC PWM3 pin definition: Output High */
#define PIN_ESTK_PWMC_PWMH3  {1u << 23, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT}
/** @define PWMC PWM3 pin definition: Output Low */
#define PIN_ESTK_PWMC_PWML3  {1u << 27, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT}

/** @define PWM pins definition for CH0 */
#define PIN_ESTK_PWM_CH0 PIN_ESTK_PWMC_PWMH0, PIN_ESTK_PWMC_PWML0
/** @define PWM pins definition for CH1 */
#define PIN_ESTK_PWM_CH1 PIN_ESTK_PWMC_PWMH1, PIN_ESTK_PWMC_PWML1
/** @define PWM pins definition for CH2 */
#define PIN_ESTK_PWM_CH2 PIN_ESTK_PWMC_PWMH2, PIN_ESTK_PWMC_PWML2
/** @define PWM pins definition for CH3 */
#define PIN_ESTK_PWM_CH3 PIN_ESTK_PWMC_PWMH3, PIN_ESTK_PWMC_PWML3

#define 	PWM_CMR0_CPRE_MCKA                 (0xB) // (PWMC_CH)
#define PWM_CMR0_CPOL (1u<<9) /**< \brief (PWM_CMR0) Channel Polarity */

/** @define NXT motor_encoder(0) configuration */
#define MOT0_ENC_INT	{1u << 29, PIOC, ID_PIOC, PIO_INPUT, PIO_IT_EDGE}
/** @define NXT motor_encoder(1) configuration */
#define MOT1_ENC_INT	{1u << 18, PIOD, ID_PIOD, PIO_INPUT, PIO_IT_EDGE}
/** @define NXT motor_encoder(2) configuration */
#define MOT2_ENC_INT	{1u << 28, PIOD, ID_PIOD, PIO_INPUT, PIO_IT_EDGE}
/** @define NXT motor_encoder(3) configuration */
#define MOT3_ENC_INT	{1u << 30, PIOD, ID_PIOD, PIO_INPUT, PIO_IT_EDGE}

static const Pin ESTK_MOT0_ENCODER_INT = MOT0_ENC_INT;
static const Pin ESTK_MOT1_ENCODER_INT = MOT1_ENC_INT;
static const Pin ESTK_MOT2_ENCODER_INT = MOT2_ENC_INT;
static const Pin ESTK_MOT3_ENCODER_INT = MOT3_ENC_INT;

static int a_current_count = 0;
static int a_last = 0;
static int b_current_count = 0;
static int b_last = 0;
static int c_current_count = 0;
static int c_last = 0;
static int d_current_count = 0;
static int d_last = 0;

void motor_init(void);

static void motor_pwm_set(Pio *PIOX, Pio *PIOY, unsigned int PinX, unsigned int PinY);

void motor_set(int port, int speed);


void motor_encoder(int pin, int line);

static void ESTK_motor_encoder_ISR(uint32_t ul_mask, uint32_t ul_id);

void encoder_init(void);

int encoder_get(int port);

void encoder_reset(int port);

#endif /* APPLICATION_LIB_MOTOR_DRIVER_ITF_LIB_MOTOR_DRIVER_H_ */
